/**
 ******************************************************************************
 * @file    bsp_xxx.c
 * @author  STMicroelectronics
 * @version V1.0
 * @date    2013-xx-xx
 * @brief   spi font flash 底层应用函数bsp
 ******************************************************************************
 * @attention
 *
 * 实验平台:
 * 论坛
 * 淘宝
 *
 ******************************************************************************
 */

#include "bsp_font_flash.h"
#include "includes.h"
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

/**
  * @brief  FONT_SPI_FLASH初始化
  * @param  无
  * @retval 无
  */
void FONT_SPI_FLASH_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能SPI时钟 */
    FONT_FLASH_SPI_APBxClock_FUN ( FONT_FLASH_SPI_CLK, ENABLE );

    /* 使能SPI引脚相关的时钟 */
    FONT_FLASH_SPI_CS_APBxClock_FUN ( FONT_FLASH_SPI_CS_CLK|FONT_FLASH_SPI_SCK_CLK|
                                 FONT_FLASH_SPI_MISO_PIN|FONT_FLASH_SPI_MOSI_PIN, ENABLE );

    /* 配置SPI的 CS引脚，普通IO即可 */
    GPIO_InitStructure.GPIO_Pin = FONT_FLASH_SPI_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(FONT_FLASH_SPI_CS_PORT, &GPIO_InitStructure);

    /* 配置SPI的 SCK引脚*/
    GPIO_InitStructure.GPIO_Pin = FONT_FLASH_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(FONT_FLASH_SPI_SCK_PORT, &GPIO_InitStructure);

    /* 配置SPI的 MISO引脚*/
    GPIO_InitStructure.GPIO_Pin = FONT_FLASH_SPI_MISO_PIN;
    GPIO_Init(FONT_FLASH_SPI_MISO_PORT, &GPIO_InitStructure);

    /* 配置SPI的 MOSI引脚*/
    GPIO_InitStructure.GPIO_Pin = FONT_FLASH_SPI_MOSI_PIN;
    GPIO_Init(FONT_FLASH_SPI_MOSI_PORT, &GPIO_InitStructure);

    /* 停止信号 FLASH: CS引脚高电平*/
    FONT_SPI_FLASH_CS_HIGH();

    /* SPI 模式配置 */
    // FLASH芯片 支持SPI模式0及模式3，据此设置CPOL CPHA
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(FONT_FLASH_SPIx , &SPI_InitStructure);

    /* 使能 SPI  */
    SPI_Cmd(FONT_FLASH_SPIx , ENABLE);

}

/**
 * @brief  去FLASH扇区写保护
 * @param  Unprotect Sector    去写保护
 * @retval 无
 */
void FONT_SPI_FLASH_UnprotectSector(u32 SectorAddr)
{
    /* 发送FLASH写使能命令 */
    FONT_SPI_FLASH_WriteEnable();
    FONT_SPI_FLASH_WaitForWriteEnd();
    /* 擦除扇区 */
    /* 选择FLASH: CS低电平 */
    FONT_SPI_FLASH_CS_LOW();
    /* 发送扇区擦除指令*/
    FONT_SPI_FLASH_SendByte(JLX128_UnprotectSector);
    /*发送擦除扇区地址的高位*/
    FONT_SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /* 发送擦除扇区地址的中位 */
    FONT_SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /* 发送擦除扇区地址的低位 */
    FONT_SPI_FLASH_SendByte(SectorAddr & 0xFF);
    /* 停止信号 FLASH: CS 高电平 */
    FONT_SPI_FLASH_CS_HIGH();
    /* 等待擦除完毕*/
    FONT_SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief  擦除FLASH扇区
 * @param  SectorAddr：要擦除的扇区地址
 * @retval 无
 */
void FONT_SPI_FLASH_SectorErase(u32 SectorAddr)
{
    /* 发送FLASH写使能命令 */
    FONT_SPI_FLASH_WriteEnable();
    //FONT_SPI_FLASH_WaitForWriteEnd();
    /* 擦除扇区 */
    /* 选择FLASH: CS低电平 */
    FONT_SPI_FLASH_CS_LOW();
    /* 发送扇区擦除指令*/
    FONT_SPI_FLASH_SendByte(JLX128_BlockErase4K);
    /*发送擦除扇区地址的高位*/
    FONT_SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /* 发送擦除扇区地址的中位 */
    FONT_SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /* 发送擦除扇区地址的低位 */
    FONT_SPI_FLASH_SendByte(SectorAddr & 0xFF);
    /* 停止信号 FLASH: CS 高电平 */
    FONT_SPI_FLASH_CS_HIGH();
    /* 等待擦除完毕*/
    FONT_SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief  擦除FLASH扇区，整片擦除
 * @param  无
 * @retval 无
 */
void FONT_SPI_FLASH_ChipErase(void)
{
    /* 发送FLASH写使能命令 */
    FONT_SPI_FLASH_WriteEnable();
    FONT_SPI_FLASH_WaitForWriteEnd();
    /* 整块 Erase */
    /* 选择FLASH: CS低电平 */
    FONT_SPI_FLASH_CS_LOW();
    /* 发送整块擦除指令*/
    FONT_SPI_FLASH_SendByte(JLX128_ChipEraseFirst);
    FONT_SPI_FLASH_SendByte(JLX128_ChipEraseSecend);
    /* 停止信号 FLASH: CS 高电平 */
    FONT_SPI_FLASH_CS_HIGH();

    /* 等待擦除完毕*/
    FONT_SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
 * @param	pBuffer，要写入数据的指针
 * @param WriteAddr，写入地址
 * @param  NumByteToWrite，写入数据长度，必须小于等于FONT_SPI_FLASH_PerWritePageSize
 * @retval 无
 */
void FONT_SPI_FLASH_ByteWrite(u32 WriteAddr, u8 ch)
{
    /* 发送FLASH写使能命令 */
    FONT_SPI_FLASH_WriteEnable();
    //FONT_SPI_FLASH_WaitForWriteEnd();
    /* 选择FLASH: CS低电平 */
    FONT_SPI_FLASH_CS_LOW();

    /* 写页写指令*/
    FONT_SPI_FLASH_SendByte(JLX128_ByteProgram);
    /*发送写地址的高位*/
    FONT_SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /*发送写地址的中位*/
    FONT_SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    /*发送写地址的低位*/
    FONT_SPI_FLASH_SendByte(WriteAddr & 0xFF);
    //写一个字节
    FONT_SPI_FLASH_SendByte(ch);

    FONT_SPI_FLASH_CS_HIGH();
    FONT_SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
 * @param	pBuffer，要写入数据的指针
 * @param WriteAddr，写入地址
 * @param  NumByteToWrite，写入数据长度，必须小于等于FONT_SPI_FLASH_PerWritePageSize
 * @retval 无
 */
void FONT_SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    /* 发送FLASH写使能命令 */
    FONT_SPI_FLASH_WriteEnable();
    //FONT_SPI_FLASH_WaitForWriteEnd();
    /* 选择FLASH: CS低电平 */
    FONT_SPI_FLASH_CS_LOW();

    /* 写页写指令*/
    FONT_SPI_FLASH_SendByte(JLX128_SequentialByteProgram);
    //FONT_SPI_FLASH_SendByte(JLX128_ByteProgram);
    /*发送写地址的高位*/
    FONT_SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /*发送写地址的中位*/
    FONT_SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    /*发送写地址的低位*/
    FONT_SPI_FLASH_SendByte(WriteAddr & 0xFF);
    //写第一个字节
    FONT_SPI_FLASH_SendByte(*pBuffer);

    pBuffer++;
    //要写字节数据的个数减一
    NumByteToWrite--;

    /* 停止信号 FLASH: CS 高电平 */
    FONT_SPI_FLASH_CS_HIGH();

    FONT_SPI_FLASH_WaitForWriteEnd();

    if (NumByteToWrite > FONT_SPI_FLASH_PerWritePageSize)
    {
        NumByteToWrite = FONT_SPI_FLASH_PerWritePageSize;
        FONT_FLASH_ERROR("FONT_SPI_FLASH_PageWrite too large!");
    }

    /* 写入数据*/
    while (NumByteToWrite--)
    {

        FONT_SPI_FLASH_CS_LOW();
        /* Send "sequtial Write to Memory " instruction */
        FONT_SPI_FLASH_SendByte(JLX128_SequentialByteProgram);
        /* Send the current byte */
        FONT_SPI_FLASH_SendByte(*pBuffer);
        /* 指向下一字节数据 */
        /* Point on the next byte to be written */
        pBuffer++;
        FONT_SPI_FLASH_CS_HIGH();
        FONT_SPI_FLASH_WaitForWriteEnd();
    }

    /* 停止信号 FLASH: CS 高电平 */
    FONT_SPI_FLASH_CS_HIGH();

    /* 等待写入完毕*/
    FONT_SPI_FLASH_WaitForWriteEnd();
    /* 关闭写使能 */
    FONT_SPI_FLASH_WriteDisnable();      // 必须加上此语句
}

/**
 * @brief  对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
 * @param	pBuffer，要写入数据的指针
 * @param  WriteAddr，写入地址
 * @param  NumByteToWrite，写入数据长度
 * @retval 无
 */
void FONT_SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    /*mod运算求余，若writeAddr是FONT_SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
    Addr = WriteAddr % FONT_SPI_FLASH_PageSize;

    /*差count个数据值，刚好可以对齐到页地址*/
    count = FONT_SPI_FLASH_PageSize - Addr;
    /*计算出要写多少整数页*/
    NumOfPage =  NumByteToWrite / FONT_SPI_FLASH_PageSize;
    /*mod运算求余，计算出剩余不满一页的字节数*/
    NumOfSingle = NumByteToWrite % FONT_SPI_FLASH_PageSize;

    /* Addr=0,则WriteAddr 刚好按页对齐 aligned  */
    if (Addr == 0)
    {
        /* NumByteToWrite < FONT_SPI_FLASH_PageSize */
        if (NumOfPage == 0)
        {
            FONT_SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        }
        else /* NumByteToWrite > FONT_SPI_FLASH_PageSize */
        {
            /*先把整数页都写了*/
            while (NumOfPage--)
            {
                FONT_SPI_FLASH_PageWrite(pBuffer, WriteAddr, FONT_SPI_FLASH_PageSize);
                WriteAddr +=  FONT_SPI_FLASH_PageSize;
                pBuffer += FONT_SPI_FLASH_PageSize;
            }
            /*若有多余的不满一页的数据，把它写完*/
            FONT_SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    /* 若地址与 FONT_SPI_FLASH_PageSize 不对齐  */
    else
    {
        /* NumByteToWrite < FONT_SPI_FLASH_PageSize */
        if (NumOfPage == 0)
        {
            /*当前页剩余的count个位置比NumOfSingle小，一页写不完*/
            if (NumOfSingle > count)
            {
                temp = NumOfSingle - count;
                /*先写满当前页*/
                FONT_SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);

                WriteAddr +=  count;
                pBuffer += count;
                /*再写剩余的数据*/
                FONT_SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
            }
            else /*当前页剩余的count个位置能写完NumOfSingle个数据*/
            {
                FONT_SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else /* NumByteToWrite > FONT_SPI_FLASH_PageSize */
        {
            /*地址不对齐多出的count分开处理，不加入这个运算*/
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / FONT_SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % FONT_SPI_FLASH_PageSize;

            /* 先写完count个数据，为的是让下一次要写的地址对齐 */
            FONT_SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);

            /* 接下来就重复地址对齐的情况 */
            WriteAddr +=  count;
            pBuffer += count;
            /*把整数页都写了*/
            while (NumOfPage--)
            {
                FONT_SPI_FLASH_PageWrite(pBuffer, WriteAddr, FONT_SPI_FLASH_PageSize);
                WriteAddr +=  FONT_SPI_FLASH_PageSize;
                pBuffer += FONT_SPI_FLASH_PageSize;
            }
            /*若有多余的不满一页的数据，把它写完*/
            if (NumOfSingle != 0)
            {
                FONT_SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/**
 * @brief  读取FLASH数据
 * @param 	pBuffer，存储读出数据的指针
 * @param   ReadAddr，读取地址
 * @param   NumByteToRead，读取数据长度
 * @retval 无
 */
void FONT_SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
    /* 选择FLASH: CS低电平 */
    FONT_SPI_FLASH_CS_LOW();

    /* 发送 读 指令 */
    FONT_SPI_FLASH_SendByte(JLX128_FastReadData);
    /* 发送 读 地址高位 */
    FONT_SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* 发送 读 地址中位 */
    FONT_SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* 发送 读 地址低位 */
    FONT_SPI_FLASH_SendByte(ReadAddr & 0xFF);

    FONT_SPI_FLASH_SendByte(Dummy_Byte);

    /* 读取数据 */
    while (NumByteToRead--) /* while there is data to be read */
    {
        /* 读取一个字节*/
        *pBuffer = FONT_SPI_FLASH_SendByte(Dummy_Byte);
        /* 指向下一个字节缓冲区 */
        pBuffer++;
    }

    /* 停止信号 FLASH: CS 高电平 */
    FONT_SPI_FLASH_CS_HIGH();
}

/**
 * @brief  读取FLASH ID
 * @param 	无
 * @retval FLASH ID
 */
u32 FONT_SPI_FLASH_ReadID(void)
{
    u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

    /* 开始通讯：CS低电平 */
    FONT_SPI_FLASH_CS_LOW();

    /* 发送JEDEC指令，读取ID */
    FONT_SPI_FLASH_SendByte(JLX128_DeviceID);

    /* 读取一个字节数据 */
    Temp0 = FONT_SPI_FLASH_SendByte(Dummy_Byte);

    /* 读取一个字节数据 */
    Temp1 = FONT_SPI_FLASH_SendByte(Dummy_Byte);

    /* 读取一个字节数据 */
    Temp2 = FONT_SPI_FLASH_SendByte(Dummy_Byte);

    /* 停止通讯：CS高电平 */
    FONT_SPI_FLASH_CS_HIGH();

    /*把数据组合起来，作为函数的返回值*/
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

    return Temp;
}
/**
 * @brief  读取FLASH Device ID
 * @param 	无
 * @retval FLASH Device ID
 */
u32 FONT_SPI_FLASH_ReadDeviceID(void)
{
    u32 Temp = 0;

    /* Select the FLASH: Chip Select low */
    FONT_SPI_FLASH_CS_LOW();

    /* Send "RDID " instruction */
    FONT_SPI_FLASH_SendByte(JLX128_DeviceID);
    FONT_SPI_FLASH_SendByte(Dummy_Byte);
    FONT_SPI_FLASH_SendByte(Dummy_Byte);
    FONT_SPI_FLASH_SendByte(Dummy_Byte);

    /* Read a byte from the FLASH */
    Temp = FONT_SPI_FLASH_SendByte(Dummy_Byte);

    /* Deselect the FLASH: Chip Select high */
    FONT_SPI_FLASH_CS_HIGH();

    return Temp;
}
/*******************************************************************************
* Function Name  : FONT_SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void FONT_SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
    /* Select the FLASH: Chip Select low */
    FONT_SPI_FLASH_CS_LOW();

    /* Send "Read from Memory " instruction */
    FONT_SPI_FLASH_SendByte(JLX128_ReadData);

    /* Send the 24-bit address of the address to read from -----------------------*/
    /* Send ReadAddr high nibble address byte */
    FONT_SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte */
    FONT_SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte */
    FONT_SPI_FLASH_SendByte(ReadAddr & 0xFF);
}


/**
 * @brief  使用SPI读取一个字节的数据
 * @param  无
 * @retval 返回接收到的数据
 */
u8 FONT_SPI_FLASH_ReadByte(void)
{
    return (FONT_SPI_FLASH_SendByte(Dummy_Byte));
}

/**
 * @brief  使用SPI发送一个字节的数据
 * @param  byte：要发送的数据
 * @retval 返回接收到的数据
 */
u8 FONT_SPI_FLASH_SendByte(u8 byte)
{
    SPITimeout = SPIT_FLAG_TIMEOUT;
    /* 等待发送缓冲区为空，TXE事件 */
    while (SPI_I2S_GetFlagStatus(FONT_FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)
    {
        if ((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
    }

    /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
    SPI_I2S_SendData(FONT_FLASH_SPIx , byte);

    SPITimeout = SPIT_FLAG_TIMEOUT;
    /* 等待接收缓冲区非空，RXNE事件 */
    while (SPI_I2S_GetFlagStatus(FONT_FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
    {
        if ((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
    }

    /* 读取数据寄存器，获取接收缓冲区数据 */
    return SPI_I2S_ReceiveData(FONT_FLASH_SPIx );
}

/**
 * @brief  使用SPI发送两个字节的数据
 * @param  byte：要发送的数据
 * @retval 返回接收到的数据
 */
u16 FONT_SPI_FLASH_SendHalfWord(u16 HalfWord)
{
    SPITimeout = SPIT_FLAG_TIMEOUT;
    /* 等待发送缓冲区为空，TXE事件 */
    while (SPI_I2S_GetFlagStatus(FONT_FLASH_SPIx , SPI_I2S_FLAG_TXE) == RESET)
    {
        if ((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
    }

    /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
    SPI_I2S_SendData(FONT_FLASH_SPIx , HalfWord);

    SPITimeout = SPIT_FLAG_TIMEOUT;
    /* 等待接收缓冲区非空，RXNE事件 */
    while (SPI_I2S_GetFlagStatus(FONT_FLASH_SPIx , SPI_I2S_FLAG_RXNE) == RESET)
    {
        if ((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
    }
    /* 读取数据寄存器，获取接收缓冲区数据 */
    return SPI_I2S_ReceiveData(FONT_FLASH_SPIx );
}

/**
 * @brief  向FLASH发送 写使能 命令
 * @param  none
 * @retval none
 */
void FONT_SPI_FLASH_WriteEnable(void)
{
    /* 通讯开始：CS低 */
    FONT_SPI_FLASH_CS_LOW();

    /* 发送写使能命令*/
    FONT_SPI_FLASH_SendByte(JLX128_WriteEnable);

    /*通讯结束：CS高 */
    FONT_SPI_FLASH_CS_HIGH();
}

/**
 * @brief  向FLASH发送 不可写使能 命令
 * @param  none
 * @retval none
 */
void FONT_SPI_FLASH_WriteDisnable(void)
{
    /* 通讯开始：CS低 */
    FONT_SPI_FLASH_CS_LOW();

    /* 发送写使能命令*/
    FONT_SPI_FLASH_SendByte(JLX128_WriteDisable);

    /*通讯结束：CS高 */
    FONT_SPI_FLASH_CS_HIGH();
}
/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  0x01

/**
 * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
 * @param  none
 * @retval none
 */
void FONT_SPI_FLASH_WaitForWriteEnd(void)
{
    u8 FLASH_Status = 0;

    /* 选择 FLASH: CS 低 */
    FONT_SPI_FLASH_CS_LOW();

    /* 发送 读状态寄存器 命令 */
    FONT_SPI_FLASH_SendByte(JLX128_ReadStatusReg);

    /* 若FLASH忙碌，则等待 */
    do
    {
        /* 读取FLASH芯片的状态寄存器 */
        FLASH_Status = FONT_SPI_FLASH_SendByte(Dummy_Byte);
    }
    while ((FLASH_Status & WIP_Flag) == SET);  /* 正在写入标志 */

    /* 停止信号  FLASH: CS 高 */
    FONT_SPI_FLASH_CS_HIGH();
}


//进入掉电模式
void FONT_SPI_FLASH_PowerDown(void)
{
    /* 通讯开始：CS低 */
    FONT_SPI_FLASH_CS_LOW();

    /* 发送 掉电 命令 */
    FONT_SPI_FLASH_SendByte(JLX128_PowerDown);

    /*通讯结束：CS高 */
    FONT_SPI_FLASH_CS_HIGH();
}

//唤醒
void FONT_SPI_FLASH_WAKEUP(void)
{
    /*选择 FLASH: CS 低 */
    FONT_SPI_FLASH_CS_LOW();

    /* 发送 上电 命令 */
    FONT_SPI_FLASH_SendByte(JLX128_ReleasePowerDown);

    /* 停止信号 FLASH: CS 高 */
    FONT_SPI_FLASH_CS_HIGH();
}


/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* 等待超时后的处理,输出错误信息 */
    FONT_FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
    return 0;
}

/*********************************************END OF FILE**********************/

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



/******************************************************************/
void display_GB2312_string(u8 x, u8 y, unsigned char *text, unsigned char isTurn)
{
    u8 i = 0;
    u8 fontbuf[32] = {0};
    u32 fontaddr = 0;
    while ((text[i] > 0x00))
    {
        if ( ( ( text[i] >= 0xb0 ) && ( text[i] <= 0xf7 ) ) && ( text[i+1] >= 0xa1 ) )
        {
            /*国标简体（GB2312）汉字在晶联讯字库 IC 中的地址由以下公式来计算：*/
            /*Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32 + BaseAdd;BaseAdd=0*/
            fontaddr = ( ( text[i] - 0xb0 ) * 94 + ( text[i + 1] - 0xa1 ) + 846 ) * 32;
            //get_n_bytes_data_from_ROM(addrHigh,addrMid,addrLow,fontbuf,32 );      /*取 32 个字节的数据，存到"fontbuf[32]"*/
            FONT_SPI_FLASH_BufferRead(fontbuf, fontaddr, sizeof (fontbuf) );        /*取 32 个字节的数据，存到"fontbuf[32]"*/
            //display_graphic_16x16(y,x,fontbuf);                                   /*显示汉字到 LCD 上，y 为页地址，x 为列地址，fontbuf[]为数据*/
            OLED_xShowCN(x, y, fontbuf, isTurn);
            i += 2;
            x += 16;
        }
        else if ( ( ( text[i] >= 0xa1 ) && ( text[i] <= 0xa3 ) ) && ( text[i+1] >= 0xa1 ) )
        {
            /*国标简体（GB2312）15x16 点的字符在晶联讯字库 IC 中的地址由以下公式来计算：*/
            /*Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0*/
            fontaddr = (( text[i]- 0xa1 ) * 94 + ( text[i + 1] - 0xa1)) * 32;
            FONT_SPI_FLASH_BufferRead(fontbuf, fontaddr, sizeof (fontbuf) );        /*取 32 个字节的数据，存到"fontbuf[32]"*/
            OLED_xShowCN(x, y, fontbuf, isTurn);
            i += 2;
            x += 16;
        }
        else if ((text[i]>=0x20) &&(text[i]<=0x7e))
        {
            unsigned char fontbuf[16];
            fontaddr = (( text[i] - 0x20) * 16) + 0x3cf80;             //0x3b7c0
            FONT_SPI_FLASH_BufferRead( fontbuf, fontaddr, 16 );        /*取 16 个字节的数据，存到"fontbuf[32]"*/
            OLED_xShowEN(x, y, fontbuf , isTurn);
            i += 1;
            x += 8;
        }
        else
            i++;
    }
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

/**
  ******************************************************************************
  * @file    OLED_I2C.c
  * @author  fire
  * @version V1.0
  * @date    2014-xx-xx
  * @brief   128*64点阵的OLED显示屏驱动文件，仅适用于SD1306驱动IIC通信方式显示屏
  ******************************************************************************
  * @attention
  *
  * 实验平台:
  * 论坛
  * 淘宝

    * Function List:
    *    1. void I2C_Configuration(void) -- 配置CPU的硬件I2C
    * 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- 向寄存器地址写一个byte的数据
    * 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
    * 4. void WriteDat(unsigned char I2C_Data) -- 写数据
    * 5. void OLED_Init(void) -- OLED屏初始化
    * 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
    * 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
    * 8. void OLED_CLS(void) -- 清屏
    * 9. void OLED_ON(void) -- 唤醒
    * 10. void OLED_OFF(void) -- 睡眠
    * 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
    * 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- 显示中文(中文需要先取模，然后放到codetab.h中)
    * 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
    *
  *
  ******************************************************************************
  */



#include <includes.h>
#include "bsp_LCD_I2C.h"
#include "delay.h"
#include "codetab.h"


 /**
  * @brief  I2C_Configuration，初始化硬件IIC引脚
  * @param  无
  * @retval 无
  */
void I2C_Configuration(void)
{
    I2C_InitTypeDef  I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    /* 配置 RESET 引脚 */
    GPIO_InitStructure.GPIO_Pin = macOLED_RESET_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init ( macOLED_RESET_GPIO_PORT, & GPIO_InitStructure );

    /*STM32F103RET6芯片的硬件I2C: PB6 -- SCL; PB7 -- SDA */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_DeInit(I2C1);//使用I2C1
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x30;//主机的I2C地址,随便写的
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

    I2C_Cmd(I2C1, ENABLE);
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_AcknowledgeConfig(I2C1, ENABLE);
}


 /**
  * @brief  I2C_WriteByte，向OLED寄存器地址写一个byte的数据
  * @param  addr：寄存器地址
    *                    data：要写入的数据
  * @retval 无
  */
void I2C_WriteByte(uint8_t addr,uint8_t data)
{
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

    I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, addr);//寄存器地址
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_SendData(I2C1, data);//发送数据
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
}


 /**
  * @brief  WriteCmd，向OLED写入命令
  * @param  I2C_Command：命令代码
  * @retval 无
  */
void WriteCmd(unsigned char I2C_Command)//写命令
{
    I2C_WriteByte(0x00, I2C_Command);
}


 /**
  * @brief  WriteDat，向OLED写入数据
  * @param  I2C_Data：数据
  * @retval 无
  */
void WriteDat(unsigned char I2C_Data)//写数据
{
    I2C_WriteByte(0x40, I2C_Data);
}


 /**
  * @brief  OLED_Init，初始化OLED
  * @param  无
  * @retval 无
  */
void OLED_Init(void)
{
    OS_ERR      err;
    macOLED_RESET_ON();
    macOLED_RESET_OFF();

    WriteCmd(0x3b); //
    WriteCmd(0x03); //

    WriteCmd(0x38); //
    WriteCmd(0x05); //
    WriteCmd(0x0c); //

    WriteCmd(0x39); //
    WriteCmd(0x08); //
    WriteCmd(0x10); //
    WriteCmd(0xff); //
}


 /**
  * @brief  OLED_SetPos，设置光标
  * @param  x,光标x位置
    *                    y，光标y位置
  * @retval 无
  */
void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{
    WriteCmd(0x38);
    WriteCmd(0x40 + y);
    x = x + 4;
    WriteCmd(( x & 0x0f ) | 0xe0 );
    WriteCmd((( x & 0xf0 ) >> 4 )| 0xf0 );

}

 /**
  * @brief  OLED_Fill，填充整个屏幕
  * @param  fill_Data:要填充的数据
    * @retval 无
  */
void OLED_Fill(unsigned char fill_Data)//全屏填充
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        WriteCmd(0x40+m);        //page0-page1
        WriteCmd(0xe0 | 0x03);        //low column start address
        WriteCmd(0xf0);        //high column start address
        for(n=0;n<129;n++)
            {
                WriteDat(fill_Data);
            }
    }
}

 /**
  * @brief  OLED_CLS，清屏
  * @param  无
    * @retval 无
  */
void OLED_CLS(void)//清屏
{
    OLED_Fill(0x00);
}


 /**
  * @brief  OLED_ON，将OLED从休眠中唤醒
  * @param  无
    * @retval 无
  */
void OLED_ON(void)
{
    WriteCmd(0X8D);  //设置电荷泵
    WriteCmd(0X14);  //开启电荷泵
    WriteCmd(0XAF);  //OLED唤醒
}


 /**
  * @brief  OLED_OFF，让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
  * @param  无
    * @retval 无
  */
void OLED_OFF(void)
{
    WriteCmd(0X8D);  //设置电荷泵
    WriteCmd(0X10);  //关闭电荷泵
    WriteCmd(0XAE);  //OLED休眠
}

 /**
  * @brief  OLED_ShowStr，显示codetab.h中的ASCII字符,有6*8和8*16可选择
  * @param  x,y : 起始点坐标(x:0~127, y:0~7);
    *                    ch[] :- 要显示的字符串;
    *                    TextSize : 字符大小(1:6*8 ; 2:8*16)
    * @retval 无
  */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                 //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                 //，开中断时将该值还原.
#ifdef OLED 
    unsigned char c = 0,i = 0,j = 0;
    OLED_Fill(0);
    switch(TextSize)
    {
        case 1:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 126)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<6;i++)
                    WriteDat(F6x8[c][i]);
                x += 6;
                j++;
            }
        }break;
        case 2:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 120)
                {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x,y);
                for(i=0;i<8;i++)
                    WriteDat(F8X16[c*16+i]);
                OLED_SetPos(x,y+1);
                for(i=0;i<8;i++)
                    WriteDat(F8X16[c*16+i+8]);
                x += 8;
                j++;
            }
        }break;
    }
#else
    OS_CRITICAL_ENTER();                 //进入临界段，不希望下面串口打印遭到中断
    printf ("%s",ch);
    OS_CRITICAL_EXIT();
#endif
}

 /**
  * @brief  OLED_ShowCN，显示codetab.h中的汉字,16*16点阵
  * @param  x,y: 起始点坐标(x:0~127, y:0~7);
    *                    N:汉字在codetab.h中的索引
    * @retval 无
  */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
    unsigned char wm=0;
    unsigned int  adder=32*N;
    OLED_SetPos(x , y);
    for(wm = 0;wm < 16;wm++)
    {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x,y + 1);
    for(wm = 0;wm < 16;wm++)
    {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
}



 /**
  * @brief  OLED_DrawBMP，显示BMP位图
  * @param  x0,y0 :起始点坐标(x0:0~127, y0:0~7);
    *                    x1,y1 : 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
    * @retval 无
  */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

    if(y1%8==0)
        y = y1/8;
    else
        y = y1/8 + 1;
    for(y=y0;y<y1;y++)
    {
        OLED_SetPos(x0,y);
        for(x=x0;x<x1;x++)
            {
                WriteDat(BMP[j++]);
            }
    }
}

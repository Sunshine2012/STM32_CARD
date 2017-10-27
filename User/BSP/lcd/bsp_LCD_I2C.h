#ifndef __OLED_I2C_H
#define __OLED_I2C_H

#include "stm32f10x.h"

#define OLED_ADDRESS    0x7e // 屏的默认地址0x7e




#define             macOLED_RESET_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macOLED_RESET_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             macOLED_RESET_GPIO_PORT                       GPIOB
#define             macOLED_RESET_GPIO_PIN                        GPIO_Pin_5



/****************************** LED 函数宏定义***************************************/
#define             macOLED_RESET_ON()                            GPIO_ResetBits ( macOLED_RESET_GPIO_PORT, macOLED_RESET_GPIO_PIN )
#define             macOLED_RESET_OFF()                           GPIO_SetBits ( macOLED_RESET_GPIO_PORT, macOLED_RESET_GPIO_PIN )
#define             macOLED_RESET_TOGGLE()                        GPIO_ReadOutputDataBit ( macOLED_RESET_GPIO_PORT, macLED1_GPIO_PIN ) ? \
                                                                  GPIO_ResetBits ( macOLED_RESET_GPIO_PORT, macOLED_RESET_GPIO_PIN ) : GPIO_SetBits ( macOLED_RESET_GPIO_PORT, macOLED_RESET_GPIO_PIN )

#define LCD_BKLIGHT_LOW()              GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define LCD_BKLIGHT_HIGH()             GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define LCD_RESET_LOW()                GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define LCD_RESET_HIGH()               GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define LCD_SCLK_LOW()                 GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define LCD_SCLK_HIGH()                GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define LCD_SDA_LOW()                  GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define LCD_SDA_HIGH()                 GPIO_SetBits(GPIOB, GPIO_Pin_7)
void LCD_GPIO_Config ( void );
void initial_lcd ();
void delay ( int i );
void transfer ( int data1 );
void start_flag ();
void stop_flag ();

void I2C_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N, unsigned char isTurn);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void OLED_xShowCN(unsigned char x, unsigned char y, unsigned char * pFontbuf, unsigned char isTurn);
void OLED_xShowEN(unsigned char x, unsigned char y, unsigned char * pFontbuf, unsigned char isTurn);


static uint16_t IIC_TIMEOUT_UserCallback(uint8_t errorCode);

#endif

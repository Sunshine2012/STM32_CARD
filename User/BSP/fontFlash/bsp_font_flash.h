#ifndef __FONT_SPI_FLASH_H
#define __FONT_SPI_FLASH_H

#include "stm32f10x.h"
#include <stdio.h>

//#define  sFLASH_ID                    0xEF3015    //AT26XX16
//#define  sFLASH_ID                    0xEF4015    //W25Q16
//#define  sFLASH_ID                    0XEF4018    //W25Q128
//#define  sFLASH_ID                      0XEF4017    //W25Q64
#define  sFLASH_ID                      0x1F0400    //AT26F004

#define FONT_SPI_FLASH_PageSize              256
#define FONT_SPI_FLASH_PerWritePageSize      256

/*命令定义-开头*******************************/

#define JLX128_FastReadData             0x0B
#define JLX128_ReadData                 0x03

#define JLX128_BlockErase4K             0x20
#define JLX128_BlockErase32K            0x52
#define JLX128_BlockErase64K            0xD8
#define JLX128_ChipEraseFirst           0x60
#define JLX128_ChipEraseSecend          0xc7

#define JLX128_ByteProgram              0x02
#define JLX128_SequentialByteProgram    0xAF

#define JLX128_WriteEnable              0x06
#define JLX128_WriteDisable             0x04
#define JLX128_ProtectSector            0x36
#define JLX128_UnprotectSector          0x39
#define JLX128_ReadSectorProtection     0x3c

#define JLX128_ReadStatusReg            0x05
#define JLX128_WriteStatusReg           0x01

#define JLX128_DeviceID                 0x9F
#define JLX128_PowerDown                0xB9
#define JLX128_ReleasePowerDown         0xAB

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_Flag                  0x01
#define Dummy_Byte                0xFF
/*命令定义-结尾*******************************/


/*SPI接口定义-开头****************************/
#define      FONT_FLASH_SPIx                         SPI2
#define      FONT_FLASH_SPI_APBxClock_FUN            RCC_APB1PeriphClockCmd
#define      FONT_FLASH_SPI_CLK                      RCC_APB1Periph_SPI2

//CS(NSS)引脚 片选选普通GPIO即可
#define      FONT_FLASH_SPI_CS_APBxClock_FUN         RCC_APB2PeriphClockCmd
#define      FONT_FLASH_SPI_CS_CLK                   RCC_APB2Periph_GPIOB
#define      FONT_FLASH_SPI_CS_PORT                  GPIOB
#define      FONT_FLASH_SPI_CS_PIN                   GPIO_Pin_12

//SCK引脚
#define      FONT_FLASH_SPI_SCK_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define      FONT_FLASH_SPI_SCK_CLK                  RCC_APB2Periph_GPIOB
#define      FONT_FLASH_SPI_SCK_PORT                 GPIOB
#define      FONT_FLASH_SPI_SCK_PIN                  GPIO_Pin_13
//MISO引脚
#define      FONT_FLASH_SPI_MISO_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      FONT_FLASH_SPI_MISO_CLK                 RCC_APB2Periph_GPIOB
#define      FONT_FLASH_SPI_MISO_PORT                GPIOB
#define      FONT_FLASH_SPI_MISO_PIN                 GPIO_Pin_14
//MOSI引脚
#define      FONT_FLASH_SPI_MOSI_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      FONT_FLASH_SPI_MOSI_CLK                 RCC_APB2Periph_GPIOB
#define      FONT_FLASH_SPI_MOSI_PORT                GPIOB
#define      FONT_FLASH_SPI_MOSI_PIN                 GPIO_Pin_15

#define         FONT_SPI_FLASH_CS_LOW()                             GPIO_ResetBits( FONT_FLASH_SPI_CS_PORT, FONT_FLASH_SPI_CS_PIN )
#define         FONT_SPI_FLASH_CS_HIGH()                            GPIO_SetBits( FONT_FLASH_SPI_CS_PORT, FONT_FLASH_SPI_CS_PIN )

/*SPI接口定义-结尾****************************/

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))

/*信息输出*/
#define FLASH_DEBUG_ON         1

#define FONT_FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FONT_FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FONT_FLASH_DEBUG(fmt,arg...)          do{\
                                            if(FLASH_DEBUG_ON)\
                                            printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                            }while(0)

void FONT_SPI_FLASH_Init(void);

void FONT_SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);


u8 FONT_SPI_FLASH_ReadByte(void);
u8 FONT_SPI_FLASH_SendByte(u8 byte);
void display_GB2312_string(u8 x, u8 y, unsigned char *text, unsigned char isTurn);


#endif /* __FONT_SPI_FLASH_H */


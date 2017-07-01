#ifndef __USART4_H
#define	__USART4_H


#include "stm32f10x.h"
#include <stdio.h>

/** 
  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
  */
	
// 串口1-USART1
//#define  DEBUG_USARTx                       USART1
//#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
//#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT         GPIOA   
//#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_9
//#define  DEBUG_USART_RX_GPIO_PORT         GPIOA
//#define  DEBUG_USART_RX_GPIO_PIN          GPIO_Pin_10

//#define  DEBUG_USART_IRQ                  USART1_IRQn
//#define  DEBUG_USART_IRQHandler           USART1_IRQHandler


// 串口2-USART2
//#define  DEBUG_USARTx                   USART2
//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART2
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT         GPIOA   
//#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_2
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_3

//#define  DEBUG_USART_IRQ                USART2_IRQn
//#define  DEBUG_USART_IRQHandler         USART2_IRQHandler

//// 串口3-USART3
//#define  DEBUG_USARTx                   USART3
//#define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT         GPIOB   
//#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_10
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOB
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

//#define  DEBUG_USART_IRQ                USART3_IRQn
//#define  DEBUG_USART_IRQHandler         USART3_IRQHandler

////// 串口4-UART4
////#define  DEBUG_USARTx                   UART4
////#define  DEBUG_USART_CLK                RCC_APB1Periph_UART4
////#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
////#define  DEBUG_USART_BAUDRATE           115200

////// USART GPIO 引脚宏定义
////#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
////#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
////    
////#define  DEBUG_USART_TX_GPIO_PORT       GPIOC   
////#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_10
////#define  DEBUG_USART_RX_GPIO_PORT       GPIOC
////#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

////#define  DEBUG_USART_IRQ                UART4_IRQn
////#define  DEBUG_USART_IRQHandler         UART4_IRQHandler


/**************************USART参数定义********************************/
#define             mac4USART_BAUD_RATE                       9600

#define             mac4USART                                 UART4
#define             mac4USART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             mac4USART_CLK                             RCC_APB1Periph_UART4
#define             mac4USART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             mac4USART_GPIO_CLK                        RCC_APB2Periph_GPIOC
#define             mac4USART_TX_PORT                         GPIOC
#define             mac4USART_TX_PIN                          GPIO_Pin_10
#define             mac4USART_RX_PORT                         GPIOC
#define             mac4USART_RX_PIN                          GPIO_Pin_11
#define             mac4USART_IRQ                             UART4_IRQn
#define             mac4USART_IRQHandler                      UART4_IRQHandler

// 串口5-UART5
//#define  DEBUG_USARTx                   UART5
//#define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
//#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT         GPIOC   
//#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_12
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOD
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_2

//#define  DEBUG_USART_IRQ                UART5_IRQn
//#define  DEBUG_USART_IRQHandler         UART5_IRQHandler


void USART4_Config(void);
void USART4_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void USART4_nSendString( USART_TypeDef * pUSARTx, char *str,int n);
void USART4_SendString( USART_TypeDef * pUSARTx, char *str);
void USART4_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __USART_H */

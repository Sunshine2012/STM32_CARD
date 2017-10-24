#ifndef __USART1_H
#define __USART1_H


#include "stm32f10x.h"
#include <stdio.h>



#if 1

/**************************USART参数定义********************************/
#define             macUSART1_BAUD_RATE                       9600

#define             macUSART1                                 USART1
#define             macUSART1_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define             macUSART1_CLK                             RCC_APB2Periph_USART1
#define             macUSART1_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART1_GPIO_CLK                        RCC_APB2Periph_GPIOA
#define             macUSART1_TX_PORT                         GPIOA
#define             macUSART1_TX_PIN                          GPIO_Pin_9
#define             macUSART1_RX_PORT                         GPIOA
#define             macUSART1_RX_PIN                          GPIO_Pin_10
#define             macUSART1_IRQ                             USART1_IRQn
#define             macUSART1_IRQHandler                      USART1_IRQHandler

#else

/**************************USART参数定义*******************************
#define             macUSART4_BAUD_RATE                       9600

#define             macUSART4                                 UART4
#define             macUSART4_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macUSART4_CLK                             RCC_APB1Periph_UART4
#define             macUSART4_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART4_GPIO_CLK                        RCC_APB2Periph_GPIOC
#define             macUSART4_TX_PORT                         GPIOC
#define             macUSART4_TX_PIN                          GPIO_Pin_10
#define             macUSART4_RX_PORT                         GPIOC
#define             macUSART4_RX_PIN                          GPIO_Pin_11
#define             macUSART4_IRQ                             USART4_IRQn
#define             macUSART4_INT_FUN                         USART4_IRQHandler
*/
#endif

void USART1_Config(void);
void USART1_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void USART1_nSendString( USART_TypeDef * pUSARTx, char *str,int n);
void USART1_SendString( USART_TypeDef * pUSARTx, char *str);
void USART1_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif /* __USART_H */

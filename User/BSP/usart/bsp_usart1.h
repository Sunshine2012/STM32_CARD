#ifndef __USART1_H
#define	__USART1_H


#include "stm32f10x.h"
#include <stdio.h>



#if 0

/**************************USART参数定义********************************/
#define             macUSART_BAUD_RATE                       9600

#define             macUSARTx                                USART1
#define             macUSART_APBxClock_FUN                   RCC_APB2PeriphClockCmd
#define             macUSART_CLK                             RCC_APB2Periph_USART1
#define             macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART_GPIO_CLK                        RCC_APB2Periph_GPIOA     
#define             macUSART_TX_PORT                         GPIOA   
#define             macUSART_TX_PIN                          GPIO_Pin_9
#define             macUSART_RX_PORT                         GPIOA 
#define             macUSART_RX_PIN                          GPIO_Pin_10
#define             macUSART_IRQ                             USART1_IRQn
#define             macUSART_INT_FUN                         USART1_IRQHandler

#else

/**************************USART参数定义********************************/
#define             macUSART_BAUD_RATE                       9600

#define             macUSARTx                                UART4
#define             macUSART_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             macUSART_CLK                             RCC_APB1Periph_UART4
#define             macUSART_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macUSART_GPIO_CLK                        RCC_APB2Periph_GPIOC
#define             macUSART_TX_PORT                         GPIOC
#define             macUSART_TX_PIN                          GPIO_Pin_10
#define             macUSART_RX_PORT                         GPIOC
#define             macUSART_RX_PIN                          GPIO_Pin_11
#define             macUSART_IRQ                             USART4_IRQn
#define             macUSART_INT_FUN                         USART4_IRQHandler

#endif




void                USARTx_Config                           ( void );



#endif /* __USART1_H */

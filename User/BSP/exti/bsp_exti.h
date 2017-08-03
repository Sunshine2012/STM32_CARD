#ifndef  __EXTI_H
#define  __EXTI_H

#include "stm32f10x.h"

//Òý½Å¶¨Òå
#define NVIC0_INT_GPIO_PORT         GPIOB
#define NVIC0_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define NVIC0_INT_GPIO_PIN          GPIO_Pin_0
#define NVIC0_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define NVIC0_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define NVIC0_INT_EXTI_LINE         EXTI_Line0
#define NVIC0_INT_EXTI_IRQ          EXTI0_IRQn

#define NVIC0_IRQHandler            EXTI0_IRQHandler


#define NVIC1_INT_GPIO_PORT         GPIOB
#define NVIC1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define NVIC1_INT_GPIO_PIN          GPIO_Pin_1
#define NVIC1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define NVIC1_INT_EXTI_PINSOURCE    GPIO_PinSource1
#define NVIC1_INT_EXTI_LINE         EXTI_Line1
#define NVIC1_INT_EXTI_IRQ          EXTI1_IRQn

#define NVIC1_IRQHandler            EXTI1_IRQHandler

#define NVIC2_INT_GPIO_PORT         GPIOB
#define NVIC2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define NVIC2_INT_GPIO_PIN          GPIO_Pin_9
#define NVIC2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define NVIC2_INT_EXTI_PINSOURCE    GPIO_PinSource9
#define NVIC2_INT_EXTI_LINE         EXTI_Line9
#define NVIC2_INT_EXTI_IRQ          EXTI9_5_IRQn

#define NVIC2_IRQHandler            EXTI9_5_IRQHandler

#define NVIC3_INT_GPIO_PORT         GPIOB
#define NVIC3_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define NVIC3_INT_GPIO_PIN          GPIO_Pin_11
#define NVIC3_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define NVIC3_INT_EXTI_PINSOURCE    GPIO_PinSource11
#define NVIC3_INT_EXTI_LINE         EXTI_Line11
#define NVIC3_INT_EXTI_IRQ          EXTI15_10_IRQn

#define NVIC3_IRQHandler            EXTI15_10_IRQHandler


void EXTI_Config( void );

void NVIC0_IRQHandler( void );
void NVIC1_IRQHandler( void );
void NVIC2_IRQHandler( void );
void NVIC3_IRQHandler( void );

#endif /* __EXTI_H */

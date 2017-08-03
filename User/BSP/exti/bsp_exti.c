/**
  ******************************************************************************
  * @file    bsp_exti.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   I/O线中断应用bsp
  ******************************************************************************
  * @attention
  *
  * 实验平台:
  * 论坛    :
  * 淘宝    :
  *
  ******************************************************************************
  */

#include "includes.h"
#include "bsp_exti.h"

/**
 * @brief  配置嵌套向量中断控制器NVIC
 * @param  无
 * @retval 无
 */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置NVIC为优先级组1 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* 配置中断源1 */
    NVIC_InitStructure.NVIC_IRQChannel = NVIC0_INT_EXTI_IRQ;
    /* 配置抢占优先级 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    /* 配置子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    /* 使能中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 配置中断源2：其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = NVIC1_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);    
    
    /* 配置中断源3：其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = NVIC2_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 配置中断源4：其他使用上面相关配置 */
    NVIC_InitStructure.NVIC_IRQChannel = NVIC3_INT_EXTI_IRQ;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  配置 IO为EXTI中断口，并设置中断优先级
 * @param  无
 * @retval 无
 */
void EXTI_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /*开启按键GPIO口的时钟*/
    RCC_APB2PeriphClockCmd(NVIC0_INT_GPIO_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(NVIC1_INT_GPIO_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(NVIC2_INT_GPIO_CLK,ENABLE);
    RCC_APB2PeriphClockCmd(NVIC3_INT_GPIO_CLK,ENABLE);

    /* 配置 NVIC 中断*/
    NVIC_Configuration();

    /*--------------------------NVIC0配置-----------------------------*/
    /* 选择中断用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = NVIC0_INT_GPIO_PIN;
    /* 配置为浮空输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(NVIC0_INT_GPIO_PORT, &GPIO_InitStructure);

    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(NVIC0_INT_EXTI_PORTSOURCE, NVIC0_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = NVIC0_INT_EXTI_LINE;

    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------NVIC1配置-----------------------------*/
    /* 选择中断用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = NVIC1_INT_GPIO_PIN;
    /* 配置为浮空输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(NVIC1_INT_GPIO_PORT, &GPIO_InitStructure);

    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(NVIC1_INT_EXTI_PORTSOURCE, NVIC1_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = NVIC1_INT_EXTI_LINE;

    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    
    /*--------------------------NVIC2配置-----------------------------*/
    /* 选择中断用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = NVIC2_INT_GPIO_PIN;
    /* 配置为浮空输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(NVIC2_INT_GPIO_PORT, &GPIO_InitStructure);

    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(NVIC2_INT_EXTI_PORTSOURCE, NVIC2_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = NVIC2_INT_EXTI_LINE;

    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    
    /*--------------------------NVIC3配置-----------------------------*/
    /* 选择中断用到的GPIO */
    GPIO_InitStructure.GPIO_Pin = NVIC3_INT_GPIO_PIN;
    /* 配置为浮空输入 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(NVIC3_INT_GPIO_PORT, &GPIO_InitStructure);

    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(NVIC3_INT_EXTI_PORTSOURCE, NVIC3_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = NVIC3_INT_EXTI_LINE;

    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    
}
/*********************************************END OF FILE**********************/




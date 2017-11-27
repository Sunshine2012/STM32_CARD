/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI

  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <includes.h>
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

CPU_INT08U g_rx_buf[1024];    // 使用全局的数据缓存
CPU_INT16U g_num = 0;

// 串口中断服务函数
// 把接收到的数据存在一个数组缓冲区里面，当接收到的的值等于0XFF时，把值返回
void macUSART1_IRQHandler(void)
{
    OS_ERR      err;
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                             //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                             //，开中断时将该值还原。
    OS_CRITICAL_ENTER();                 // 进入临界段，不希望下面语句遭到中断
    if(USART_GetITStatus(macUSART1,USART_IT_RXNE)!=RESET)
    {
        g_rx_buf[g_num] = USART_ReceiveData(macUSART1);
        if(g_rx_buf[g_num] == FRAME_START)
        {
            memset(g_rx_buf, 0, sizeof (g_rx_buf));
            g_rx_buf[0] = FRAME_START;
            g_num = 0;
            g_num++;
        }
        else if( g_rx_buf[g_num] == FRAME_END)  // 当接收到的值等于'>'时，结束一帧数据
        {
            g_rx_buf[g_num + 1] = 0;
            /* 发布消息到消息队列 queue */
            OSQPost ((OS_Q        *)&queue_uart,                            //消息变量指针
                     (void        *)g_rx_buf,                               //要发送的数据的指针，将内存块首地址通过队列"发送出去"
                     (OS_MSG_SIZE  )strlen ( (const char*)g_rx_buf ),       //数据字节大小
                     (OS_OPT       )OS_OPT_POST_FIFO | OS_OPT_POST_ALL,     //先进先出和发布给全部任务的形式
                     (OS_ERR      *)&err);                                  //返回错误类型

        }
        // 当值不等时候，则继续接收下一个
        else
        {
            g_num++;
            if (g_num > 50) //一帧数据最大50字节,超出则丢弃
            {
                g_num = 0;
            }
        }
    }
    OS_CRITICAL_EXIT();
}
#if uart4
// 串口中断服务函数
// 把接收到的数据存在一个数组缓冲区里面，当接收到的的值等于0XFF时，把值返回
void macUSART4_IRQHandler(void)
{
    OS_ERR      err;
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                             //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                             //，开中断时将该值还原。
    OS_CRITICAL_ENTER();                 // 进入临界段，不希望下面语句遭到中断
    if(USART_GetITStatus(macUSART4,USART_IT_RXNE)!=RESET)
    {
        g_rx_buf[g_num] = USART_ReceiveData(macUSART4);
        if(g_rx_buf[g_num] == FRAME_START)
        {
            memset(g_rx_buf, 0, sizeof (g_rx_buf));
            g_rx_buf[0] = FRAME_START;
            g_num = 0;
            g_num++;
        }
        else if( g_rx_buf[g_num] == FRAME_END)  // 当接收到的值等于'>'时，结束一帧数据
        {
            g_rx_buf[g_num + 1] = 0;
            /* 发布消息到消息队列 queue */
            OSQPost ((OS_Q        *)&queue_uart,                            //消息变量指针
                     (void        *)g_rx_buf,                               //要发送的数据的指针，将内存块首地址通过队列"发送出去"
                     (OS_MSG_SIZE  )strlen ( (const char*)g_rx_buf ),       //数据字节大小
                     (OS_OPT       )OS_OPT_POST_FIFO | OS_OPT_POST_ALL,     //先进先出和发布给全部任务的形式
                     (OS_ERR      *)&err);                                  //返回错误类型
        }
        // 当值不等时候，则继续接收下一个
        else
        {
            g_num++;
            if (g_num > 50) //一帧数据最大50字节,超出则丢弃
            {
                g_num = 0;
            }
        }
    }
    OS_CRITICAL_EXIT();
}
#endif
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

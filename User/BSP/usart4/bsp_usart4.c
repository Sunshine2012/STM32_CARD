/**
  ******************************************************************************
  * @file    bsp_usart4.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重定向c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:
  * 论坛    :
  * 淘宝    :
  *
  ******************************************************************************
  */

#include "bsp_usart4.h"

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void UART4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 嵌套向量中断控制器组选择 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* 配置USART为中断源 */
    NVIC_InitStructure.NVIC_IRQChannel = macUSART4_IRQ;
    /* 抢断优先级*/
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    /* 子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    /* 使能中断 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    /* 初始化配置NVIC */
    NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USARTx GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USART4_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	macUSART4_GPIO_APBxClock_FUN(macUSART4_GPIO_CLK, ENABLE);

	// 打开串口外设的时钟
	macUSART4_APBxClock_FUN(macUSART4_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = macUSART4_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(macUSART4_TX_PORT, &GPIO_InitStructure);

    // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = macUSART4_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(macUSART4_RX_PORT, &GPIO_InitStructure);

	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = macUSART4_BAUD_RATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(macUSART4, &USART_InitStructure);

	// 串口中断优先级配置
	UART4_NVIC_Configuration();

	// 使能串口接收中断
	USART_ITConfig(macUSART4, USART_IT_RXNE, ENABLE);

	// 使能串口
	USART_Cmd(macUSART4, ENABLE);

  // 清除发送完成标志
	//USART_ClearFlag(USART1, USART_FLAG_TC);
}

/*****************  发送一个字符 **********************/
void USART4_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);

	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  发送字符串 **********************/
void USART4_SendString( USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k=0;
    do
    {
        USART4_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');

  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart4_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;

	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  发送字符串 **********************/
void USART4_nSendString(USART_TypeDef * pUSARTx,char *str,int n)
{
    unsigned int k=0;
    do
    {
        /* 发送一个字节数据到USART */
        USART_SendData(pUSARTx,str[k]);

        /* 等待发送数据寄存器为空 */
        while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
        k++;
    } while(k < n);
}

#ifdef uart4
///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    u32 i = 100000000;
	/* 发送一个字节数据到串口 */
	USART_SendData(macUSART4, (uint8_t) ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(macUSART4, USART_FLAG_TXE) == RESET)
    {
        if (i-- == 0)
        {
            break;
        }
    }

	return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
    u32 i = 100000000;
	/* 等待串口输入数据 */
	while (USART_GetFlagStatus(macUSART4, USART_FLAG_RXNE) == RESET)
	{
		if (i-- == 0)
        {
            break;
        }
    }
	return (int)USART_ReceiveData(macUSART4);
}
/*********************************************END OF FILE**********************/
#endif


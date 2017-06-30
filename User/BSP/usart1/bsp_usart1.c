#include "bsp_usart1.h"

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void UART1_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = macUSART1_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}
 /**
  * @brief  USARTx GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	
	/* config USART1 clock */
	macUSART1_APBxClock_FUN(macUSART1_CLK, ENABLE);
	macUSART1_GPIO_APBxClock_FUN(macUSART1_GPIO_CLK, ENABLE);
		
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin =  macUSART1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(macUSART1_TX_PORT, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = macUSART1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(macUSART1_RX_PORT, &GPIO_InitStructure);	
			
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = macUSART1_BAUD_RATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(macUSART1, &USART_InitStructure);
	
	USART_Cmd(macUSART1, ENABLE);
    
    	// �����ж����ȼ�����
	UART1_NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(macUSART1, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(macUSART1, ENABLE);		

  // ���������ɱ�־
	//USART_ClearFlag(USART1, USART_FLAG_TC);  
	
}

#ifdef uart1
/// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(macUSART1x, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(macUSART1x, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}


/// �ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(macUSART1x, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(macUSART1x);
}
/*********************************************END OF FILE**********************/
#endif

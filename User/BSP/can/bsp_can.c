#include <includes.h>
#include "stm32f10x.h"


static void CAN_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

#if 0
    /* Configure CAN pin: RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Configure CAN pin: TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE );	   //重影射CAN IO脚到 PD0，PD1

    /* Configure CAN pin: RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure CAN pin: TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE );	   //重影射CAN IO脚到 PD0，PD1
#endif

    /* Configure CAN pin: RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure CAN pin: TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE );	   //重影射CAN IO脚到 PB8，PB9,    如果是使用PA12发送，PA11则不需要重映射
}

//系统中断管理
void CAN_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
#if 0
    #ifdef  VECT_TAB_RAM
        /* Set the Vector Table base location at 0x20000000 */
        NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
    #else  /* VECT_TAB_FLASH  */
        /* Set the Vector Table base location at 0x08000000 */
        NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    #endif
#endif
    /* enabling interrupt */
    NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//配置系统时钟,使能各外设时钟
void CAN_RCC_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
    /* CAN Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
}



void MyCANTransmit (void)
{
    u32 i = 0;
    u8 TransmitMailbox;
    CanTxMsg TxMessage;
    //CanRxMsg RxMessage;

    /* transmit */

    TxMessage.StdId = 0x00;
    TxMessage.ExtId = 0x7800;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_EXT;;
    TxMessage.DLC = 8;
    TxMessage.Data[0] = 0x55;
    TxMessage.Data[1] = 0xA0;
    TxMessage.Data[2] = 0x50;
    TxMessage.Data[3] = 0xA0;
    TxMessage.Data[4] = 0x50;
    TxMessage.Data[5] = 0xA0;
    TxMessage.Data[6] = 0x50;
    TxMessage.Data[7] = 0xA0;
    TransmitMailbox = CAN_Transmit(CAN1,&TxMessage);
    i = 0;
    while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (i != 0xFF))
    {
        i++;
    }

    i = 0;
    while((CAN_MessagePending(CAN1,CAN_FIFO0) < 1) && (i != 0xFF))
    {
        i++;
    }
}


void CANTransmit (void * p_Msg)
{
    u32 i = 0;
    u8 TransmitMailbox;
    CanTxMsg *p_TxMessage = (CanTxMsg *)p_Msg;
    //CanRxMsg RxMessage;

    /* transmit */
    TransmitMailbox = CAN_Transmit(CAN1,p_TxMessage);
    i = 0;
    while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (i != 0xFF))
    {
        i++;
    }

    i = 0;
    while((CAN_MessagePending(CAN1,CAN_FIFO0) < 1) && (i != 0xFF))
    {
        i++;
    }
}


/*******************************************************************************
* Function Name  : CAN_Polling
* Description    : Configures the CAN and transmit and receive by polling
* Input          : None
* Output         : None
* Return         : PASSED if the reception is well done, FAILED in other case
*******************************************************************************/
void CAN_Polling(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* CAN register init */
  CAN_DeInit(CAN1);
  // aiwesky 20140506
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;
  CAN_InitStructure.CAN_ABOM=DISABLE;
  CAN_InitStructure.CAN_AWUM=DISABLE;
  CAN_InitStructure.CAN_NART=DISABLE;
  CAN_InitStructure.CAN_RFLM=DISABLE;
  CAN_InitStructure.CAN_TXFP=DISABLE;
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1=CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2=CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler=6;
  CAN_Init(CAN1,&CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
  //
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

}

/*******************************************************************************
* Function Name  : CAN_Interrupt
* Description    : Configures the CAN and transmit and receive by interruption
* Input          : None
* Output         : None
* Return         : PASSED if the reception is well done, FAILED in other case
*******************************************************************************/
void CAN_Interrupt(void)
{
  //http://blog.csdn.net/flydream0/article/details/8148791   设置来源为此网站
  u32 ext_id =0x7810;    // 设置过滤ID的屏蔽位。
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;
  CAN_InitStructure.CAN_ABOM=DISABLE;
  CAN_InitStructure.CAN_AWUM=DISABLE;
  CAN_InitStructure.CAN_NART=DISABLE;
  CAN_InitStructure.CAN_RFLM=DISABLE;
  CAN_InitStructure.CAN_TXFP=DISABLE;
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
  // 波特率为   36MHz / CAN_Prescaler / (CAN_SJW + CAN_BS1 + CAN_BS2)    = 500kps    aiwesky 20170704
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1=CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2=CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler=6;
  CAN_Init(CAN1,&CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=                CAN_FilterMode_IdList;   // 如果是过滤 '一组' 标识符(ID),则使用CAN_FilterMode_IdMask模式,如果是过滤 '一个'标识符(ID),则使用CAN_FilterMode_IdList
  CAN_FilterInitStructure.CAN_FilterScale=               CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=              ((ext_id<<3)>>16) & 0xffff;
  CAN_FilterInitStructure.CAN_FilterIdLow=               ((ext_id<<3)& 0xffff)  | CAN_ID_EXT;      //

  //CAN_FilterInitStructure.CAN_FilterMaskIdHigh=          ((ext_id<<3)>>16) & 0xffff;
  //CAN_FilterInitStructure.CAN_FilterMaskIdLow=           ((ext_id<<3)& 0xffff) | CAN_ID_EXT;     // 如果是列表模式,则不需要设置屏蔽位寄存器
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=      CAN_FIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation=          ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* CAN FIFO0 message pending interrupt enable */
  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);


}

CanRxMsg RxMessage = {0};   // 发布消息，应该使用全局的变量，否则出错    20170720
/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                             //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                             //，开中断时将该值还原。
    OS_ERR      err;

    //MyCANTransmit ();

    OS_CRITICAL_ENTER();                 //进入临界段，不希望下面串口打印遭到中断
    CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
    CANTransmit (&RxMessage);
    if(((0x0000ffff & RxMessage.ExtId) == 0x00007800) && (RxMessage.IDE == CAN_ID_EXT))
    {
        /* 发布消息到消息队列 queue */
        OSQPost ((OS_Q        *)&queue_can,                             //消息变量指针
                (void        *)&RxMessage,                              //要发送的数据的指针，将内存块首地址通过队列"发送出去"
                (OS_MSG_SIZE  )sizeof (RxMessage) ,                     //数据字节大小
                (OS_OPT       )OS_OPT_POST_FIFO | OS_OPT_POST_ALL,      //先进先出和发布给全部任务的形式
                (OS_ERR      *)&err);
    }
    else
    {

    }
    OS_CRITICAL_EXIT();
}



void CAN_init( void )
{
    CAN_RCC_Config ();      // 初始化总线
    CAN_GPIO_Config ();     // 初始化GPIO,需要用到TX:PA12 ,RX:PA11
    CAN_NVIC_Config ();     // 接收中断
    CAN_Interrupt();        // 设置波特率等其他参数
}

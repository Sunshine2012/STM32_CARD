#include <includes.h>
#include "stm32f10x.h"
#include "bsp_can.h"
CanTxMsg gt_TxMessage;      // CAN�������ݻ���
unsigned int g_uiSerNum = 0; // ֡���,ȫ��,����������֮���֡���

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

    GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE );    //��Ӱ��CAN IO�ŵ� PD0��PD1

    /* Configure CAN pin: RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure CAN pin: TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE );    //��Ӱ��CAN IO�ŵ� PD0��PD1
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
    //GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE );      //��Ӱ��CAN IO�ŵ� PB8��PB9,    �����ʹ��PA12���ͣ�PA11����Ҫ��ӳ��
}

//ϵͳ�жϹ���
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

//����ϵͳʱ��,ʹ�ܸ�����ʱ��
void CAN_RCC_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
    /* CAN Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
}

unsigned char myCANTransmit_ID(void * p_Msg, unsigned char targetID, unsigned char mechineID, unsigned char boxNum, unsigned char cmd, unsigned char status,
                      unsigned char data_H, unsigned char data_L, unsigned char errNum)
{
    u32 i = 0;
    //static u8 uCount = 0;
    u8 TransmitMailbox;
    CanTxMsg *p_TxMessage = (CanTxMsg *)p_Msg;
    CanTxMsg TxMessage;
    memset(p_TxMessage,0,sizeof (CanTxMsg));
    p_TxMessage->StdId = 0x00;
    p_TxMessage->ExtId = 0x7810 | targetID;
    p_TxMessage->RTR = CAN_RTR_DATA;
    p_TxMessage->IDE = CAN_ID_EXT;;
    p_TxMessage->DLC = 8;
    p_TxMessage->Data[0] = g_uiSerNum;
    p_TxMessage->Data[1] = mechineID;
    p_TxMessage->Data[2] = boxNum;
    p_TxMessage->Data[3] = cmd;
    p_TxMessage->Data[4] = status;
    p_TxMessage->Data[5] = data_H;
    p_TxMessage->Data[6] = data_L;
    p_TxMessage->Data[7] = errNum;

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
    g_uiSerNum++;           // ֡���ÿ�μ�1
    return 0;
}


unsigned char myCANTransmit (void * p_Msg, unsigned char mechine_id, unsigned char boxNum, unsigned char cmd, unsigned char status,
                      unsigned char data_H, unsigned char data_L, unsigned char errNum)
{
    u32 i = 0;
    OS_ERR      err;
    u8 TransmitMailbox;
    CanTxMsg *p_TxMessage = (CanTxMsg *)p_Msg;
    CanTxMsg TxMessage;
    memset(p_TxMessage,0,sizeof (CanTxMsg));
    p_TxMessage->StdId = 0x00;
    p_TxMessage->ExtId = 0x7810 | mechine_id;
    p_TxMessage->RTR = CAN_RTR_DATA;
    p_TxMessage->IDE = CAN_ID_EXT;;
    p_TxMessage->DLC = 8;
    p_TxMessage->Data[0] = g_uiSerNum;
    p_TxMessage->Data[1] = mechine_id;
    p_TxMessage->Data[2] = boxNum;
    p_TxMessage->Data[3] = cmd;
    p_TxMessage->Data[4] = status;
    p_TxMessage->Data[5] = data_H;
    p_TxMessage->Data[6] = data_L;
    p_TxMessage->Data[7] = errNum;
    //OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );
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
    g_uiSerNum++;           // ֡���ÿ�μ�1
    OSTimeDly ( 30, OS_OPT_TIME_DLY, & err );
    return 0;
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
  //http://blog.csdn.net/flydream0/article/details/8148791   ������ԴΪ����վ
  //u32 ext_id =0x7810;    // ���ù���ID������λ��
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
  // ������Ϊ   36MHz / CAN_Prescaler / (CAN_SJW + CAN_BS1 + CAN_BS2)    = 50kps    aiwesky 20171022
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1=CAN_BS1_10tq;           // CAN_BS1_6tq ------> CAN_BS1_10tq
  CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;            // CAN_BS1_5tq ------> CAN_BS1_7tq
  CAN_InitStructure.CAN_Prescaler=40;               // 6 -----> 40
  CAN_Init(CAN1,&CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=                CAN_FilterMode_IdMask;   // ����ǹ��� 'һ��' ��ʶ��(ID),��ʹ��CAN_FilterMode_IdMaskģʽ,����ǹ��� 'һ��'��ʶ��(ID),��ʹ��CAN_FilterMode_IdList
  CAN_FilterInitStructure.CAN_FilterScale=               CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;

  //CAN_FilterInitStructure.CAN_FilterIdHigh=              ((ext_id<<3)>>16) & 0xffff;
  //CAN_FilterInitStructure.CAN_FilterIdLow=               ((ext_id<<3)& 0xffff)  | CAN_ID_EXT;      //

  //CAN_FilterInitStructure.CAN_FilterMaskIdHigh=          ((ext_id<<3)>>16) & 0xffff;
  //CAN_FilterInitStructure.CAN_FilterMaskIdLow=           ((ext_id<<3)& 0xffff) | CAN_ID_EXT;     // ������б�ģʽ,����Ҫ��������λ�Ĵ���
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=      CAN_FIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation=          ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* CAN FIFO0 message pending interrupt enable */
  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);


}

CanRxMsg gt_RxMessage = {0};   // ������Ϣ��Ӧ��ʹ��ȫ�ֵı������������    20170720
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
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                             //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                             //�����ж�ʱ����ֵ��ԭ��
    OS_ERR      err;

    OS_CRITICAL_ENTER();                 //�����ٽ�Σ���ϣ�����洮�ڴ�ӡ�⵽�ж�
    CAN_Receive(CAN1,CAN_FIFO0, &gt_RxMessage);
    // CANTransmit (&gt_RxMessage);
    // macLED1_TOGGLE ();
    if(((0x0000ff00 & gt_RxMessage.ExtId) == 0x00007800) && (gt_RxMessage.IDE == CAN_ID_EXT))
    {
        /* ������Ϣ����Ϣ���� queue */
        OSQPost ((OS_Q        *)&queue_can,                             //��Ϣ����ָ��
                (void        *)&gt_RxMessage,                           //Ҫ���͵����ݵ�ָ�룬���ڴ���׵�ַͨ������"���ͳ�ȥ"
                (OS_MSG_SIZE  )sizeof (gt_RxMessage) ,                  //�����ֽڴ�С
                (OS_OPT       )OS_OPT_POST_FIFO | OS_OPT_POST_ALL,      //�Ƚ��ȳ��ͷ�����ȫ���������ʽ
                (OS_ERR      *)&err);
    }
    else
    {

    }
    OS_CRITICAL_EXIT();
}



void CAN_init( void )
{
    CAN_RCC_Config ();      // ��ʼ������
    CAN_GPIO_Config ();     // ��ʼ��GPIO,��Ҫ�õ�TX:PA12 ,RX:PA11
    CAN_NVIC_Config ();     // �����ж�
    CAN_Interrupt();        // ���ò����ʵ���������
    gt_TxMessage.StdId = 0x00;
    gt_TxMessage.ExtId = 0x7800;
    gt_TxMessage.RTR = CAN_RTR_DATA;
    gt_TxMessage.IDE = CAN_ID_EXT;;
    gt_TxMessage.DLC = 8;
    gt_TxMessage.Data[0] = 0x55;
    gt_TxMessage.Data[1] = 0xA0;
    gt_TxMessage.Data[2] = 0x50;
    gt_TxMessage.Data[3] = 0xA0;
    gt_TxMessage.Data[4] = 0x50;
    gt_TxMessage.Data[5] = 0xA0;
    gt_TxMessage.Data[6] = 0x50;
    gt_TxMessage.Data[7] = 0xA0;
}

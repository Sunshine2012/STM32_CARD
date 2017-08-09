/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                        Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include "WAV_C_xiexie.h"
#include "WAV_C_anjianquka.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

CPU_TS             ts_start;       //ʱ�������
CPU_TS             ts_end;



/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

OS_TCB   AppTaskStartTCB;                                //������ƿ�

OS_TCB   AppTaskTmrTCB;

OS_TCB   AppTaskOledTCB;                // OLED����������ƿ�

OS_TCB   AppTaskCanFrameTCB;            // ��������������ƿ�

OS_TCB   AppTaskUartFrameTCB;           // ��ȡ��������������ƿ�

OS_TCB   AppTaskKeyScanTCB;             // ����ɨ��

OS_Q queue_uart;     // ��Ϣ����

OS_Q queue_can;      // ��Ϣ����


/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk [ APP_TASK_START_STK_SIZE ];       //�����ջ

static  CPU_STK  AppTaskTmrStk [ APP_TASK_TMR_STK_SIZE ];

static  CPU_STK  AppTaskOLEDStk [ APP_TASK_OLED_STK_SIZE ];

static  CPU_STK  AppTaskCanFrameStk [ APP_TASK_CAN_FRAME_STK_SIZE ];

static  CPU_STK  AppTaskUartFrameStk [ APP_TASK_UART_FRAME_STK_SIZE ];

static  CPU_STK  AppTaskKeyScanStk [ APP_TASK_KEY_SCAN_STK_SIZE ];
/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart  (void *p_arg);                       //����������

static  void  AppTaskTmr  ( void * p_arg );

static  void  AppTaskOLED  ( void * p_arg );

static void  AppTaskCanFrame ( void * p_arg );

static void  AppTaskUartFrame ( void * p_arg );

static void  AppTaskKeyScan ( void * p_arg );

extern unsigned char BMP1[];

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main (void)
{
    OS_ERR  err;


    OSInit(&err);                                                           //��ʼ�� uC/OS-III

    /* ������ʼ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Start",                            //��������
                 (OS_TASK_PTR ) AppTaskStart,                               //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //��������ȼ�
                 (CPU_STK    *)&AppTaskStartStk[0],                         //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������

    OSStart(&err);                                                          //�����������������uC/OS-III���ƣ�
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;


   (void)p_arg;

    BSP_Init();                                                 //�弶��ʼ��
    CPU_Init();                                                 //��ʼ�� CPU �����ʱ��������ж�ʱ���������������

    cpu_clk_freq = BSP_CPU_ClkFreq();                           //��ȡ CPU �ں�ʱ��Ƶ�ʣ�SysTick ����ʱ�ӣ�
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        //�����û��趨��ʱ�ӽ���Ƶ�ʼ��� SysTick ��ʱ���ļ���ֵ
    OS_CPU_SysTickInit(cnts);                                   //���� SysTick ��ʼ�����������ö�ʱ������ֵ��������ʱ��

    Mem_Init();                                                 //��ʼ���ڴ������������ڴ�غ��ڴ�ر�

#if OS_CFG_STAT_TASK_EN > 0u                                    //���ʹ�ܣ�Ĭ��ʹ�ܣ���ͳ������
    OSStatTaskCPUUsageInit(&err);                               //����û��Ӧ������ֻ�п�����������ʱ CPU �ģ����
#endif                                                          //���������� OS_Stat_IdleCtrMax ��ֵ��Ϊ������� CPU
                                                                //ʹ����ʹ�ã���
    CPU_IntDisMeasMaxCurReset();                                //��λ�����㣩��ǰ�����ж�ʱ��

    /* ������Ϣ���� queue */
    OSQCreate   ((OS_Q       *)&queue_uart,         //ָ����Ϣ���е�ָ��
                (CPU_CHAR    *)"uart Frame",        //���е�����
                (OS_MSG_QTY   )30,                  //���ɴ����Ϣ����Ŀ
                (OS_ERR      *)&err);               //���ش�������

    /* ������Ϣ���� queue */
    OSQCreate   ((OS_Q       *)&queue_can,          //ָ����Ϣ���е�ָ��
                (CPU_CHAR    *)"can Frame",         //���е�����
                (OS_MSG_QTY   )30,                  //���ɴ����Ϣ����Ŀ
                (OS_ERR      *)&err);               //���ش�������

    /* ���� AppTaskTmr ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskTmrTCB,                              //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Tmr",                              //��������
                 (OS_TASK_PTR ) AppTaskTmr,                                 //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_TMR_PRIO,                          //��������ȼ�
                 (CPU_STK    *)&AppTaskTmrStk[0],                           //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_TMR_STK_SIZE / 10,                 //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_TMR_STK_SIZE,                      //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������

    /* ���� AppTaskOLED ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskOledTCB,                             //������ƿ��ַ
                 (CPU_CHAR   *)"OLED_APP",                                  //��������
                 (OS_TASK_PTR ) AppTaskOLED,                                //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_OLED_PRIO,                         //��������ȼ�
                 (CPU_STK    *)&AppTaskOLEDStk[0],                          //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_OLED_STK_SIZE / 10,                //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_OLED_STK_SIZE,                     //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);

    /* ���� AppTaskCanFRAME ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskCanFrameTCB,                         //������ƿ��ַ
                 (CPU_CHAR   *)"READ_CAN_FRAME_APP",                        //��������
                 (OS_TASK_PTR ) AppTaskCanFrame,                            //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_CAN_FRAME_PRIO,                    //��������ȼ�
                 (CPU_STK    *)&AppTaskCanFrameStk[0],                      //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_CAN_FRAME_STK_SIZE / 10,           //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_CAN_FRAME_STK_SIZE,                //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);

    /* ���� AppTaskUartFRAME ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskUartFrameTCB,                        //������ƿ��ַ
                 (CPU_CHAR   *)"READ_UART_FRAME_APP",                       //��������
                 (OS_TASK_PTR ) AppTaskUartFrame,                           //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_UART_FRAME_PRIO,                   //��������ȼ�
                 (CPU_STK    *)&AppTaskUartFrameStk[0],                     //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_UART_FRAME_STK_SIZE / 10,          //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_UART_FRAME_STK_SIZE,               //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);

        /* ���� AppTaskKeyScan ���� */
    OSTaskCreate((OS_TCB     *)&AppTaskKeyScanTCB,                          //������ƿ��ַ
                 (CPU_CHAR   *)"Key_Scan",                                  //��������
                 (OS_TASK_PTR ) AppTaskKeyScan,                             //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_KEY_SCAN_PRIO,                     //��������ȼ�
                 (CPU_STK    *)&AppTaskKeyScanStk[0],                       //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) APP_TASK_KEY_SCAN_STK_SIZE / 10,            //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) APP_TASK_KEY_SCAN_STK_SIZE,                 //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);

    OSTaskDel ( & AppTaskStartTCB, & err );                     //ɾ����ʼ������������������
}


/*
*********************************************************************************************************
*                                          TMR TASK
*********************************************************************************************************
*/
void TmrCallback (OS_TMR *p_tmr, void *p_arg) //�����ʱ��MyTmr�Ļص�����
{

    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                             //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                             //�����ж�ʱ����ֵ��ԭ��
    CPU_INT32U       cpu_clk_freq;

    g_tCardMechineStatusFrame.RSCTL = (g_ucSerNum++ % 10) + '0';

    cpu_clk_freq = BSP_CPU_ClkFreq();                   //��ȡCPUʱ�ӣ�ʱ������Ը�ʱ�Ӽ���

    //macLED2_TOGGLE ();

    ts_end = OS_TS_GET() - ts_start;     //��ȡ��ʱ���ʱ�������CPUʱ�ӽ��м�����һ������ֵ��
                                         //�������㶨ʱʱ�䡣
    OS_CRITICAL_ENTER();                 //�����ٽ�Σ���ϣ�����洮�ڴ�ӡ�⵽�ж�

    //printf ( "\r\n��ʱ2s��ͨ��ʱ�����ö�ʱ %07d us���� %04d ms��\r\n",
    //                    ts_end / ( cpu_clk_freq / 1000000 ),     //����ʱʱ������� us
    //                    ts_end / ( cpu_clk_freq / 1000 ) );      //����ʱʱ������� ms

    printf ( "%s\n", ( char * ) &g_tCardMechineStatusFrame );
    OS_CRITICAL_EXIT();

    ts_start = OS_TS_GET();                            //��ȡ��ʱǰʱ���
}


static  void  AppTaskTmr ( void * p_arg )
{
    OS_ERR      err;
    OS_TMR      my_tmr;   //���������ʱ������

    (void)p_arg;

    /* ���������ʱ�� */
    OSTmrCreate ((OS_TMR              *)&my_tmr,           //�����ʱ������
               (CPU_CHAR            *)"MySoftTimer",       //���������ʱ��
               (OS_TICK              )20,                  //��ʱ����ʼֵ����10Hzʱ�����㣬��Ϊ1s
               (OS_TICK              )20,                  //��ʱ����������ֵ����10Hzʱ�����㣬��Ϊ1s
               (OS_OPT               )OS_OPT_TMR_PERIODIC, //�����Զ�ʱ
               (OS_TMR_CALLBACK_PTR  )TmrCallback,         //�ص�����
               (void                *)"Timer Over!",       //����ʵ�θ��ص�����
               (OS_ERR              *)err);                //���ش�������

    /* ���������ʱ�� */
    OSTmrStart ((OS_TMR   *)&my_tmr, //�����ʱ������
              (OS_ERR   *)err);    //���ش�������

    ts_start = OS_TS_GET();                       //��ȡ��ʱǰʱ���

    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��

        OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //��������������

    }
}

TestStatus Buffercmp(u8* pBuffer1,u8* pBuffer2, uint16_t BufferLength);

/*
 * ��������Buffercmp
 * ����  ���Ƚ������������е������Ƿ����
 * ����  ��-pBuffer1     src������ָ��
 *         -pBuffer2     dst������ָ��
 *         -BufferLength ����������
 * ���  ����
 * ����  ��-PASSED pBuffer1 ����   pBuffer2
 *         -FAILED pBuffer1 ��ͬ�� pBuffer2
 */
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}
static  void AppTaskOLED ( void * p_arg )
{
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                 //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                 //�����ж�ʱ����ֵ��ԭ.
   char *pcMsg = NULL;
   OS_MSG_SIZE    msg_size;
   OS_ERR         err;
   CPU_TS         ts;
   /* ���ͻ�������ʼ�� */
    u8 Tx_Buffer[] = "11\r\n";
    typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
    #define countof(a)      (sizeof(a) / sizeof(*(a)))
    /* ��ȡ�������ĳ��� */
    #define TxBufferSize1   (countof(TxBuffer1) - 1)
    #define RxBufferSize1   (countof(TxBuffer1) - 1)

    #define  BufferSize (countof(Tx_Buffer)-1)

    #define  FLASH_WriteAddress     0x00000
    #define  FLASH_ReadAddress      FLASH_WriteAddress
    #define  FLASH_SectorToErase    FLASH_WriteAddress
    __IO uint32_t DeviceID = 0;
    __IO uint32_t FlashID = 0;
    __IO TestStatus TransferStatus1 = FAILED;

    u8 Rx_Buffer[BufferSize] = "";

#ifdef OLED
    //u8 arr[]
    macLED1_ON();
    //macLED3_ON();
    OLED_Init ();
    //OSTimeDly ( 10, OS_OPT_TIME_DLY, & err ); //��������������
    OLED_SetPos(0,0);
    OLED_CLS();
    //OLED_ShowCN(0,0,2);
    //OLED_ShowCN(16,0,3);
    //OLED_Fill(0xff);
    //OLED_DrawBMP(0,0,128,8,BMP1);
    display_GB2312_string (0,0,g_dlg[1].MsgRow0);
    display_GB2312_string (0,2,g_dlg[1].MsgRow1);
    display_GB2312_string (0,4,g_dlg[1].MsgRow2);
    display_GB2312_string (0,6,g_dlg[1].MsgRow3);
#endif

    //SPI_FLASH_Init();

    //OSTimeDly ( 10, OS_OPT_TIME_DLY, & err ); // ��ʱ
    /* ��ȡ SPI Flash ID */
    OS_CRITICAL_ENTER();                 //�����ٽ�Σ���ϣ�����洮�ڴ�ӡ�⵽�ж�
    FlashID = SPI_FLASH_ReadID();
    printf("FlashID is 0x%X\r\n", FlashID);

    /* ���� SPI Flash ID */
    if (FlashID == sFLASH_ID)
    {
        SPI_FLASH_UnprotectSector(FLASH_SectorToErase);  // ���Բ���Ҫ����
        printf("��⵽����flash AT26F004 !\r\n");

        /* ������Ҫд��� SPI FLASH ������FLASHд��ǰҪ�Ȳ��� */
        // �������4K����һ����������������С��λ������
        SPI_FLASH_SectorErase(FLASH_SectorToErase);
        //SPI_FLASH_ChipErase();

        /* �����ͻ�����������д��flash�� */
        // ����дһҳ��һҳ�Ĵ�СΪ256���ֽ�

        SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);

        printf("д�������Ϊ:%s\r\n", Tx_Buffer);
        //OSTimeDly ( 10, OS_OPT_TIME_DLY, & err ); // ��ʱ
        /* ���ո�д������ݶ������ŵ����ջ������� */
        SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
        printf("����������Ϊ:%s\r\n", Rx_Buffer);
        /* ���д�������������������Ƿ���� */
        TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);

        if( PASSED == TransferStatus1 )
        {
            printf("8M flash(AT26F004)ok!\r\n");
        }
        else
        {
            printf("8M flash(AT26F004)fail!\r\n");
        }
    }
    OS_CRITICAL_EXIT();
    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��
       //macLED2_TOGGLE ();
       /*
        OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //��������������

        dacSet(DATA_anjianquka,SOUND_LENGTH_anjianquka);
        OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );

        dacSet(DATA_xiexie,SOUND_LENGTH_xiexie);
        OSTimeDly ( 2500, OS_OPT_TIME_DLY, & err );
       */
       /* �������񣬵ȴ�������Ϣ */
       /*
       pcMsg = OSTaskQPend ((OS_TICK        )0,                    //�����޵ȴ�
                            (OS_OPT         )OS_OPT_PEND_BLOCKING, //û����Ϣ����������
                            (OS_MSG_SIZE   *)&msg_size,            //������Ϣ����
                            (CPU_TS        *)&ts,                  //������Ϣ��������ʱ���
                            (OS_ERR        *)&err);                //���ش�������
       */
       macLED1_TOGGLE ();

       OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
    }
}



void  AppTaskCanFrame ( void * p_arg )
{
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                     //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                     //�����ж�ʱ����ֵ��ԭ.
    OS_ERR      err;
    OS_MSG_SIZE    msg_size;
    CPU_INT08U * pMsg = NULL;
    CanRxMsg *ptRxMessage = NULL;                // can���ݽ��ջ���

    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��

        //OS_CRITICAL_ENTER();                 //�����ٽ�Σ���ϣ�����洮�ڴ�ӡ�⵽�ж�
        /* ������Ϣ���� queue ����Ϣ */
        ptRxMessage = OSQPend ((OS_Q         *)&queue_can,            //��Ϣ����ָ��
                        (OS_TICK       )10,                    //�ȴ�ʱ��
                        (OS_OPT        )OS_OPT_PEND_BLOCKING,  //���û�л�ȡ���ź����͵ȴ�
                        (OS_MSG_SIZE  *)&msg_size,             //��ȡ��Ϣ���ֽڴ�С
                        (CPU_TS       *)0,                     //��ȡ������ʱ��ʱ���
                        (OS_ERR       *)&err);                 //���ش���
        //OS_CRITICAL_EXIT();
        if(ptRxMessage != NULL)
        {
            AnalyzeCANFrame((void *)ptRxMessage);
            // printf ("%s",(char *)ptRxMessage);
        }
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err ); //��������������
    }
}

void  AppTaskUartFrame ( void * p_arg )
{
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                     //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                     //�����ж�ʱ����ֵ��ԭ.
    OS_ERR      err;
    OS_MSG_SIZE    msg_size;
    CPU_INT08U * pMsg = NULL;
    //CPU_INT08U ucaMsg[30] = "0aiwesky uC/OS-III";

    OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //�ȴ�1S
    OS_CRITICAL_ENTER();                 //�����ٽ�Σ���ϣ�����洮�ڴ�ӡ�⵽�ж�
    DEBUG_printf ("%s\r\n","���,��ӭʹ����Ϊ���Ӱ忨ϵͳ");

    printf("%s", (char *)&g_tCardMechinePowerOnFrame);      // �ϵ���Ϣ

    OS_CRITICAL_EXIT();

    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��
        pMsg = OSQPend ((OS_Q         *)&queue_uart,            //��Ϣ����ָ��
                        (OS_TICK       )10,                     //�ȴ�ʱ��
                        (OS_OPT        )OS_OPT_PEND_BLOCKING,   //���û�л�ȡ���ź����͵ȴ�
                        (OS_MSG_SIZE  *)&msg_size,              //��ȡ��Ϣ���ֽڴ�С
                        (CPU_TS       *)0,                      //��ȡ������ʱ��ʱ���
                        (OS_ERR       *)&err);                  //���ش���

        //macLED2_TOGGLE ();
        if (pMsg != NULL)
        {
            AnalyzeUartFrame((void *)pMsg);
        }
        if(pMsg != NULL)
        {
            //OLED_ShowStr(0,0,pMsg,1);
        }
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //��������������
    }
}


void  AppTaskKeyScan ( void * p_arg )
{
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                     //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                     //�����ж�ʱ����ֵ��ԭ.
    OS_ERR      err;
    OS_MSG_SIZE    msg_size;
    //OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //�ȴ�1S
    //OS_CRITICAL_ENTER ();                 //�����ٽ�Σ���ϣ�����洮�ڴ�ӡ�⵽�ж�
	 //macLED1_TOGGLE ();
    //OS_CRITICAL_EXIT ();

    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��


        //macLED2_TOGGLE ();
        //macLED3_TOGGLE ();


        matrix_update_key();
        OSTimeDly ( 20, OS_OPT_TIME_DLY, & err );     //��������������
    }
}
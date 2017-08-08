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

CPU_TS             ts_start;       //时间戳变量
CPU_TS             ts_end;



/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

OS_TCB   AppTaskStartTCB;                                //任务控制块

OS_TCB   AppTaskTmrTCB;

OS_TCB   AppTaskOledTCB;                // OLED更新任务控制块

OS_TCB   AppTaskCanFrameTCB;            // 发送数据任务控制块

OS_TCB   AppTaskUartFrameTCB;           // 读取解析数据任务控制块

OS_TCB   AppTaskKeyScanTCB;             // 按键扫描

OS_Q queue_uart;     // 消息队列

OS_Q queue_can;      // 消息队列


/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk [ APP_TASK_START_STK_SIZE ];       //任务堆栈

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

static  void  AppTaskStart  (void *p_arg);                       //任务函数声明

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


    OSInit(&err);                                                           //初始化 uC/OS-III

    /* 创建起始任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                            //任务控制块地址
                 (CPU_CHAR   *)"App Task Start",                            //任务名称
                 (OS_TASK_PTR ) AppTaskStart,                               //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //任务的优先级
                 (CPU_STK    *)&AppTaskStartStk[0],                         //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,               //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,                    //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型

    OSStart(&err);                                                          //启动多任务管理（交由uC/OS-III控制）
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

    BSP_Init();                                                 //板级初始化
    CPU_Init();                                                 //初始化 CPU 组件（时间戳、关中断时间测量和主机名）

    cpu_clk_freq = BSP_CPU_ClkFreq();                           //获取 CPU 内核时钟频率（SysTick 工作时钟）
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        //根据用户设定的时钟节拍频率计算 SysTick 定时器的计数值
    OS_CPU_SysTickInit(cnts);                                   //调用 SysTick 初始化函数，设置定时器计数值和启动定时器

    Mem_Init();                                                 //初始化内存管理组件（堆内存池和内存池表）

#if OS_CFG_STAT_TASK_EN > 0u                                    //如果使能（默认使能）了统计任务
    OSStatTaskCPUUsageInit(&err);                               //计算没有应用任务（只有空闲任务）运行时 CPU 的（最大）
#endif                                                          //容量（决定 OS_Stat_IdleCtrMax 的值，为后面计算 CPU
                                                                //使用率使用）。
    CPU_IntDisMeasMaxCurReset();                                //复位（清零）当前最大关中断时间

    /* 创建消息队列 queue */
    OSQCreate   ((OS_Q       *)&queue_uart,         //指向消息队列的指针
                (CPU_CHAR    *)"uart Frame",        //队列的名字
                (OS_MSG_QTY   )30,                  //最多可存放消息的数目
                (OS_ERR      *)&err);               //返回错误类型

    /* 创建消息队列 queue */
    OSQCreate   ((OS_Q       *)&queue_can,          //指向消息队列的指针
                (CPU_CHAR    *)"can Frame",         //队列的名字
                (OS_MSG_QTY   )30,                  //最多可存放消息的数目
                (OS_ERR      *)&err);               //返回错误类型

    /* 创建 AppTaskTmr 任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskTmrTCB,                              //任务控制块地址
                 (CPU_CHAR   *)"App Task Tmr",                              //任务名称
                 (OS_TASK_PTR ) AppTaskTmr,                                 //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_TMR_PRIO,                          //任务的优先级
                 (CPU_STK    *)&AppTaskTmrStk[0],                           //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_TMR_STK_SIZE / 10,                 //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_TMR_STK_SIZE,                      //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);                                       //返回错误类型

    /* 创建 AppTaskOLED 任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskOledTCB,                             //任务控制块地址
                 (CPU_CHAR   *)"OLED_APP",                                  //任务名称
                 (OS_TASK_PTR ) AppTaskOLED,                                //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_OLED_PRIO,                         //任务的优先级
                 (CPU_STK    *)&AppTaskOLEDStk[0],                          //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_OLED_STK_SIZE / 10,                //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_OLED_STK_SIZE,                     //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);

    /* 创建 AppTaskCanFRAME 任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskCanFrameTCB,                         //任务控制块地址
                 (CPU_CHAR   *)"READ_CAN_FRAME_APP",                        //任务名称
                 (OS_TASK_PTR ) AppTaskCanFrame,                            //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_CAN_FRAME_PRIO,                    //任务的优先级
                 (CPU_STK    *)&AppTaskCanFrameStk[0],                      //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_CAN_FRAME_STK_SIZE / 10,           //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_CAN_FRAME_STK_SIZE,                //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);

    /* 创建 AppTaskUartFRAME 任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskUartFrameTCB,                        //任务控制块地址
                 (CPU_CHAR   *)"READ_UART_FRAME_APP",                       //任务名称
                 (OS_TASK_PTR ) AppTaskUartFrame,                           //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_UART_FRAME_PRIO,                   //任务的优先级
                 (CPU_STK    *)&AppTaskUartFrameStk[0],                     //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_UART_FRAME_STK_SIZE / 10,          //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_UART_FRAME_STK_SIZE,               //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);

        /* 创建 AppTaskKeyScan 任务 */
    OSTaskCreate((OS_TCB     *)&AppTaskKeyScanTCB,                          //任务控制块地址
                 (CPU_CHAR   *)"Key_Scan",                                  //任务名称
                 (OS_TASK_PTR ) AppTaskKeyScan,                             //任务函数
                 (void       *) 0,                                          //传递给任务函数（形参p_arg）的实参
                 (OS_PRIO     ) APP_TASK_KEY_SCAN_PRIO,                     //任务的优先级
                 (CPU_STK    *)&AppTaskKeyScanStk[0],                       //任务堆栈的基地址
                 (CPU_STK_SIZE) APP_TASK_KEY_SCAN_STK_SIZE / 10,            //任务堆栈空间剩下1/10时限制其增长
                 (CPU_STK_SIZE) APP_TASK_KEY_SCAN_STK_SIZE,                 //任务堆栈空间（单位：sizeof(CPU_STK)）
                 (OS_MSG_QTY  ) 5u,                                         //任务可接收的最大消息数
                 (OS_TICK     ) 0u,                                         //任务的时间片节拍数（0表默认值OSCfg_TickRate_Hz/10）
                 (void       *) 0,                                          //任务扩展（0表不扩展）
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //任务选项
                 (OS_ERR     *)&err);

    OSTaskDel ( & AppTaskStartTCB, & err );                     //删除起始任务本身，该任务不再运行
}


/*
*********************************************************************************************************
*                                          TMR TASK
*********************************************************************************************************
*/
void TmrCallback (OS_TMR *p_tmr, void *p_arg) //软件定时器MyTmr的回调函数
{

    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                             //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                             //，开中断时将该值还原。
    CPU_INT32U       cpu_clk_freq;

    g_tCardMechineStatusFrame.RSCTL = (g_ucSerNum++ % 10) + '0';

    cpu_clk_freq = BSP_CPU_ClkFreq();                   //获取CPU时钟，时间戳是以该时钟计数

    //macLED2_TOGGLE ();

    ts_end = OS_TS_GET() - ts_start;     //获取定时后的时间戳（以CPU时钟进行计数的一个计数值）
                                         //，并计算定时时间。
    OS_CRITICAL_ENTER();                 //进入临界段，不希望下面串口打印遭到中断

    //printf ( "\r\n定时2s，通过时间戳测得定时 %07d us，即 %04d ms。\r\n",
    //                    ts_end / ( cpu_clk_freq / 1000000 ),     //将定时时间折算成 us
    //                    ts_end / ( cpu_clk_freq / 1000 ) );      //将定时时间折算成 ms

    printf ( "%s\n", ( char * ) &g_tCardMechineStatusFrame );
    OS_CRITICAL_EXIT();

    ts_start = OS_TS_GET();                            //获取定时前时间戳
}


static  void  AppTaskTmr ( void * p_arg )
{
    OS_ERR      err;
    OS_TMR      my_tmr;   //声明软件定时器对象

    (void)p_arg;

    /* 创建软件定时器 */
    OSTmrCreate ((OS_TMR              *)&my_tmr,           //软件定时器对象
               (CPU_CHAR            *)"MySoftTimer",       //命名软件定时器
               (OS_TICK              )20,                  //定时器初始值，依10Hz时基计算，即为1s
               (OS_TICK              )20,                  //定时器周期重载值，依10Hz时基计算，即为1s
               (OS_OPT               )OS_OPT_TMR_PERIODIC, //周期性定时
               (OS_TMR_CALLBACK_PTR  )TmrCallback,         //回调函数
               (void                *)"Timer Over!",       //传递实参给回调函数
               (OS_ERR              *)err);                //返回错误类型

    /* 启动软件定时器 */
    OSTmrStart ((OS_TMR   *)&my_tmr, //软件定时器对象
              (OS_ERR   *)err);    //返回错误类型

    ts_start = OS_TS_GET();                       //获取定时前时间戳

    while (DEF_TRUE)
    {                            //任务体，通常写成一个死循环

        OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //不断阻塞该任务

    }
}

TestStatus Buffercmp(u8* pBuffer1,u8* pBuffer2, uint16_t BufferLength);

/*
 * 函数名：Buffercmp
 * 描述  ：比较两个缓冲区中的数据是否相等
 * 输入  ：-pBuffer1     src缓冲区指针
 *         -pBuffer2     dst缓冲区指针
 *         -BufferLength 缓冲区长度
 * 输出  ：无
 * 返回  ：-PASSED pBuffer1 等于   pBuffer2
 *         -FAILED pBuffer1 不同于 pBuffer2
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
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                 //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                 //，开中断时将该值还原.
   char *pcMsg = NULL;
   OS_MSG_SIZE    msg_size;
   OS_ERR         err;
   CPU_TS         ts;
   /* 发送缓冲区初始化 */
    u8 Tx_Buffer[] = "11\r\n";
    typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;
    #define countof(a)      (sizeof(a) / sizeof(*(a)))
    /* 获取缓冲区的长度 */
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
    //OSTimeDly ( 10, OS_OPT_TIME_DLY, & err ); //不断阻塞该任务
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

    //OSTimeDly ( 10, OS_OPT_TIME_DLY, & err ); // 延时
    /* 获取 SPI Flash ID */
    OS_CRITICAL_ENTER();                 //进入临界段，不希望下面串口打印遭到中断
    FlashID = SPI_FLASH_ReadID();
    printf("FlashID is 0x%X\r\n", FlashID);

    /* 检验 SPI Flash ID */
    if (FlashID == sFLASH_ID)
    {
        SPI_FLASH_UnprotectSector(FLASH_SectorToErase);  // 可以不需要此行
        printf("检测到串行flash AT26F004 !\r\n");

        /* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
        // 这里擦除4K，即一个扇区，擦除的最小单位是扇区
        SPI_FLASH_SectorErase(FLASH_SectorToErase);
        //SPI_FLASH_ChipErase();

        /* 将发送缓冲区的数据写到flash中 */
        // 这里写一页，一页的大小为256个字节

        SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);

        printf("写入的数据为:%s\r\n", Tx_Buffer);
        //OSTimeDly ( 10, OS_OPT_TIME_DLY, & err ); // 延时
        /* 将刚刚写入的数据读出来放到接收缓冲区中 */
        SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);
        printf("读出的数据为:%s\r\n", Rx_Buffer);
        /* 检查写入的数据与读出的数据是否相等 */
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
    {                            //任务体，通常写成一个死循环
       //macLED2_TOGGLE ();
       /*
        OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //不断阻塞该任务

        dacSet(DATA_anjianquka,SOUND_LENGTH_anjianquka);
        OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );

        dacSet(DATA_xiexie,SOUND_LENGTH_xiexie);
        OSTimeDly ( 2500, OS_OPT_TIME_DLY, & err );
       */
       /* 阻塞任务，等待任务消息 */
       /*
       pcMsg = OSTaskQPend ((OS_TICK        )0,                    //无期限等待
                            (OS_OPT         )OS_OPT_PEND_BLOCKING, //没有消息就阻塞任务
                            (OS_MSG_SIZE   *)&msg_size,            //返回消息长度
                            (CPU_TS        *)&ts,                  //返回消息被发布的时间戳
                            (OS_ERR        *)&err);                //返回错误类型
       */
       macLED1_TOGGLE ();

       OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err );
    }
}



void  AppTaskCanFrame ( void * p_arg )
{
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                     //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                     //，开中断时将该值还原.
    OS_ERR      err;
    OS_MSG_SIZE    msg_size;
    CPU_INT08U * pMsg = NULL;
    CanRxMsg *ptRxMessage = NULL;                // can数据接收缓存

    while (DEF_TRUE)
    {                            //任务体，通常写成一个死循环

        //OS_CRITICAL_ENTER();                 //进入临界段，不希望下面串口打印遭到中断
        /* 请求消息队列 queue 的消息 */
        ptRxMessage = OSQPend ((OS_Q         *)&queue_can,            //消息变量指针
                        (OS_TICK       )10,                    //等待时长
                        (OS_OPT        )OS_OPT_PEND_BLOCKING,  //如果没有获取到信号量就等待
                        (OS_MSG_SIZE  *)&msg_size,             //获取消息的字节大小
                        (CPU_TS       *)0,                     //获取任务发送时的时间戳
                        (OS_ERR       *)&err);                 //返回错误
        //OS_CRITICAL_EXIT();
        if(ptRxMessage != NULL)
        {
            AnalyzeCANFrame((void *)ptRxMessage);
            // printf ("%s",(char *)ptRxMessage);
        }
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err ); //不断阻塞该任务
    }
}

void  AppTaskUartFrame ( void * p_arg )
{
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                     //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                     //，开中断时将该值还原.
    OS_ERR      err;
    OS_MSG_SIZE    msg_size;
    CPU_INT08U * pMsg = NULL;
    //CPU_INT08U ucaMsg[30] = "0aiwesky uC/OS-III";

    OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //等待1S
    OS_CRITICAL_ENTER();                 //进入临界段，不希望下面串口打印遭到中断
    DEBUG_printf ("%s\r\n","你好,欢迎使用乐为电子板卡系统");

    printf("%s", (char *)&g_tCardMechinePowerOnFrame);      // 上电信息

    OS_CRITICAL_EXIT();

    while (DEF_TRUE)
    {                            //任务体，通常写成一个死循环
        pMsg = OSQPend ((OS_Q         *)&queue_uart,            //消息变量指针
                        (OS_TICK       )10,                     //等待时长
                        (OS_OPT        )OS_OPT_PEND_BLOCKING,   //如果没有获取到信号量就等待
                        (OS_MSG_SIZE  *)&msg_size,              //获取消息的字节大小
                        (CPU_TS       *)0,                      //获取任务发送时的时间戳
                        (OS_ERR       *)&err);                  //返回错误

        //macLED2_TOGGLE ();
        if (pMsg != NULL)
        {
            AnalyzeUartFrame((void *)pMsg);
        }
        if(pMsg != NULL)
        {
            //OLED_ShowStr(0,0,pMsg,1);
        }
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}


void  AppTaskKeyScan ( void * p_arg )
{
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                     //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                     //，开中断时将该值还原.
    OS_ERR      err;
    OS_MSG_SIZE    msg_size;
    //OSTimeDly ( 1000, OS_OPT_TIME_DLY, & err ); //等待1S
    //OS_CRITICAL_ENTER ();                 //进入临界段，不希望下面串口打印遭到中断
	 //macLED1_TOGGLE ();
    //OS_CRITICAL_EXIT ();

    while (DEF_TRUE)
    {                            //任务体，通常写成一个死循环


        //macLED2_TOGGLE ();
        //macLED3_TOGGLE ();


        matrix_update_key();
        OSTimeDly ( 20, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}
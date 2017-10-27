#include <includes.h>
#include "frame.h"
#include "WAV_C_xiexie.h"
#include "WAV_C_quka.h"

unsigned int g_uiaInitCardCount[5]    = {9999, 9998, 9997, 9996, 9995};    // 卡初始设置值,[0]为总卡数量,发1张卡,减1,[1~4]为每个卡机初始卡数量,发1张卡,减1.
unsigned int g_uiaSpitCardCount[5]    = {0, 0, 0, 0, 0};    // 出卡数量,[0]为出卡总数量,发1张卡,加1,[1~4]为每个卡机发卡数量,发1张卡,加1.


CPU_INT08U g_ucSerNum = '0';  // 帧序号   全局

RSCTL_FREME g_tP_RsctlFrame = {'<','0','0','>'};        // 正应答帧
RSCTL_FREME g_tN_sctlFrame =  {'<','1','0','>'};        // 负应答帧

/* 卡机上电信息(41H)帧          4字节 */
CARD_MACHINE_POWER_ON_FREME      g_tCardMechinePowerOnFrame = {'<', '0', CARD_MACHINE_POWER_ON, '>'};;

/* 状态信息(42H)帧             30字节 */
CARD_MACHINE_STATUES_FRAME       g_tCardMechineStatusFrame =    {'<', '0', 'B', '1','3',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '>'};

/* 已出卡信息(43H)帧            6字节 */
CARD_MECHINE_TO_PC_FRAME         g_tCardSpitOutFrame = {'<', '0', CARD_SPIT_OUT, '1', '1', '>'};

/* 按钮取卡信息(44H)帧          6字节 */
CARD_MECHINE_TO_PC_FRAME        g_tCardKeyPressFrame = {'<', '0', CARD_KEY_PRESS, '1', '1', '>'};

/* 卡被取走信息(45H)帧          6字节 */
CARD_MECHINE_TO_PC_FRAME         g_tCardTakeAwayFrame = {'<', '0', CARD_TAKE_AWAY, '1', '1', '>'};

/* 上报卡夹号编号信息(46H)帧   36字节 */
CARD_REPORT_SPIT_STATUES_FRAME   g_tCardReportSpitStatusFrame = {'<', '0', CARD_REPORT_SPIT_STATUES, '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0', '>'};



/* 初始化卡机信息(61H)帧       20字节 */
PC_TO_CARD_INIT_FREME            g_tPcToCardInitFrame = {'<', '0', PC_INIT_MECHINE, '9', '9', '9', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0','>'};

/* 出卡信息(62H)帧              5字节 */
PC_TO_CARD_MECHINE_FRAME         g_tPcSpitOutCardFrame = {'<', '0', PC_SPIT_OUT_CARD, '1', '>'};

/* 坏卡信息(63H)帧              5字节 */
PC_TO_CARD_MECHINE_FRAME         g_tPcBadCardFrame = {'<', '0', PC_BAD_CARD, '1', '>'};

/* 查询卡机状态(65H)帧          5字节 */
PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardMechineFrame = {'<', '0', PC_QUERY_CARD_MECHINE, '1', '>'};

 /* 查询卡夹(66H)帧              5字节 */
PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardCpipFrame = {'<', '0', PC_QUERY_CARD_CLIP, '1', '>'};

/* 设置卡夹卡数(67H)帧          8字节*/
PC_SET_CARD_NUM_FRAME            g_tPcSetCardNumFrame = {'<', '0', PC_SET_CARD_NUM, '1', '9', '9' , '9', '>'};





//#pragma  diag_suppress 870          // 不显示警告

const Print_msg g_taPri_msg[] = {
                            {'$',                           "/* 无效信息 */"},
                            {CARD_MACHINE_POWER_ON,         "/* 卡机上电信息(41H)帧          4字节 */"},
                            {CARD_MACHINE_STATUES,          "/* 状态信息(42H)帧             30字节 */"},
                            {CARD_SPIT_OUT,                 "/* 已出卡信息(43H)帧            6字节 */"},
                            {CARD_KEY_PRESS,                "/* 按钮取卡信息(44H)帧          6字节 */"},
                            {CARD_TAKE_AWAY,                "/* 卡被取走信息(45H)帧          6字节 */"},
                            {CARD_REPORT_SPIT_STATUES,      "/* 上报卡夹号编号信息(46H)帧   36字节 */"},

                            {PC_INIT_MECHINE,               "/* 初始化卡机信息(61H)帧       20字节 */"},
                            {PC_SPIT_OUT_CARD,              "/* 出卡信息(62H)帧              5字节 */"},
                            {PC_BAD_CARD,                   "/* 坏卡信息(63H)帧              5字节 */"},
                            {PC_QUERY_CARD_MECHINE,         "/* 查询卡机状态(65H)帧          5字节 */"},
                            {PC_QUERY_CARD_CLIP,            "/* 查询卡夹(66H)帧              5字节 */"},
                            {PC_SET_CARD_NUM,               "/* 设置卡夹卡数(67H)帧          8字节 */"},
                            {PC_GET_DIST,                   "/* 测距(74H)帧                  5字节 */"},
                            {'0',NULL}
                        };

const Print_msg g_taShow_msg[] = {
                            {0,                             "NULL"},
                            {MACHINE_CHECK_CARD,            "验卡"},
                            {KEY_PRESS,                     "按键"},
                            {CARD_SPIT_NOTICE,              "出卡"},
                            {CARD_TAKE_AWAY_NOTICE,         "取卡"},
                            {CARD_IS_READY,                 "卡就绪"},
                            {0xfe,                          "      "},
                            {0xff,NULL}
                        };

const Print_msg g_taShowStatus_msg[] = {
                            {0,                             "NULL"},
                            {CARD_IS_OK,                    "好卡"},
                            {CARD_IS_BAD,                   "坏卡"},
                            {IS_WORKING,                    "工作"},
                            {IS_BACKING,                    "备用"},
                            {0xfe,                          "    "},
                            {0xff,NULL}
                        };
const Print_msg g_taShowFaultCode_msg[] = {
                            {0,                             "NULL"},
                            {CARD_IS_OK,                    "好卡"},
                            {CARD_IS_BAD,                   "坏卡"},
                            {0xfe,                          "    "},
                            {0xff,NULL}
                        };

// 复制要显示的菜单数据到数组中
void copyMenu (CPU_INT08U num, CPU_INT08U cmd, CPU_INT08U values, CPU_INT08U addr, CPU_INT08U count)
{
    CPU_INT08U *str_id = CheckShowMsg(cmd);
    unsigned char i, n;
    n = check_menu (DLG_STATUS);
    for (i = addr; i < 16; i++)
    {
        g_dlg[n].MsgRow[num - 1][i] = ' ';
    }
    for (i = 0; i < count; i++)
    {
        g_dlg[n].MsgRow[num - 1][i + addr] = str_id[i];
    }
    g_ucIsUpdateMenu = 1;      // 更新界面
}

// 复制要显示的菜单数据到数组中
void copyStatusMsg (CPU_INT08U num, CPU_INT08U cmd, CPU_INT08U values, CPU_INT08U addr, CPU_INT08U count)
{
    CPU_INT08U *str_id = CheckShowStatusMsg(cmd);
    unsigned char i, n;
    //strcpy() = CheckShowMsg(id);
    n = check_menu (DLG_STATUS);
    for (i = 0; i < count; i++)
    {
        g_dlg[n].MsgRow[num - 1][i + addr] = str_id[i];
    }
}

// 复制要显示的菜单数据到数组中
void copyFaultMsg (CPU_INT08U num, CPU_INT08U cmd, CPU_INT08U values, CPU_INT08U addr, CPU_INT08U count)
{
    CPU_INT08U *str_id = CheckShowStatusMsg(cmd);
    unsigned char i, n;
    //strcpy() = CheckShowMsg(id);
    n = check_menu (DLG_FAULT_CODE);
    for (i = 0; i < count; i++)
    {
        g_dlg[n].MsgRow[num - 1][i + addr] = str_id[i];
    }
}

// 找到打印的字符串，并返回其首地址
CPU_INT08U * CheckShowFaultCode (CPU_INT08U ch)
{
    CPU_INT08U i = 0;
    for (i = 0; i < (sizeof (g_taShowFaultCode_msg) / sizeof (g_taShowFaultCode_msg[0])); i++)
    {
        if(g_taShowFaultCode_msg[i].CTL == ch)
        {
            return (CPU_INT08U *)g_taShowFaultCode_msg[i].Msg;
        }
    }
    return (CPU_INT08U *)g_taShowFaultCode_msg[0].Msg;
}

// 找到打印的字符串，并返回其首地址
CPU_INT08U * CheckShowStatusMsg (CPU_INT08U ch)
{
    CPU_INT08U i = 0;
    for (i = 0; i < (sizeof (g_taShowStatus_msg) / sizeof (g_taShowStatus_msg[0])); i++)
    {
        if(g_taShowStatus_msg[i].CTL == ch)
        {
            return (CPU_INT08U *)g_taShowStatus_msg[i].Msg;
        }
    }
    return (CPU_INT08U *)g_taShowStatus_msg[0].Msg;
}

// 找到打印的字符串，并返回其首地址
CPU_INT08U * CheckShowMsg (CPU_INT08U ch)
{
    CPU_INT08U i = 0;
    for (i = 0; i < (sizeof (g_taShow_msg) / sizeof (g_taShow_msg[0])); i++)
    {
        if(g_taShow_msg[i].CTL == ch)
        {
            return (CPU_INT08U *)g_taShow_msg[i].Msg;
        }
    }
    return (CPU_INT08U *)g_taShow_msg[0].Msg;
}

// 找到打印的字符串，并返回其首地址
CPU_INT08U * CheckPriMsg (CPU_INT08U ch)
{
    CPU_INT08U i = 0;
    for (i = 0; i < (sizeof (g_taPri_msg) / sizeof (g_taPri_msg[0])); i++)
    {
        if(g_taPri_msg[i].CTL == ch)
        {
             return (CPU_INT08U *)g_taPri_msg[i].Msg;
         }
     }
     return (CPU_INT08U *)g_taPri_msg[0].Msg;
 }

// CAN总线数据处理
 CPU_INT08U  AnalyzeCANFrame ( CanRxMsg arg )
 {
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                             //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                             //，开中断时将该值还原。

    CanRxMsg mtRxMessage = arg;                       // can数据接收缓存
    OS_ERR      err;
    unsigned char i, n;
    unsigned char ID_temp = 0;
    unsigned char str_id[10] = {0};
    static unsigned char count = 0;
    g_uiSerNum = mtRxMessage.Data[0];                               // 保持帧序号不变,将数据回复


    switch(mtRxMessage.Data[3])
    {
        case KEY_PRESS:                                 // 司机已按键
            if ( g_ucaDeviceIsSTBY[0] == 1 && g_ucaDeviceIsSTBY[1] == 1 && g_ucaDeviceIsSTBY[2] == 1 && g_ucaDeviceIsSTBY[3] == 1 || mtRxMessage.Data[2] == 0xff )
            {
                if ( mtRxMessage.Data[4] == 0x10 ) // 未进入发卡流程,且有卡
                {
                    DEBUG_printf ( "%s\r\n", ( char * ) CheckPriMsg ( CARD_KEY_PRESS ) );
                    printf ( "%s\n", ( char * ) &g_tCardKeyPressFrame );
                    g_ucaDeviceIsSTBY[mtRxMessage.Data[1] -1] = 0; // 按键发卡流程开始之后，再次按键不再响应
                    myCANTransmit ( &gt_TxMessage, mtRxMessage.Data[1], 0, WRITE_CARD_STATUS, CARD_IS_OK, 0, 0, NO_FAIL );
                    copyMenu ( mtRxMessage.Data[1], KEY_PRESS, 0, 7, 4 );
                    OSTimeDly ( 5, OS_OPT_TIME_DLY, &err );
                }
                else
                {
                    myCANTransmit ( &gt_TxMessage, mtRxMessage.Data[1], 0, WRITE_CARD_STATUS, 0x10, 0, 0, NO_FAIL );
                    g_ucaDeviceIsSTBY[mtRxMessage.Data[1] -1] = 1;
                }
            }
            else
            {
                myCANTransmit ( &gt_TxMessage, mtRxMessage.Data[1], 0, WRITE_CARD_STATUS, 0x10, 0, 0, NO_FAIL );
            }

            break;
        case CARD_SPIT_NOTICE:                          // 出卡通知
            myCANTransmit ( &gt_TxMessage, mtRxMessage.Data[1], 0, CARD_SPIT_NOTICE_ACK, 0, 0, 0, NO_FAIL );
            dacSet ( DATA_quka, SOUND_LENGTH_quka );
            copyMenu ( mtRxMessage.Data[1], CARD_SPIT_NOTICE, 0, 7, 4 );
            copyStatusMsg ( mtRxMessage.Data[1], 0xfe, 0, 12, 4 ); //
            break;
        case CARD_TAKE_AWAY_NOTICE:                     // 卡已被取走通知
            g_ucaDeviceIsSTBY[mtRxMessage.Data[1] -1] = 1;  // 表明卡已经被取走,置位状态
            myCANTransmit ( &gt_TxMessage, mtRxMessage.Data[1], 0, CARD_TAKE_AWAY_NOTICE_ACK, 0, 0, 0, NO_FAIL );

            switch ( mtRxMessage.Data[1] )
            {
                case 1:
                    if ( g_ucaFaultCode[1] == 0 )
                    {
                        g_ucUpWorkingID     = 2;
                        g_ucUpBackingID     = 1;
                        myCANTransmit ( &gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                        myCANTransmit ( &gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                    }
                    break;
                case 2:
                    if ( g_ucaFaultCode[0] == 0 )
                    {
                        g_ucUpWorkingID     = 1;
                        g_ucUpBackingID     = 2;
                        myCANTransmit ( &gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                        myCANTransmit ( &gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                    }
                    break;
                case 3:
                    if ( g_ucaFaultCode[3] == 0 )
                    {
                        g_ucDownWorkingID   = 4;
                        g_ucDownBackingID   = 3;
                        myCANTransmit ( &gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                        myCANTransmit ( &gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                    }
                    break;
                case 4:
                    if ( g_ucaFaultCode[2] == 0 )
                    {
                        g_ucDownWorkingID   = 3;
                        g_ucDownBackingID   = 4;
                        myCANTransmit ( &gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                        myCANTransmit ( &gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                    }
                    break;
                default:
                    break;
            }

            DEBUG_printf ( "%s\r\n", ( char * ) CheckPriMsg ( CARD_TAKE_AWAY ) );
            printf ( "%s\n", ( char * ) &g_tCardTakeAwayFrame );
            dacSet ( DATA_xiexie, SOUND_LENGTH_xiexie );
            copyMenu ( mtRxMessage.Data[1], CARD_TAKE_AWAY_NOTICE, 0, 7, 4 );
            copyStatusMsg ( mtRxMessage.Data[1], 0xfe, 0, 12, 4 );

            g_ucIsUpdateMenu    = 1;                    // 更新界面
            break;
        case CARD_IS_READY:                             // 卡就绪
            myCANTransmit ( &gt_TxMessage, mtRxMessage.Data[1], 0, CARD_READY_ACK, 0, 0, 0, NO_FAIL );
            copyMenu ( mtRxMessage.Data[1], CARD_IS_READY, 0, 7, 6 );
            copyStatusMsg ( mtRxMessage.Data[1], 0xfe, 0, 13, 3 );
            break;
        case MECHINE_WARNING:                           // 报警
            myCANTransmit ( &gt_TxMessage, mtRxMessage.Data[1], 0, FAULT_CODE_ACK, 0, 0, 0, NO_FAIL ); // 回复故障码
            g_ucaDeviceIsSTBY[0] = 1;                       // 表明卡已经被取走,置位状态
            g_ucaDeviceIsSTBY[1] = 1;                       // 表明卡已经被取走,置位状态
            g_ucaDeviceIsSTBY[2] = 1;                       // 表明卡已经被取走,置位状态
            g_ucaDeviceIsSTBY[3] = 1;                       // 表明卡已经被取走,置位状态

            if ( ( mtRxMessage.Data[2] != 0xff ) && ( mtRxMessage.Data[4] == 0x21 ) && ( mtRxMessage.Data[7] <= FAULT_CODE11 ) )
            {
                switch ( mtRxMessage.Data[1] )
                {
                    case 1:
                        if ( g_ucaFaultCode[1] == 0 )
                        {
                            g_ucUpWorkingID     = 2;
                            g_ucUpBackingID     = 1;
                            myCANTransmit ( &gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                            myCANTransmit ( &gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                        }
                        break;
                    case 2:
                        if ( g_ucaFaultCode[0] == 0 )
                        {
                            g_ucUpWorkingID     = 1;
                            g_ucUpBackingID     = 2;
                            myCANTransmit ( &gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                            myCANTransmit ( &gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                        }
                        break;
                    case 3:
                        if ( g_ucaFaultCode[3] == 0 )
                        {
                            g_ucDownWorkingID   = 4;
                            g_ucDownBackingID   = 3;
                            myCANTransmit ( &gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                            myCANTransmit ( &gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                        }
                        break;
                    case 4:
                        if ( g_ucaFaultCode[2] == 0 )
                        {
                            g_ucDownWorkingID   = 3;
                            g_ucDownBackingID   = 4;
                            myCANTransmit ( &gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                            myCANTransmit ( &gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                        }
                        break;
                    default:
                        break;
                }
                g_ucaFaultCode[mtRxMessage.Data[1] -1] = mtRxMessage.Data[7]; // 报告故障码
            }
            else if ( mtRxMessage.Data[7] > FAULT_CODE11 )
            {
                g_ucaFaultCode[mtRxMessage.Data[1] -1] = FAULT_CODE11 + 1; // 报告故障码
            }
            g_ucIsUpdateMenu    = 1;                        // 更新界面
            break;
        case CYCLE_ACK:
            break;
        case CARD_MACHINE_INIT_ACK:
            break;
        default:
            break;
    }
    //OS_CRITICAL_EXIT();
    return 0;
}

CPU_INT08U  AnalyzeUartFrame ( CPU_INT08U argv[] , OS_MSG_SIZE size)
{
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                     //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                     //，开中断时将该值还原.

    OS_ERR      err;
    CPU_INT08U ucaFrame[50] = {0};
    CPU_INT08U ucSerNum = 0;
    CPU_INT08U ucNum = argv[1];
    CPU_INT08U type_frame = argv[2];
    //strcpy (ucaFrame, argv);
    if (POSITIVE_ACK == type_frame)    // 正应答帧
    {
        return 0;
    }
    else if (NAGATIVE_ACK == type_frame)    // 负应答帧
    {

    }
    else if (PC_INIT_MECHINE <= type_frame <= PC_SET_CARD_NUM)  // 检测数据合法性
    {
        g_tP_RsctlFrame.RSCTL = ucNum;
        OS_CRITICAL_ENTER();                 // 进入临界段，不希望下面语句遭到中断
        printf("%s",(char *)&g_tP_RsctlFrame);   //发送正应答帧
        DEBUG_printf ("%s\r\n",(char *)CheckPriMsg(type_frame));
        //printf ("%s\r\n","收到信息");
        switch(type_frame)
        {
            case PC_INIT_MECHINE:               /* 初始化卡机信息(61H)帧 */
                OLED_ShowStr(0,0,argv,1);
                display_GB2312_string (0, 2, "初始化", 0);
                break;
            case PC_SPIT_OUT_CARD:              /* 出卡信息(62H)帧 */
                OLED_ShowStr(0,0,argv,1);
                display_GB2312_string (0, 2, "出卡信息", 0);
                if ( argv[3] )
                {
                    myCANTransmit ( &gt_TxMessage, argv[3] - '0', 0, WRITE_CARD_STATUS, CARD_IS_OK, 0, 0, NO_FAIL );
                    dacSet ( DATA_quka, SOUND_LENGTH_quka );
                    copyMenu ( argv[3] - '0', CARD_SPIT_NOTICE, 0, 7, 4 );
                    copyStatusMsg ( argv[3] - '0', 0xfe, 0, 12, 4 ); //
                }
                break;
            case PC_BAD_CARD:                  /* 坏卡信息(63H)帧 */
                OLED_ShowStr(0,0,argv,1);
                display_GB2312_string (0, 2, "坏卡", 0);
                if ( argv[3] )
                {
                    myCANTransmit ( &gt_TxMessage, argv[3] - '0', 0, WRITE_CARD_STATUS, CARD_IS_BAD, 0, 0, NO_FAIL );
                }
                break;
            case PC_QUERY_CARD_MECHINE:         /* 查询卡机状态(65H)帧 */
                OLED_ShowStr(0,0,argv,1);
                display_GB2312_string (0, 2, "查询卡机", 0);
                break;
            case PC_QUERY_CARD_CLIP:
                OLED_ShowStr(0,0,argv,1);   /* 查询卡夹(66H)帧 */
                display_GB2312_string (0, 2, "查询卡夹", 0);
                break;
            case PC_SET_CARD_NUM:
                OLED_ShowStr(0,0,argv,1);   /* 设置卡夹卡数(67H)帧 */
                display_GB2312_string (0, 2, "设置卡夹", 0);
                break;
            case PC_GET_DIST:
                //OLED_ShowStr(0,0,argv,1);   /* 测距帧 */
                //display_GB2312_string (0, 2, "测距", 0);
                break;
            case PC_CAR_HAS_COMING:
                OLED_ShowStr(0,0,argv,1);   /* 车以来 */
                display_GB2312_string (0, 2, "车已来", 0);
                break;
            case PC_CAR_HAS_GONE:
                OLED_ShowStr(0,0,argv,1);   /* 车以走 */
                display_GB2312_string (0, 2, "车已走", 0);
                break;
            default:
                OLED_ShowStr(0,0,argv,1);   /* 无效信息 */
                display_GB2312_string (0, 2, "无效信息", 0);
                break;
        }
        OS_CRITICAL_EXIT();
    }
    ucSerNum = (g_ucSerNum++) % 10 + '0';
    return 0;
}

#include <includes.h>
#include "frame.h"


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
                            {'0',NULL}
                        };

Dlg g_dlg[] = {
                        {LOGO_ID,           "    ****电子    ", " www.*****.com  ", "   ****发卡机   ", "   版本: V1.0   "},
                        {STATUS_ID,         "1:              ", "2:              ", "3:              ", "4:              "},
                        {MAIN_ID,           "    进入状态    ", "1.输入IC卡数量  ", "2.调看开机状态  ", "3.调机运行状态  "},
                        {CARD_NUM_ID,       "卡数1:          ", "卡数2:          ", "卡数3:          ", "卡数4:          "},
                        {CARD_MIAN_ID,      "卡机状态:1号卡机", "         2号卡机", "         3号卡机", "         4号卡机"},
                        {MECHINE_STATUS_ID, "卡机状态:(     )", "出好卡数量:     ", "回收坏卡数:     ", "故障次数:       "},
                        
                        {DEBUG_MANI_ID,     "1:              ", "2:              ", "3:              ", "4:              "},
                        {DEBUG_ONE_ID,      "                ", "1.连续循环出卡  ", "2.循环出一张卡  ", "3.翻一张好卡    "},
                        {DEBUG_TWO_ID,      "                ", "4.读取送卡传感器", "5.读取卡盒传感器", "6.读取取卡传感器"},
                        
                        {255,               "                ", "                ", "                ", "                "},
                     };





// 找到打印的字符串，并返回其首地址
CPU_INT08U * check_msg(CPU_INT08U ch)
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



CPU_INT08U  AnalyzeCANFrame ( void * p_arg )
{
    CanRxMsg *pRxMessage = (CanRxMsg *)p_arg;                // can数据接收缓存

    switch(pRxMessage->Data[3])
    {
        case 0x01:
            //OLED_ShowStr(0,0,p_arg,1);
        break;
        case 0x02:

        break;
        case 0x03:
            //OLED_ShowStr(0,0,p_arg,1);
            DEBUG_printf ("%s\r\n",(char *)check_msg(CARD_KEY_PRESS));
            printf ("%s\n",(char *)&g_tCardKeyPressFrame);
            //OLED_ShowStr(0,0,p_arg,1);
        break;
        case 0x04:
            //OLED_ShowStr(0,0,p_arg,1);   /* 查询卡夹(66H)帧 */
        break;

        case PC_SET_CARD_NUM:
            //OLED_ShowStr(0,0,p_arg,1);   /* 设置卡夹卡数(67H)帧 */
        break;
        default:
            OLED_ShowStr(0,0,p_arg,1);
        break;
    }
    return 0;
}

CPU_INT08U  AnalyzeUartFrame ( void * p_arg )
{
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                     //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                     //，开中断时将该值还原.
    //OS_ERR      err;
    CPU_INT08U ucSerNum = 0;
    CPU_INT08U ucNum = *((CPU_INT08U *)p_arg + 1);
    CPU_INT08U type_frame = *((CPU_INT08U *)p_arg + 2);

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
        OS_CRITICAL_ENTER();                 //进入临界段，不希望下面串口打印遭到中断
        printf("%s",(char *)&g_tP_RsctlFrame);   //发送正应答帧
        DEBUG_printf ("%s\r\n",(char *)check_msg(type_frame));
        //printf ("%s\r\n","收到信息");
        OS_CRITICAL_EXIT();
        switch(type_frame)
        {
            case PC_INIT_MECHINE:               /* 初始化卡机信息(61H)帧 */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "初始化");
            break;
            case PC_SPIT_OUT_CARD:              /* 出卡信息(62H)帧 */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "出卡信息");
            break;
            case PC_BAD_CARD:                  /* 坏卡信息(63H)帧 */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "坏卡");
            break;
            case PC_QUERY_CARD_MECHINE:         /* 查询卡机状态(65H)帧 */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "查询卡机");
            break;
            case PC_QUERY_CARD_CLIP:
                OLED_ShowStr(0,0,p_arg,1);   /* 查询卡夹(66H)帧 */
                display_GB2312_string (0, 2, "查询卡夹");
            break;
            case PC_SET_CARD_NUM:
                OLED_ShowStr(0,0,p_arg,1);   /* 设置卡夹卡数(67H)帧 */
                display_GB2312_string (0, 2, "设置卡夹");
            break;
            default:
                display_GB2312_string (0, 2, "无效信息");
                printf("错误信息帧！\n");
            break;
        }
    }


    ucSerNum = (g_ucSerNum++) % 10 + '0';
    return 0;
}

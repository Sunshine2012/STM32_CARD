#include <includes.h>
#include "frame.h"

RSCTL_FREME gt_P_RsctlFrame = {'<','0','0','>'};        // 正应答帧
RSCTL_FREME gt_N_sctlFrame =  {'<','1','0','>'};        // 负应答帧
PC_TO_CARD_INIT_FREME t_PcToCardInitFrame;

//#pragma  diag_suppress 870          // 不显示警告

Print_msg gta_pri_msg[16] = {
                            {'<',                           "/* 无效信息*/"},
                            {CARD_MACHINE_POWER_ON,         "/* 卡机上电信息(41H)帧        4字节 */"},
                            {CARD_MACHINE_STATUES,          "/* 状态信息(42H)帧           30字节 */"},
                            {CARD_SPIT_OUT_CARD,            "/* 已出卡信息(43H)帧          6字节 */"},
                            {CARD_KEY_PRESS,                "/* 按钮取卡信息(44H)帧        6字节 */"},
                            {CARD_TAKE_AWAY,                "/* 卡被取走信息(45H)帧        6字节 */"},
                            {CARD_REPORT_STATUES,           "/* 上报卡夹号信息(46H)帧     29字节 */"},

                            {PC_INIT_MECHINE,               "/* 初始化卡机信息(61H)帧     20字节 */"},
                            {PC_SPIT_OUT_CARD,              "/* 出卡信息(62H)帧            5字节 */"},
                            {PC_CARD_FALSE,                 "/* 坏卡信息(63H)帧            5字节 */"},
                            {PC_QUERY_CARD_MECHINE,         "/* 查询卡机状态(65H)帧        5字节 */"},
                            {PC_QUERY_CARD_CLIP,            "/* 查询卡夹(66H)帧            5字节 */"},
                            {PC_SET_CARD_NUM,               "/* 设置卡夹卡数(67H)帧        8字节 */"},
                            {'0',NULL}
                        };

// 找到打印的字符串，并返回其首地址
char * check_msg(CPU_INT08U ch)
{
    CPU_INT08U i = 0;
    for (i = 0; i < (sizeof (gta_pri_msg) / sizeof (gta_pri_msg[0])); i++)
    {
        if(gta_pri_msg[i].CTL == ch)
        {
            return (char *)gta_pri_msg[i].Msg;
        }
    }
    return (char *)gta_pri_msg[0].Msg;
}



CPU_INT08U  AnalyzeCANFrame ( CPU_INT08U * p_arg )
{
    //OS_ERR      err;
    //OS_MSG_SIZE    msg_size;
    //CPU_TS         ts;
    //char * pMsg = NULL;
    //USART4_SendString();
    return 0;
}

CPU_INT08U  AnalyzeUartFrame ( CPU_INT08U * p_arg )
{
    CPU_SR_ALLOC();      //使用到临界段（在关/开中断时）时必需该宏，该宏声明和定义一个局部变
                                     //量，用于保存关中断前的 CPU 状态寄存器 SR（临界段关中断只需保存SR）
                                     //，开中断时将该值还原.
    //OS_ERR      err;
    CPU_INT08U ucSerNum = 0;
    CPU_INT08U ucNum = *(p_arg + 1);
    CPU_INT08U type_frame = *(p_arg + 2);

    if (POSITIVE_ACK == type_frame)    // 正应答帧
    {
        return 0;
    }
    else if (NAGATIVE_ACK == type_frame)    // 负应答帧
    {

    }
    else if (PC_INIT_MECHINE <= type_frame <= PC_SET_CARD_NUM)  // 检测数据合法性
    {
        gt_P_RsctlFrame.RSCTL = ucNum;
        OS_CRITICAL_ENTER();                 //进入临界段，不希望下面串口打印遭到中断
        USART4_SendString(mac4USART,(char *)&gt_P_RsctlFrame);   //发送正应答帧
        DEBUG_printf ("%s\r\n",(char *)check_msg(type_frame));
        //printf ("%s\r\n","收到信息");
        OS_CRITICAL_EXIT();
        switch(type_frame)
        {
            case PC_INIT_MECHINE:               /* 初始化卡机信息(61H)帧 */
                OLED_ShowStr(0,0,p_arg,1);
            break;
            case PC_SPIT_OUT_CARD:              /* 出卡信息(62H)帧 */
                OLED_ShowStr(0,0,p_arg,1);
            break;
            case PC_CARD_FALSE:                 /* 坏卡信息(63H)帧 */
                OLED_ShowStr(0,0,p_arg,1);
            break;
            case PC_QUERY_CARD_MECHINE:         /* 查询卡机状态(65H)帧 */
                OLED_ShowStr(0,0,p_arg,1);
            break;
            case PC_QUERY_CARD_CLIP:
                OLED_ShowStr(0,0,p_arg,1);      /* 查询卡夹(66H)帧 */
            break;
            case PC_SET_CARD_NUM:
                OLED_ShowStr(0,0,p_arg,1);      /* 设置卡夹卡数(67H)帧 */
            break;
            default:
            break;
        }
    }


    ucSerNum = (g_ucSerNum++) % 10 + '0';
    return 0;
}

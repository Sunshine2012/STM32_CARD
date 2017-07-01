#include <includes.h>
#include "frame.h"

RSCTL_FREME gt_P_RsctlFrame = {'<','0','0','>'};        // ��Ӧ��֡
RSCTL_FREME gt_N_sctlFrame =  {'<','1','0','>'};        // ��Ӧ��֡
PC_TO_CARD_INIT_FREME t_PcToCardInitFrame;

//#pragma  diag_suppress 870          // ����ʾ����

Print_msg gta_pri_msg[16] = {
                            {'<',                           "/* ��Ч��Ϣ*/"},
                            {CARD_MACHINE_POWER_ON,         "/* �����ϵ���Ϣ(41H)֡        4�ֽ� */"},
                            {CARD_MACHINE_STATUES,          "/* ״̬��Ϣ(42H)֡           30�ֽ� */"},
                            {CARD_SPIT_OUT_CARD,            "/* �ѳ�����Ϣ(43H)֡          6�ֽ� */"},
                            {CARD_KEY_PRESS,                "/* ��ťȡ����Ϣ(44H)֡        6�ֽ� */"},
                            {CARD_TAKE_AWAY,                "/* ����ȡ����Ϣ(45H)֡        6�ֽ� */"},
                            {CARD_REPORT_STATUES,           "/* �ϱ����к���Ϣ(46H)֡     29�ֽ� */"},

                            {PC_INIT_MECHINE,               "/* ��ʼ��������Ϣ(61H)֡     20�ֽ� */"},
                            {PC_SPIT_OUT_CARD,              "/* ������Ϣ(62H)֡            5�ֽ� */"},
                            {PC_CARD_FALSE,                 "/* ������Ϣ(63H)֡            5�ֽ� */"},
                            {PC_QUERY_CARD_MECHINE,         "/* ��ѯ����״̬(65H)֡        5�ֽ� */"},
                            {PC_QUERY_CARD_CLIP,            "/* ��ѯ����(66H)֡            5�ֽ� */"},
                            {PC_SET_CARD_NUM,               "/* ���ÿ��п���(67H)֡        8�ֽ� */"},
                            {'0',NULL}
                        };

// �ҵ���ӡ���ַ��������������׵�ַ
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
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                     //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                     //�����ж�ʱ����ֵ��ԭ.
    //OS_ERR      err;
    CPU_INT08U ucSerNum = 0;
    CPU_INT08U ucNum = *(p_arg + 1);
    CPU_INT08U type_frame = *(p_arg + 2);

    if (POSITIVE_ACK == type_frame)    // ��Ӧ��֡
    {
        return 0;
    }
    else if (NAGATIVE_ACK == type_frame)    // ��Ӧ��֡
    {

    }
    else if (PC_INIT_MECHINE <= type_frame <= PC_SET_CARD_NUM)  // ������ݺϷ���
    {
        gt_P_RsctlFrame.RSCTL = ucNum;
        OS_CRITICAL_ENTER();                 //�����ٽ�Σ���ϣ�����洮�ڴ�ӡ�⵽�ж�
        USART4_SendString(mac4USART,(char *)&gt_P_RsctlFrame);   //������Ӧ��֡
        DEBUG_printf ("%s\r\n",(char *)check_msg(type_frame));
        //printf ("%s\r\n","�յ���Ϣ");
        OS_CRITICAL_EXIT();
        switch(type_frame)
        {
            case PC_INIT_MECHINE:               /* ��ʼ��������Ϣ(61H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
            break;
            case PC_SPIT_OUT_CARD:              /* ������Ϣ(62H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
            break;
            case PC_CARD_FALSE:                 /* ������Ϣ(63H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
            break;
            case PC_QUERY_CARD_MECHINE:         /* ��ѯ����״̬(65H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
            break;
            case PC_QUERY_CARD_CLIP:
                OLED_ShowStr(0,0,p_arg,1);      /* ��ѯ����(66H)֡ */
            break;
            case PC_SET_CARD_NUM:
                OLED_ShowStr(0,0,p_arg,1);      /* ���ÿ��п���(67H)֡ */
            break;
            default:
            break;
        }
    }


    ucSerNum = (g_ucSerNum++) % 10 + '0';
    return 0;
}

#include <includes.h>
#include "frame.h"


CPU_INT08U g_ucSerNum = '0';  // ֡���   ȫ��

RSCTL_FREME g_tP_RsctlFrame = {'<','0','0','>'};        // ��Ӧ��֡
RSCTL_FREME g_tN_sctlFrame =  {'<','1','0','>'};        // ��Ӧ��֡

/* �����ϵ���Ϣ(41H)֡          4�ֽ� */
CARD_MACHINE_POWER_ON_FREME      g_tCardMechinePowerOnFrame = {'<', '0', CARD_MACHINE_POWER_ON, '>'};;

/* ״̬��Ϣ(42H)֡             30�ֽ� */
CARD_MACHINE_STATUES_FRAME       g_tCardMechineStatusFrame =    {'<', '0', 'B', '1','3',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '>'};

/* �ѳ�����Ϣ(43H)֡            6�ֽ� */
CARD_MECHINE_TO_PC_FRAME         g_tCardSpitOutFrame = {'<', '0', CARD_SPIT_OUT, '1', '1', '>'};

/* ��ťȡ����Ϣ(44H)֡          6�ֽ� */
CARD_MECHINE_TO_PC_FRAME        g_tCardKeyPressFrame = {'<', '0', CARD_KEY_PRESS, '1', '1', '>'};

/* ����ȡ����Ϣ(45H)֡          6�ֽ� */
CARD_MECHINE_TO_PC_FRAME         g_tCardTakeAwayFrame = {'<', '0', CARD_TAKE_AWAY, '1', '1', '>'};

/* �ϱ����кű����Ϣ(46H)֡   36�ֽ� */
CARD_REPORT_SPIT_STATUES_FRAME   g_tCardReportSpitStatusFrame = {'<', '0', CARD_REPORT_SPIT_STATUES, '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0', '>'};



/* ��ʼ��������Ϣ(61H)֡       20�ֽ� */
PC_TO_CARD_INIT_FREME            g_tPcToCardInitFrame = {'<', '0', PC_INIT_MECHINE, '9', '9', '9', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0','>'};

/* ������Ϣ(62H)֡              5�ֽ� */
PC_TO_CARD_MECHINE_FRAME         g_tPcSpitOutCardFrame = {'<', '0', PC_SPIT_OUT_CARD, '1', '>'};

/* ������Ϣ(63H)֡              5�ֽ� */
PC_TO_CARD_MECHINE_FRAME         g_tPcBadCardFrame = {'<', '0', PC_BAD_CARD, '1', '>'};

/* ��ѯ����״̬(65H)֡          5�ֽ� */
PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardMechineFrame = {'<', '0', PC_QUERY_CARD_MECHINE, '1', '>'};

 /* ��ѯ����(66H)֡              5�ֽ� */
PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardCpipFrame = {'<', '0', PC_QUERY_CARD_CLIP, '1', '>'};

/* ���ÿ��п���(67H)֡          8�ֽ�*/
PC_SET_CARD_NUM_FRAME            g_tPcSetCardNumFrame = {'<', '0', PC_SET_CARD_NUM, '1', '9', '9' , '9', '>'};





//#pragma  diag_suppress 870          // ����ʾ����

const Print_msg g_taPri_msg[] = {
                            {'$',                           "/* ��Ч��Ϣ */"},
                            {CARD_MACHINE_POWER_ON,         "/* �����ϵ���Ϣ(41H)֡          4�ֽ� */"},
                            {CARD_MACHINE_STATUES,          "/* ״̬��Ϣ(42H)֡             30�ֽ� */"},
                            {CARD_SPIT_OUT,                 "/* �ѳ�����Ϣ(43H)֡            6�ֽ� */"},
                            {CARD_KEY_PRESS,                "/* ��ťȡ����Ϣ(44H)֡          6�ֽ� */"},
                            {CARD_TAKE_AWAY,                "/* ����ȡ����Ϣ(45H)֡          6�ֽ� */"},
                            {CARD_REPORT_SPIT_STATUES,      "/* �ϱ����кű����Ϣ(46H)֡   36�ֽ� */"},

                            {PC_INIT_MECHINE,               "/* ��ʼ��������Ϣ(61H)֡       20�ֽ� */"},
                            {PC_SPIT_OUT_CARD,              "/* ������Ϣ(62H)֡              5�ֽ� */"},
                            {PC_BAD_CARD,                   "/* ������Ϣ(63H)֡              5�ֽ� */"},
                            {PC_QUERY_CARD_MECHINE,         "/* ��ѯ����״̬(65H)֡          5�ֽ� */"},
                            {PC_QUERY_CARD_CLIP,            "/* ��ѯ����(66H)֡              5�ֽ� */"},
                            {PC_SET_CARD_NUM,               "/* ���ÿ��п���(67H)֡          8�ֽ� */"},
                            {'0',NULL}
                        };

Dlg g_dlg[] = {
                        {LOGO_ID,           "    ****����    ", " www.*****.com  ", "   ****������   ", "   �汾: V1.0   "},
                        {STATUS_ID,         "1:              ", "2:              ", "3:              ", "4:              "},
                        {MAIN_ID,           "    ����״̬    ", "1.����IC������  ", "2.��������״̬  ", "3.��������״̬  "},
                        {CARD_NUM_ID,       "����1:          ", "����2:          ", "����3:          ", "����4:          "},
                        {CARD_MIAN_ID,      "����״̬:1�ſ���", "         2�ſ���", "         3�ſ���", "         4�ſ���"},
                        {MECHINE_STATUS_ID, "����״̬:(     )", "���ÿ�����:     ", "���ջ�����:     ", "���ϴ���:       "},
                        
                        {DEBUG_MANI_ID,     "1:              ", "2:              ", "3:              ", "4:              "},
                        {DEBUG_ONE_ID,      "                ", "1.����ѭ������  ", "2.ѭ����һ�ſ�  ", "3.��һ�źÿ�    "},
                        {DEBUG_TWO_ID,      "                ", "4.��ȡ�Ϳ�������", "5.��ȡ���д�����", "6.��ȡȡ��������"},
                        
                        {255,               "                ", "                ", "                ", "                "},
                     };





// �ҵ���ӡ���ַ��������������׵�ַ
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
    CanRxMsg *pRxMessage = (CanRxMsg *)p_arg;                // can���ݽ��ջ���

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
            //OLED_ShowStr(0,0,p_arg,1);   /* ��ѯ����(66H)֡ */
        break;

        case PC_SET_CARD_NUM:
            //OLED_ShowStr(0,0,p_arg,1);   /* ���ÿ��п���(67H)֡ */
        break;
        default:
            OLED_ShowStr(0,0,p_arg,1);
        break;
    }
    return 0;
}

CPU_INT08U  AnalyzeUartFrame ( void * p_arg )
{
    CPU_SR_ALLOC();      //ʹ�õ��ٽ�Σ��ڹ�/���ж�ʱ��ʱ����ú꣬�ú������Ͷ���һ���ֲ���
                                     //�������ڱ�����ж�ǰ�� CPU ״̬�Ĵ��� SR���ٽ�ι��ж�ֻ�豣��SR��
                                     //�����ж�ʱ����ֵ��ԭ.
    //OS_ERR      err;
    CPU_INT08U ucSerNum = 0;
    CPU_INT08U ucNum = *((CPU_INT08U *)p_arg + 1);
    CPU_INT08U type_frame = *((CPU_INT08U *)p_arg + 2);

    if (POSITIVE_ACK == type_frame)    // ��Ӧ��֡
    {
        return 0;
    }
    else if (NAGATIVE_ACK == type_frame)    // ��Ӧ��֡
    {

    }
    else if (PC_INIT_MECHINE <= type_frame <= PC_SET_CARD_NUM)  // ������ݺϷ���
    {
        g_tP_RsctlFrame.RSCTL = ucNum;
        OS_CRITICAL_ENTER();                 //�����ٽ�Σ���ϣ�����洮�ڴ�ӡ�⵽�ж�
        printf("%s",(char *)&g_tP_RsctlFrame);   //������Ӧ��֡
        DEBUG_printf ("%s\r\n",(char *)check_msg(type_frame));
        //printf ("%s\r\n","�յ���Ϣ");
        OS_CRITICAL_EXIT();
        switch(type_frame)
        {
            case PC_INIT_MECHINE:               /* ��ʼ��������Ϣ(61H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "��ʼ��");
            break;
            case PC_SPIT_OUT_CARD:              /* ������Ϣ(62H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "������Ϣ");
            break;
            case PC_BAD_CARD:                  /* ������Ϣ(63H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "����");
            break;
            case PC_QUERY_CARD_MECHINE:         /* ��ѯ����״̬(65H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "��ѯ����");
            break;
            case PC_QUERY_CARD_CLIP:
                OLED_ShowStr(0,0,p_arg,1);   /* ��ѯ����(66H)֡ */
                display_GB2312_string (0, 2, "��ѯ����");
            break;
            case PC_SET_CARD_NUM:
                OLED_ShowStr(0,0,p_arg,1);   /* ���ÿ��п���(67H)֡ */
                display_GB2312_string (0, 2, "���ÿ���");
            break;
            default:
                display_GB2312_string (0, 2, "��Ч��Ϣ");
                printf("������Ϣ֡��\n");
            break;
        }
    }


    ucSerNum = (g_ucSerNum++) % 10 + '0';
    return 0;
}

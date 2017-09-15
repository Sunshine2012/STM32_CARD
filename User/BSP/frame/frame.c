#include <includes.h>
#include "frame.h"
#include "WAV_C_xiexie.h"
#include "WAV_C_quka.h"

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

const Print_msg g_taShow_msg[] = {
                            {0,                             "NULL"},
                            {MACHINE_CHECK_CARD,            "�鿨"},
                            {KEY_PRESS,                     "����"},
                            {CARD_SPIT_NOTICE,              "����"},
                            {CARD_TAKE_AWAY_NOTICE,         "ȡ��"},
                            {CARD_IS_READY,                 "������"},
                            {0xfe,                          "      "},
                            {0xff,NULL}
                        };

const Print_msg g_taShowStatus_msg[] = {
                            {0,                             "NULL"},
                            {CARD_IS_OK,                    "�ÿ�"},
                            {CARD_IS_BAD,                   "����"},
                            {0xfe,                          "    "},
                            {0xff,NULL}
                        };

// ����Ҫ��ʾ�Ĳ˵����ݵ�������
void copyMenu (CPU_INT08U num, CPU_INT08U cmd, CPU_INT08U values, CPU_INT08U addr, CPU_INT08U count)
{
    CPU_INT08U *str_id = CheckShowMsg(cmd);
    unsigned char i, n;
    n = check_menu (DLG_STATUS);
    for (i = 0; i < count; i++)
    {
        g_dlg[n].MsgRow[num - 1][i + addr] = str_id[i];
    }
    g_ucIsUpdateMenu = 1;      // ���½���
}

// ����Ҫ��ʾ�Ĳ˵����ݵ�������
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

// �ҵ���ӡ���ַ��������������׵�ַ
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

// �ҵ���ӡ���ַ��������������׵�ַ
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

// �ҵ���ӡ���ַ��������������׵�ַ
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



CPU_INT08U  AnalyzeCANFrame ( void * p_arg )
{
    CanRxMsg *pRxMessage = (CanRxMsg *)p_arg;                // can���ݽ��ջ���
    OS_ERR      err;
    unsigned char i, n;
    unsigned char str_id[10] = {0};
    static unsigned char count = 1;

    switch(pRxMessage->Data[3])
    {
        case MACHINE_CHECK_CARD:    // ָ����λ�鿨
            DEBUG_printf ("%s\r\n",(char *)CheckPriMsg(CARD_KEY_PRESS));
            myCANTransmit(&gt_TxMessage, pRxMessage->Data[1], pRxMessage->Data[1], MACHINE_STATUES, (count) % 10 ? CARD_IS_OK : CARD_IS_BAD, 0, 0, NO_FAIL);
            printf ("%s\n",(char *)&g_tCardKeyPressFrame);
            copyStatusMsg (pRxMessage->Data[1], (count++) % 10 ? CARD_IS_OK : CARD_IS_BAD, 0, 12, 4);
            copyMenu (pRxMessage->Data[1], MACHINE_CHECK_CARD, 0, 8, 4);
            break;
        case KEY_PRESS:             // ˾���Ѱ���
            DEBUG_printf ("%s\r\n",(char *)CheckPriMsg(CARD_KEY_PRESS));
            myCANTransmit(&gt_TxMessage, pRxMessage->Data[1], pRxMessage->Data[1], WRITE_CARD_STATUS, CARD_IS_OK, 0, 0, NO_FAIL);
            printf ("%s\n",(char *)&g_tCardKeyPressFrame);
            break;
        case CARD_SPIT_NOTICE:      // ����֪ͨ
            dacSet(DATA_quka,SOUND_LENGTH_quka);
            copyStatusMsg (pRxMessage->Data[1], 0xfe, 0, 12, 4);
            copyMenu (pRxMessage->Data[1], CARD_SPIT_NOTICE, 0, 8, 4);
            //OSTimeDly ( 2000, OS_OPT_TIME_DLY, & err );
            break;
        case CARD_TAKE_AWAY_NOTICE: // ���ѱ�ȡ��֪ͨ
            dacSet(DATA_xiexie,SOUND_LENGTH_xiexie);
            copyStatusMsg (pRxMessage->Data[1], 0xfe, 0, 12, 4);
            copyMenu (pRxMessage->Data[1], CARD_TAKE_AWAY_NOTICE, 0, 8, 4);
            //OSTimeDly ( 2500, OS_OPT_TIME_DLY, & err );
            break;
        case CARD_IS_READY:
            copyStatusMsg (pRxMessage->Data[1], 0xfe, 0, 14, 2);
            copyMenu (pRxMessage->Data[1], CARD_IS_READY, 0, 8, 6);
            break;
        case SERCH_CARD_MECHINE_ACK:// ��ѯ�����Ļظ�
            g_usCurID = pRxMessage->Data[5] << 8 | pRxMessage->Data[6];
            sprintf(str_id,"%x  ",g_usCurID);
            n = check_menu (DLG_CARD_ID);
            for (i = 0; i < 6; i++)
            {
                g_dlg[n].MsgRow[1][i + 10] = str_id[i];
            }
            g_ucIsUpdateMenu = 1;      // ���½���
            break;
        case MECHINE_WARNING:    // ����
            if (pRxMessage->Data[1] < 2) // �������Ϲ�λ����
            {
                g_usUpWorkingID = pRxMessage->Data[1] == 1 ? 2 : 1;
                g_usUpBackingID = pRxMessage->Data[1] == 2 ? 1 : 2;
            }
            else
            {
                g_usDownWorkingID = pRxMessage->Data[1] == 3 ? 4 : 3;
                g_usDownBackingID = pRxMessage->Data[1] == 4 ? 3 : 4;
            }
            myCANTransmit(&gt_TxMessage, (unsigned char)(g_usUpWorkingID | 0x000f), 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
            myCANTransmit(&gt_TxMessage, (unsigned char)(g_usUpWorkingID | 0x000f), 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬
            g_ucIsUpdateMenu = 1;      // ���½���
            break;
        default:
            //OLED_ShowStr(0,0,p_arg,1);
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
        DEBUG_printf ("%s\r\n",(char *)CheckPriMsg(type_frame));
        //printf ("%s\r\n","�յ���Ϣ");
        OS_CRITICAL_EXIT();
        switch(type_frame)
        {
            case PC_INIT_MECHINE:               /* ��ʼ��������Ϣ(61H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "��ʼ��", 0);
            break;
            case PC_SPIT_OUT_CARD:              /* ������Ϣ(62H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "������Ϣ", 0);
            break;
            case PC_BAD_CARD:                  /* ������Ϣ(63H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "����", 0);
            break;
            case PC_QUERY_CARD_MECHINE:         /* ��ѯ����״̬(65H)֡ */
                OLED_ShowStr(0,0,p_arg,1);
                display_GB2312_string (0, 2, "��ѯ����", 0);
            break;
            case PC_QUERY_CARD_CLIP:
                OLED_ShowStr(0,0,p_arg,1);   /* ��ѯ����(66H)֡ */
                display_GB2312_string (0, 2, "��ѯ����", 0);
            break;
            case PC_SET_CARD_NUM:
                OLED_ShowStr(0,0,p_arg,1);   /* ���ÿ��п���(67H)֡ */
                display_GB2312_string (0, 2, "���ÿ���", 0);
            break;
            default:
                display_GB2312_string (0, 2, "��Ч��Ϣ", 0);
                printf("������Ϣ֡��\n");
            break;
        }
    }


    ucSerNum = (g_ucSerNum++) % 10 + '0';
    return 0;
}

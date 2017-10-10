#include <includes.h>
#include "frame.h"
#include "WAV_C_xiexie.h"
#include "WAV_C_quka.h"

unsigned char  g_ucDeviceIsReady    = 1;                  // �����������ڴ���״̬��,��������,�����յ�����������Ϣ,��ʱֻ����������,���ֻ�յ�һ��������Ϣ,��ֱ�ӷ���

unsigned short g_usaInitCardCount[5]    = {9999, 9998, 9997, 9996, 9995};    // ����ʼ����ֵ,[0]Ϊ�ܿ�����,��1�ſ�,��1,[1~4]Ϊÿ��������ʼ������,��1�ſ�,��1.
unsigned short g_usaSpitCardCount[5]    = {0, 0, 0, 0, 0};    // ��������,[0]Ϊ����������,��1�ſ�,��1,[1~4]Ϊÿ��������������,��1�ſ�,��1.


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
                            {IS_WORKING,                    "����"},
                            {IS_BACKING,                    "����"},
                            {0xfe,                          "    "},
                            {0xff,NULL}
                        };
const Print_msg g_taShowFaultCode_msg[] = {
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
    for (i = addr; i < 16; i++)
    {
        g_dlg[n].MsgRow[num - 1][i] = ' ';
    }
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

// ����Ҫ��ʾ�Ĳ˵����ݵ�������
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

// �ҵ���ӡ���ַ��������������׵�ַ
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
            printf ("%s\n",(char *)&g_tCardKeyPressFrame);
            copyMenu (pRxMessage->Data[1], MACHINE_CHECK_CARD, 0, 7, 4);
            copyStatusMsg (pRxMessage->Data[1], (count++) % 10 ? CARD_IS_OK : CARD_IS_BAD, 0, 12, 4);
            if (!(count++) % 10)
            {
                g_ucDeviceIsReady = 1;          // ���Ϊ�������������־���ȴ��ٴ��ϱ�������Ϣ
            }
            g_uiSerNum = pRxMessage->Data[0];                               // ����֡��Ų���,�����ݻظ�
            myCANTransmit(&gt_TxMessage, pRxMessage->Data[1], pRxMessage->Data[2], MACHINE_STATUES, (count) % 10 ? CARD_IS_OK : CARD_IS_BAD, 0, 0, NO_FAIL);
            break;
        case KEY_PRESS:             // ˾���Ѱ���
            if (g_ucDeviceIsReady != 1)     // �����û�б�ȡ�ߣ���������Ӧ,ֱ���˳�
            {
                u8 TransmitMailbox;
                CanTxMsg TxMessage;
                g_uiSerNum = pRxMessage->Data[0];
                memset(&gt_TxMessage,0,sizeof (CanTxMsg));

                gt_TxMessage.StdId = 0x00;
                gt_TxMessage.RTR = CAN_RTR_DATA;
                gt_TxMessage.IDE = CAN_ID_EXT;;
                gt_TxMessage.DLC = 8;
                gt_TxMessage.Data[0] = pRxMessage->Data[0];

                gt_TxMessage.Data[2] = 0;
                gt_TxMessage.Data[3] = WRITE_CARD_STATUS;
                gt_TxMessage.Data[4] = 0;
                gt_TxMessage.Data[5] = 0;
                gt_TxMessage.Data[6] = 0;
                gt_TxMessage.Data[7] = 0;


                if(pRxMessage->Data[1] == 0x01)
                {
                    gt_TxMessage.ExtId = 0x7811;
                    gt_TxMessage.Data[1] = 0x02;
                }
                else if (pRxMessage->Data[1] == 0x02)
                {
                    gt_TxMessage.ExtId = 0x7812;
                    gt_TxMessage.Data[1] = 0x01;
                }
                else if (pRxMessage->Data[1] == 0x03)
                {
                    gt_TxMessage.ExtId = 0x7813;
                    gt_TxMessage.Data[1] = 0x04;
                }
                else if (pRxMessage->Data[1] == 0x04)
                {
                    gt_TxMessage.ExtId = 0x7814;
                    gt_TxMessage.Data[1] = 0x03;
                }

                TransmitMailbox = CAN_Transmit(CAN1,&gt_TxMessage);
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
                g_uiSerNum++;                               // ����֡��Ų���,�����ݻظ�


                return 0;
            }
            g_ucDeviceIsReady = 0;      // �����������̿�ʼ֮���ٴΰ���������Ӧ
            //GENERAL_TIM_Init ();
            DEBUG_printf ("%s\r\n",(char *)CheckPriMsg(CARD_KEY_PRESS));
            if (pRxMessage->Data[2] == 1)
            {
                switch (pRxMessage->Data[1])    // ����������״̬
                {
                    case 1:
                        copyStatusMsg (1, IS_WORKING, 0, 2, 4);
                        copyStatusMsg (2, IS_BACKING, 0, 2, 4);
                        g_usUpWorkingID = 0x7811;
                        g_usUpBackingID = 0x7812;
                        break;
                    case 2:
                        copyStatusMsg (1, IS_BACKING, 0, 2, 4);
                        copyStatusMsg (2, IS_WORKING, 0, 2, 4);
                        g_usUpWorkingID = 0x7812;
                        g_usUpBackingID = 0x7811;
                        break;
                    case 3:
                        copyStatusMsg (3, IS_WORKING, 0, 2, 4);
                        copyStatusMsg (4, IS_BACKING, 0, 2, 4);
                        g_usDownWorkingID = 0x7813;
                        g_usDownBackingID = 0x7814;
                        break;
                    case 4:
                        copyStatusMsg (3, IS_BACKING, 0, 2, 4);
                        copyStatusMsg (4, IS_WORKING, 0, 2, 4);
                        g_usDownWorkingID = 0x7814;
                        g_usDownBackingID = 0x7813;
                        break;
                    default:
                        break;

                }

            }
            else if (pRxMessage->Data[2] == 2)
            {
                switch (pRxMessage->Data[1])    // ����������״̬
                {
                    case 1:
                        copyStatusMsg (1, IS_BACKING, 0, 2, 4);
                        copyStatusMsg (2, IS_WORKING, 0, 2, 4);
                        g_usUpWorkingID = 0x7812;
                        g_usUpBackingID = 0x7811;
                        break;
                    case 2:
                        copyStatusMsg (1, IS_WORKING, 0, 2, 4);
                        copyStatusMsg (2, IS_BACKING, 0, 2, 4);
                        g_usUpWorkingID = 0x7811;
                        g_usUpBackingID = 0x7812;
                        break;
                    case 3:
                        copyStatusMsg (3, IS_BACKING, 0, 2, 4);
                        copyStatusMsg (4, IS_WORKING, 0, 2, 4);
                        g_usDownWorkingID = 0x7814;
                        g_usDownBackingID = 0x7813;
                        break;
                    case 4:
                        copyStatusMsg (3, IS_WORKING, 0, 2, 4);
                        copyStatusMsg (4, IS_BACKING, 0, 2, 4);
                        g_usDownWorkingID = 0x7813;
                        g_usDownBackingID = 0x7814;
                        break;
                    default:
                        break;
                }

            }

            if (pRxMessage->Data[4] == 0x10) // ����豸Ϊ����̬���п�
            {
                g_uiSerNum = pRxMessage->Data[0];                               // ����֡��Ų���,�����ݻظ�
                myCANTransmit(&gt_TxMessage, pRxMessage->Data[1], pRxMessage->Data[2], WRITE_CARD_STATUS, CARD_IS_OK, 0, 0, NO_FAIL);
                copyMenu (pRxMessage->Data[1], KEY_PRESS, 0, 7, 4);
                // copyStatusMsg (pRxMessage->Data[1], (count++) % 10 ? CARD_IS_OK : CARD_IS_BAD, 0, 12, 4);
            }
            printf ("%s\n",(char *)&g_tCardKeyPressFrame);
            break;
        case CARD_SPIT_NOTICE:      // ����֪ͨ
            dacSet(DATA_quka,SOUND_LENGTH_quka);
            copyMenu (pRxMessage->Data[1], CARD_SPIT_NOTICE, 0, 7, 4);
            copyStatusMsg (pRxMessage->Data[1], 0xfe, 0, 12, 4);            //
            g_uiSerNum = pRxMessage->Data[0];                               // ����֡��Ų���,�����ݻظ�
            myCANTransmit(&gt_TxMessage, pRxMessage->Data[1], 0, CARD_SPIT_NOTICE_ACK, 0, 0, 0, NO_FAIL);
            break;
        case CARD_TAKE_AWAY_NOTICE: // ���ѱ�ȡ��֪ͨ
            dacSet(DATA_xiexie,SOUND_LENGTH_xiexie);
            copyMenu (pRxMessage->Data[1], CARD_TAKE_AWAY_NOTICE, 0, 7, 4);
            copyStatusMsg (pRxMessage->Data[1], 0xfe, 0, 12, 4);
            g_uiSerNum = pRxMessage->Data[0];                               // ����֡��Ų���,�����ݻظ�
            myCANTransmit(&gt_TxMessage, pRxMessage->Data[1], 0, CARD_TAKE_AWAY_NOTICE_ACK, 0, 0, 0, NO_FAIL);
            g_ucDeviceIsReady = 1;      // �������Ѿ���ȡ��,��λ״̬
            break;
        case CARD_IS_READY:
            copyMenu (pRxMessage->Data[1], CARD_IS_READY, 0, 7, 6);
            copyStatusMsg (pRxMessage->Data[1], 0xfe, 0, 13, 3);
            break;
        case MECHINE_WARNING:    // ����
            if (pRxMessage->Data[1] <= 2) // �������Ϲ�λ����
            {
                g_usUpWorkingID = pRxMessage->Data[1] == 1 ? 0x7812 : 0x7811;
                g_usUpBackingID = pRxMessage->Data[1] == 1 ? 0x7811 : 0x7812;
                myCANTransmit(&gt_TxMessage, (unsigned char)(g_usUpWorkingID & 0x000f), 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
                myCANTransmit(&gt_TxMessage, (unsigned char)(g_usUpBackingID & 0x000f), 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬
            }
            else
            {
                g_usDownWorkingID = pRxMessage->Data[1] == 3 ? 0x7814 : 0x7813;
                g_usDownBackingID = pRxMessage->Data[1] == 3 ? 0x7813 : 0x7814;
                myCANTransmit(&gt_TxMessage, (unsigned char)(g_usDownWorkingID & 0x000f), 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
                myCANTransmit(&gt_TxMessage, (unsigned char)(g_usDownBackingID & 0x000f), 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬

            }
            if(pRxMessage->Data[4] == 0x11)   // �޿�����
            {
                g_ucaFaultCode[pRxMessage->Data[1] - 1][0] = FAULT_NO_CARD;
            }
            else if (pRxMessage->Data[4] == 0x21)
            {
                if(pRxMessage->Data[7] <= FAULT_CODE0C)
                {
                    g_ucaFaultCode[pRxMessage->Data[1] - 1][0] = pRxMessage->Data[7];
                }
                else if (pRxMessage->Data[7] == 0x11)
                {
                    g_ucaFaultCode[pRxMessage->Data[1] - 1][0] = FAULT_CODE0E;
                }
            }
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

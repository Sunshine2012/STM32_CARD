#include <includes.h>
#include "menu.h"

unsigned char g_ucCurDlg;           // ��ǰ��ʾ�Ĳ˵�ID

Dlg g_dlg[] = {
                        {DLG_LOGO,           "    ****����    ", " www.*****.com  ", "   ****������   ", "   �汾: V1.0   "},
                        {DLG_STATUS,         "1:              ", "2:              ", "3:              ", "4:              "},
                        {DLG_MAIN,           "     ���˵�     ", "1.���ÿ��估ID  ", "2.��������״̬  ", "3.��������״̬  "},
                        {DLG_CARD_ID,        "   ���ÿ���ID   ", "������:         ", "ͨ��ID��:       ", "��������        "},
                        {DLG_CARD_MAIN,      "����״̬:1�ſ���", "         2�ſ���", "         3�ſ���", "         4�ſ���"},
                        {DLG_MECHINE_STATUS, "����״̬:(     )", "���ÿ�����:     ", "���ջ�����:     ", "���ϴ���:       "},

                        {DLG_DEBUG_MAIN,     "1:              ", "2:              ", "3:              ", "4:              "},
                        {DLG_DEBUG_ONE,      "    �ſ�������  ", "1.����ѭ������  ", "2.ѭ����һ�ſ�  ", "3.��һ�źÿ�    "},
                        {DLG_DEBUG_TWO,      "4.��ȡ�Ϳ�������", "5.��ȡ���д�����", "6.��ȡȡ��������", "                "},

                        {255,                "                ", "                ", "                ", "                "},
                     };
// �ҵ���ʾ�Ĳ˵�ID�����������������е�����
unsigned char check_menu(unsigned char ch)
{
    unsigned char i = 0;
    for (i = 0; i < (sizeof (g_dlg) / sizeof (g_dlg[0])); i++)
    {
        if(g_dlg[i].ID == ch)
        {
            return i;
        }
    }
    return 255;
}

// ��ʾ״̬�˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ
void doShowStatusMenu (unsigned char dlg_id, unsigned char isNotRow)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
}

// ��ʾ�˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ
void doShowMainMenu (unsigned char dlg_id, unsigned char isNotRow)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        g_ucKeyValues = KEY_NUL;
        switch (key)
        {
            case KEY_ENTRY:
                switch (isNotRow)
                {
                    case 0:
                        isNotRow++;     // ֻ���з��Բ���,��
                    case 1:
                        doShowIdSetMenu (DLG_CARD_ID, 1);
                        break;
                    case 2:
                        break;
                    case 3:
                        doShowDebugOne (DLG_DEBUG_ONE,0);
                        break;
                    default:
                        break;
                }
                break;
            case KEY_CANCEL:    // �˳�
            case KEY_QUIT:
                g_ucCurDlg = DLG_STATUS;
                g_ucKeyValues = 0xfe;
                return;
                break;
            case KEY_UP:
                if (1 < isNotRow)
                {
                    isNotRow--;
                }
                break;
            case KEY_DOWN:
                if (3 > isNotRow)
                {
                    isNotRow++;
                }
                break;
            case KEY_LEFT:
                break;
            case KEY_RIGHT:
                break;
            case KEY_OK:
                break;

            break;
            default:
                break;
        }
        if (KEY_NUL != key)
        {
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        OSTimeDly ( 5, OS_OPT_TIME_DLY, & err );     //��������������
    }
}


// ID���ò˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ
void doShowIdSetMenu (unsigned char dlg_id, unsigned char isNotRow)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = 1;      // ������
    unsigned char str_num[10] = {0};
    unsigned char str_id[10] = {0};
    unsigned char id_h = 0;          // CANͨ�ŵ�ID���ֽ�
    unsigned char id_l = 0;          // CANͨ�ŵ�ID���ֽ�
    unsigned short id = 0x7811;      // CANͨ�ŵ�ID
    sprintf(str_num,"0%d      ",num);
    sprintf(str_id,"%x   ",id);
    for (i = 0; i < 6; i++)
    {
        g_dlg[dlgId].MsgRow[1][i + 10] = str_num[i];
    }
    for (i = 0; i < 6; i++)
    {
        g_dlg[dlgId].MsgRow[2][i + 10] = str_id[i];
    }

    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        g_ucKeyValues = KEY_NUL;
        switch (key)
        {
            case KEY_ENTRY:
                if (1 <= num <=4 && 0x7811 <= id <= 0x7814)
                {
                    id_h = ( id >> 8 ) & 0xff;
                    id_l = id & 0xff;
                    MyCANTransmit(&gt_TxMessage, 0, 0, SET_MECHINE_ID, num, id_h, id_l, NO_FAIL);
                }
                break;
            case KEY_QUIT:
                g_ucCurDlg = DLG_MAIN;
                g_ucKeyValues = 0xfe;
                return;
                break;
            case KEY_UP:
                if (1 < isNotRow)
                {
                    isNotRow--;
                }
                break;
            case KEY_DOWN:
                if (3 > isNotRow)
                {
                    isNotRow++;
                }
                break;
            case KEY_LEFT:
                switch (isNotRow)
                {
                    case 1:
                        if (1 < num)
                        {
                            num--;
                            id--;
                        }
                        break;
                    case 2:
                        if (0x7811 < id)
                        {
                            num--;
                            id--;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case KEY_RIGHT:
                switch (isNotRow)
                {
                    case 1:
                        if (4 > num)
                        {
                            num++;
                            id++;
                        }
                        break;
                    case 2:
                        if (0x7814 > id)
                        {
                            num++;
                            id++;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case KEY_OK:
                switch (isNotRow)
                {
                    case 1:
                    case 2:
                        if (1 <= num <=4 && 0x7811 <= id <= 0x7814)
                        {
                            id_h = ( id >> 8 ) & 0xff;
                            id_l = id & 0xff;
                            MyCANTransmit(&gt_TxMessage, 0, 0, SET_MECHINE_ID, num, id_h, id_l, NO_FAIL);
                        }
                        break;
                    case 3:
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;

        }
        if (KEY_NUL != key)    // ����а�������,����½���
        {
            sprintf(str_num,"0%d      ",num);
            sprintf(str_id,"%x   ",id);
            for (i = 0; i < 6; i++)
            {
                g_dlg[dlgId].MsgRow[1][i + 10] = str_num[i];
            }
            for (i = 0; i < 6; i++)
            {
                g_dlg[dlgId].MsgRow[2][i + 10] = str_id[i];
            }
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        OSTimeDly ( 5, OS_OPT_TIME_DLY, & err );     //��������������
    }
}


// ���Բ˵�1,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ
void doShowDebugOne (unsigned char dlg_id, unsigned char isNotRow)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = 1;      // ������
    unsigned char str_num[10] = {0};
    unsigned char str_id[10] = {0};
    unsigned char id_h = 0;          // CANͨ�ŵ�ID���ֽ�
    unsigned char id_l = 0;          // CANͨ�ŵ�ID���ֽ�
    unsigned short id = 0x7811;      // CANͨ�ŵ�ID
    sprintf(str_num,"0%d",num);
    for (i = 0; i < 2; i++)
    {
        g_dlg[dlgId].MsgRow[0][i + 2] = str_num[i];
    }
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        g_ucKeyValues = KEY_NUL;
        switch (key)
        {
            case KEY_ENTRY:
                /*
                if (1 <= num <=4 && 0x7811 <= id <= 0x7814)
                {
                    id_h = ( id >> 8 ) & 0xff;
                    id_l = id & 0xff;
                    MyCANTransmit(&gt_TxMessage, 0, 0, SET_MECHINE_ID, num, id_h, id_l, NO_FAIL);
                }
                */
                break;
            case KEY_QUIT:
                g_ucCurDlg = DLG_MAIN;
                g_ucKeyValues = 0xfe;
                return;
                break;
            case KEY_UP:
                if (0 < isNotRow < 4)
                {
                    isNotRow--;
                }
                break;
            case KEY_DOWN:
                if (0 < isNotRow < 4)
                {
                    isNotRow++;
                }
                break;
            case KEY_LEFT:
                if (isNotRow == 0 && 1 < num)
                {
                    num--;
                    id--;
                }
                break;
            case KEY_RIGHT:
                if (isNotRow == 0 && 4 > num)
                {
                    num++;
                    id++;
                }
                break;
            case KEY_OK:
                switch (isNotRow)
                {
                    case 1:
                            id_h = ( id >> 8 ) & 0xff;
                            id_l = id & 0xff;
                            MyCANTransmit(&gt_TxMessage, num, num, CYCLE_SPIT_CARD, num, id_h, id_l, NO_FAIL);
                        break;
                    case 2:
                            id_h = ( id >> 8 ) & 0xff;
                            id_l = id & 0xff;
                            MyCANTransmit(&gt_TxMessage, num, num, CYCLE_SPIT_ONE_CARD, num, id_h, id_l, NO_FAIL);
                        break;
                    case 3:
                            id_h = ( id >> 8 ) & 0xff;
                            id_l = id & 0xff;
                            MyCANTransmit(&gt_TxMessage, num, num, SPIT_ONE_OK_CARD, num, id_h, id_l, NO_FAIL);
                        break;
                    case 4:

                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        if (KEY_NUL != key)    // ����а�������,����½���
        {
            sprintf(str_num,"0%d",num);
            for (i = 0; i < 2; i++)
            {
                g_dlg[dlgId].MsgRow[0][i + 2] = str_num[i];
            }
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        OSTimeDly ( 5, OS_OPT_TIME_DLY, & err );     //��������������
    }
}

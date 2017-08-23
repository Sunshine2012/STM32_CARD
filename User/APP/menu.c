#include <includes.h>
#include "menu.h"

unsigned char g_ucCurDlg;                       // ��ǰ��ʾ�Ĳ˵�ID
unsigned short g_usCurID = 0x7811;              // ��ǰͨ���豸��ID



Dlg g_dlg[] = {
                        {DLG_LOGO,           "    ****����    ", " www.*****.com  ", "   ****������   ", "   �汾: V1.0   "},
                        {DLG_STATUS,         "1�Ź���         ", "2�ű���         ", "3�Ź���         ", "4�ű���         "},
                        {DLG_MAIN,           "     ���˵�     ", "1.����״̬      ", "2.��������      ", "3.��������      "},
                        {DLG_CARD_ID,        "   ���ÿ���ID   ", "��������        ", "������:         ", "ͨ��ID��:       "},
                        {DLG_CARD_MAIN,      "����״̬:1�ſ���", "         2�ſ���", "         3�ſ���", "         4�ſ���"},
                        {DLG_STATUS_ONE,     "1�ų���:        ", "2�ų���:        ", "3�ų���:        ", "4�ų���:        "},
                        {DLG_STATUS_TWO,     "1�ų�����:      ", "2�ų�����:      ", "3�ų�����:      ", "4�ų�����:      "},
                       //{DLG_STATUS_ONE,     "    ����״̬    ", "���ÿ�����:     ", "���ջ�����:     ", "���ϴ���:       "},

                        {DLG_DEBUG_MAIN,     "    �ſ�������  ", "1:��������      ", "2:��������      ", "                "},
                        {DLG_DEBUG_ONE,      "��:��һ�źÿ�    ", "��:��һ�Ż���    ", "��:��һ�ſ�      ", "��:ѭ������      "},
                        {DLG_DEBUG_TWO,      "��:����������    ", "��:����������    ", "��:����������    ", "��:����������    "},

                        //{DLG_DEBUG_ONE,      "    �ſ�������  ", "1.����ѭ������  ", "2.ѭ����һ�ſ�  ", "3.��һ�źÿ�    "},
                        //{DLG_DEBUG_TWO,      "    �ſ�������  ", "4.��һ�Ż���    ", "5.��һ�ſ�      ", "6.��ȡ�Ϳ�������"},

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

// ��ʾ״̬�˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowStatusMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
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

// ��ʾ�˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowMainMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
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
        switch (key)
        {
            case KEY_ENTRY:
                switch (isNotRow)
                {
                    case 1:
                        doShowStatusOne (DLG_STATUS_ONE, 5, NULL);
                        break;
                    case 2:
                        doShowIdSetMenu (DLG_CARD_ID, 1, NULL);
                        break;
                    case 3:
                        doShowDebugMain (DLG_DEBUG_MAIN, 0, NULL);
                        break;
                    default:
                        key = KEY_NUL;
                        break;
                }
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
            case KEY_CANCEL:    // �˳�
                return;
                break;
            default:
                key = KEY_NUL;
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,�˳�,��������ֵ
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //��������������
    }
}


// ״̬�˵�1,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowStatusOne (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = *(unsigned char *)p_parm;      // ������
    unsigned char str_num[5] = {0};
    unsigned char str_id[5] = {0};
    unsigned short id = 0x7810 | num;       // CANͨ�ŵ�ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    unsigned char id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    OLED_CLS(); // ��ʾ֮ǰ,����
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_RIGHT:
                doShowStatusTwo (DLG_STATUS_TWO, 5, NULL);
                break;
            case KEY_QUIT:
                break;
            case KEY_CANCEL:
                return;
                break;
            default:
                key = KEY_NUL;
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //��������������
    }
}


// ״̬�˵�2,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowStatusTwo (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = *(unsigned char *)p_parm;      // ������
    unsigned char str_num[5] = {0};
    unsigned char str_id[5] = {0};
    unsigned short id = 0x7810 | num;       // CANͨ�ŵ�ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    unsigned char id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    OLED_CLS(); // ��ʾ֮ǰ,����
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_LEFT:
                return;
                break;
            case KEY_QUIT:
                break;
            case KEY_CANCEL:
                return;
                break;
            default:
                key = KEY_NUL;
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //��������������
    }
}




// ID���ò˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowIdSetMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
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
        g_dlg[dlgId].MsgRow[2][i + 10] = str_num[i];
    }
    for (i = 0; i < 6; i++)
    {
        g_dlg[dlgId].MsgRow[3][i + 10] = str_id[i];
    }

    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
                /*if (1 <= num && num <= 4 && 0x7811 <= id && id <= 0x7814)
                {
                    id_h = ( g_usCurID >> 8 ) & 0xff;
                    id_l = g_usCurID & 0xff;
                    myCANTransmit(&gt_TxMessage, 0, 0, SET_MECHINE_ID, num, id_h, id_l, NO_FAIL);
                }*/
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
                    case 2:
                        if (1 < num)
                        {
                            num--;
                            id--;
                        }
                        break;
                    case 3:
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
                    case 2:
                        if (4 > num)
                        {
                            num++;
                            id++;
                        }
                        break;
                    case 3:
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
                        for (i = 0; i < 4; i++)         // ѭ��������̨����,���ĸ������л�Ӧ
                        {
                            id_h = 0x78 ;
                            id_l = 0x11 + i;
                            myCANTransmit(&gt_TxMessage, i, i, SEARCH_CARD_MECHINE, i, id_h, id_l, NO_FAIL);
                        }
                        break;
                    //case 2:
                    case 3:
                        if (1 <= num && num <=4 && 0x7811 <= id && id <= 0x7814)    // �����������Ŀ����Ŀ����ź�ID��
                        {
                            id_h = ( g_usCurID >> 8 ) & 0xff;
                            id_l = g_usCurID & 0xff;
                            myCANTransmit(&gt_TxMessage, (id_l & 0x0f), (id_l & 0x0f), SET_MECHINE_ID, num, (id >> 8) & 0xff, id & 0xff, NO_FAIL);
                        }
                        break;
                    default:
                        break;
                }
                break;
            case KEY_CANCEL:
                return;
                break;
            default:
                break;

        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;
            sprintf(str_num,"0%d      ",num);
            sprintf(str_id,"%x   ",id);
            for (i = 0; i < 6; i++)
            {
                g_dlg[dlgId].MsgRow[2][i + 10] = str_num[i];
            }
            for (i = 0; i < 6; i++)
            {
                g_dlg[dlgId].MsgRow[3][i + 10] = str_id[i];
            }
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //��������������
    }
}


// ���Բ˵���ѡ��,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowDebugMain (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = 1;      // ������
    unsigned char str_num[10] = {0};
    unsigned char str_id[10] = {0};
    unsigned short id = 0x7810 | num;       // CANͨ�ŵ�ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    unsigned char id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    sprintf(str_num,"0%d",num);
    str_num [3] = 0;
    for (i = 0; i < 2; i++)
    {
        g_dlg[dlgId].MsgRow[0][i + 2] = str_num[i];
    }
    OLED_CLS(); // ��ʾ֮ǰ,����
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
                switch (isNotRow)
                {
                    case 0:
                        //isNotRow++;     // ֻ���з��Բ���,������������
                        key = KEY_NUL;
                        break;
                    case 1:
                        myCANTransmit(&gt_TxMessage, num, num, ENTER_DEBUG, num, id_h, id_l, NO_FAIL);
                        doShowDebugOne (DLG_DEBUG_ONE, 5, &num);
                        break;
                    case 2:
                        myCANTransmit(&gt_TxMessage, num, num, ENTER_DEBUG, num, id_h, id_l, NO_FAIL);
                        doShowDebugTwo (DLG_DEBUG_TWO, 5, &num);
                        break;
                    default:
                        break;
                }
                break;
            case KEY_UP:
                if (0 < isNotRow)
                {
                    isNotRow--;
                }
                break;
            case KEY_DOWN:
                if (isNotRow < 2)
                {
                    isNotRow++;
                }
                break;
            case KEY_LEFT:
                if (isNotRow == 0 && num > 1 )
                {
                    num--;
                    //id--;
                    sprintf(str_num, "0%d", num);
                    str_num [3] = 0;
                    for (i = 0; i < 2; i++)
                    {
                        g_dlg[dlgId].MsgRow[0][i + 2] = str_num[i];
                    }
                }
                else
                {
                    key = KEY_NUL;
                }
                break;
            case KEY_RIGHT:
                if (isNotRow == 0 && num < 4)
                {
                    num++;
                    //id++;
                    sprintf(str_num, "0%d", num);
                    str_num [3] = 0;
                    for (i = 0; i < 2; i++)
                    {
                        g_dlg[dlgId].MsgRow[0][i + 2] = str_num[i];
                    }
                }
                else
                {
                    key = KEY_NUL;
                }
                break;
            case KEY_OK:
                key = KEY_NUL;
                break;
            case KEY_CANCEL:
                return;
                break;
            default:
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //��������������
    }
}



// ���Բ˵�1,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowDebugOne (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = *(unsigned char *)p_parm;      // ������
    unsigned char str_num[5] = {0};
    unsigned char str_id[5] = {0};
    unsigned short id = 0x7810 | num;       // CANͨ�ŵ�ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    unsigned char id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    OLED_CLS(); // ��ʾ֮ǰ,����
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_UP:
                isNotRow = 0;
                myCANTransmit(&gt_TxMessage, num, num, SPIT_ONE_OK_CARD, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_DOWN:
                isNotRow = 1;
                myCANTransmit(&gt_TxMessage, num, num, SPIT_ONE_BAD_CARD, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_LEFT:
                isNotRow = 2;
                myCANTransmit(&gt_TxMessage, num, num, HOOK_ONE_CARD, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_RIGHT:
                isNotRow = 3;
                myCANTransmit(&gt_TxMessage, num, num, CYCLE_SPIT_ONE_CARD, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_QUIT:
                myCANTransmit(&gt_TxMessage, num, num, QUIT_DEBUG, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_CANCEL:
                return;
                break;
            default:
                key = KEY_NUL;
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //��������������
    }
}

// ���Բ˵�2,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowDebugTwo (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = *(unsigned char *)p_parm;      // ������
    unsigned char str_num[10] = {0};
    unsigned char str_id[10] = {0};
    unsigned short id = 0x7810 | num;      // CANͨ�ŵ�ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    unsigned char id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    g_ucKeyContinu = 1;             // ���뵥��ģʽ
    OLED_CLS(); // ��ʾ֮ǰ,����
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //�����壬ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_UP:
                isNotRow = 0;
                myCANTransmit(&gt_TxMessage, num, num, SPIT_MOTOR_POSITIVE_STEP, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_DOWN:
                isNotRow = 1;
                myCANTransmit(&gt_TxMessage, num, num, SPIT_MOTOR_NEGATIVE_STEP, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_LEFT:
                isNotRow = 2;
                myCANTransmit(&gt_TxMessage, num, num, HOOK_MOTOR_POSITIVE_STEP, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_RIGHT:
                isNotRow = 3;
                myCANTransmit(&gt_TxMessage, num, num, HOOK_MOTOR_NEGATIVE_STEP, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_QUIT:
                myCANTransmit(&gt_TxMessage, num, num, QUIT_DEBUG, num, id_h, id_l, NO_FAIL);
                break;
            case KEY_CANCEL:
                g_ucKeyContinu = 0;             // �˳�����ģʽ
                return;
                break;
            default:
                g_ucKeyValues = KEY_NUL;
                break;
        }
        if (g_ucKeyContinu == 0xff)
        {
            g_ucKeyContinu = 1;             // �ɿ�����֮��,����������ֹͣ����
            myCANTransmit(&gt_TxMessage, num, num, STOP_DEBUG, num, id_h, id_l, NO_FAIL);
        }
        if (g_ucKeyValues == KEY_QUIT)      // ��QUIT��,ֱ���˵�������,����������ˢ��,�˳�,�����ְ���ֵ����
        {
            g_ucKeyContinu = 0;             // �˳�����ģʽ
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // ����а�������,����½���
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //��������������
    }
}

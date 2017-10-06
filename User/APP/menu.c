#include <includes.h>
#include "menu.h"

unsigned char  g_ucCurDlg           = 0;            // ��ǰ��ʾ�Ĳ˵�ID
unsigned short g_usCurID            = 0x7811;       // ��ǰͨ���豸��ID
unsigned short g_usUpWorkingID      = 0x7811;       // �Ϲ�λ��������ID
unsigned short g_usUpBackingID      = 0x7812;       // �Ϲ�λ���ÿ���ID
unsigned short g_usDownWorkingID    = 0x7813;       // �¹�λ��������ID
unsigned short g_usDownBackingID    = 0x7814;       // �¹�λ���ÿ���ID
unsigned char  g_ucaFaultCode[4][10]   = {0};       // �����Ƿ���δ����Ĺ���

Dlg g_dlg[] = {
                        {DLG_CLEAR_LCD,      "                ", "                ", "                ", "                "},

                        {DLG_LOGO,           "    ****����    ", " www.*****.com  ", "   ****������   ", "   �汾: V1.0   "},
                        {DLG_STATUS,         "1.����:         ", "2.����:         ", "3.����:         ", "4.����:         "},
                        {DLG_MAIN,           "1.����״̬      ", "2.��������      ", "3.��������      ", "4.��������      "},
                        {DLG_CARD_COUNT_SET, "1�Ż�����:      ", "2�Ż�����:      ", "3�Ż�����:      ", "4�Ż�����:      "},
                        {DLG_CARD_ID,        "   ���ÿ���ID   ", "��������        ", "������:         ", "ͨ��ID��:       "},
                        {DLG_WORKING_SET,    " ������������   ", "1:����   2:���� ", "3:����   4:���� ", "                "},

                        {DLG_CARD_MAIN,      "����״̬:1�ſ���", "         2�ſ���", "         3�ſ���", "         4�ſ���"},
                        {DLG_STATUS_ONE,     "1�ų���:        ", "2�ų���:        ", "3�ų���:        ", "4�ų���:        "},
                        {DLG_STATUS_TWO,     "1�ų�����:      ", "2�ų�����:      ", "3�ų�����:      ", "4�ų�����:      "},

                        {DLG_DEBUG_MAIN,     "     �ſ������� ", "1:��������      ", "2:��������      ", "                "},
                        {DLG_DEBUG_ONE,      "��:��һ�źÿ�    ", "��:��һ�Ż���    ", "��:��һ�ſ�      ", "��:ѭ������      "},
                        {DLG_DEBUG_TWO,      "��:����������    ", "��:����������    ", "��:����������    ", "��:����������    "},

                        {DLG_FAULT_CODE,     "    �ſ�������  ", "������:         ", "˵��:           ", "                "},

                     };
Dlg g_dlg_fault_code[] = {
                        {DLG_CLEAR_LCD,      "                ", "                ", "                ", "                "},

                        {FAULT_CODE01,       "    �ſ�������  ", "������:(01H)    ", "˵��: ��ʼ������", "������ܻ�λ    "},
                        {FAULT_CODE02,       "    �ſ�������  ", "������:(02H)    ", "˵��: ��ʼ������", "�����ת���ܻ�λ"},
                        {FAULT_CODE03,       "    �ſ�������  ", "������:(03H)    ", "˵��: ��ʼ������", "�����ת���ܻ�λ"},
                        {FAULT_CODE04,       "    �ſ�������  ", "������:(04H)    ", "˵��: ����ʱ����", "�����ǰ��ת    "},
                        {FAULT_CODE05,       "    �ſ�������  ", "������:(05H)    ", "˵��: ����ʱ����", "�������ת    "},
                        {FAULT_CODE06,       "    �ſ�������  ", "������:(06H)    ", "˵��: ����ʱ����", "���ǰ����ת    "},
                        {FAULT_CODE07,       "    �ſ�������  ", "������:(07H)    ", "˵��: ����ʱ����", "���ǰ���س̶�ת"},
                        {FAULT_CODE08,       "    �ſ�������  ", "������:(08H)    ", "˵��: ����ʱ����", "���������ת    "},
                        {FAULT_CODE09,       "    �ſ�������  ", "������:(09H)    ", "˵��: ����ʱ����", "��������س̶�ת"},
                        {FAULT_NO_CARD,      "    �ſ�������  ", "˵��: �������޿�", "��װ��IC��      ", "                "},
                        {FAULT_BAD_CARD,     "    �ſ�������  ", "˵��: ����������", "��������      ", "                "},
                        {255,                "    δ֪����    ", "������:(δ֪)   ", "                ", "                "},
                     };

// �ҵ���ʾ�Ĳ˵�ID,���������������е�����
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
    return 0;
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
    {                            //������,ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
                switch (isNotRow)
                {
                    case 0:
                        doShowStatusOne (DLG_STATUS_ONE, 5, NULL);
                        break;
                    case 1:
                        doShowCardCountSet (DLG_CARD_COUNT_SET, 0, NULL);
                        break;
                    case 2:
                        doShowWorkingSet (DLG_WORKING_SET, 1, NULL);
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
                if (0 < isNotRow)
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //��������������
    }
}

// ���ÿ���������,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowCardCountSet (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char isSetMode = 0;   // ���������ģʽ/
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char seek = 0;        // ָʾ��ǰ���ڵ�����λ��
    unsigned char temp = 0;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = 1;      // ������

    unsigned short id = 0x7810 | num;                       // CANͨ�ŵ�ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    unsigned char id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    unsigned char cardCountShowSeek = 10;   // �������ַ��Ӵ����еĵڼ�����ʾ,Ĭ�ϴ�11���ֽڿ�ʼ��ʾ
    unsigned short usaCardCount[4] = {1000,1001,1002,1003};     // ���п�����ʼ������������
    unsigned char str_aCardCount[4][6] = {0};                     // ���п�����ʼ��������������ʾ�ַ���
    for (i = 0; i < 4; i++)
    {
        usaCardCount[i] = g_usaInitCardCount[i + 1];    // ���Ƴ�ʼ�������������棬�Թ��޸�
    }
    for (i = 0; i < 4; i++)
    {
        sprintf(str_aCardCount[i],"%04d",usaCardCount[i]);      // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����
    }

    for (i = 0; i < 4; i++)     // ���Ƴ�ʼ���������ַ����鵽��ʾ����
    {
        for (j = 0; j < 4; j++)
        {
            g_dlg[dlgId].MsgRow[i][j + 10] = str_aCardCount[i][j];
        }
    }
    for (i = 0; i < 4; i++) // ��ʾ
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    g_ucKeyValues = KEY_NUL;
    while (DEF_TRUE)
    {                            //������,ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
                switch (isNotRow)
                {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                        isSetMode = 1;      // ��������ģʽ,��ȡ�����˳����ã��ٴΰ�ȡ�������˻���һ���˵�
                        display_GB2312_string (0, isNotRow * 2, g_dlg[dlgId].MsgRow[isNotRow], 0);
                        for (i = 0; i < 4; i++)
                        {
                            sprintf(str_aCardCount[i],"%04d",usaCardCount[i]);      // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����
                            display_GB2312_char ((cardCountShowSeek + i) * 8, isNotRow * 2, str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                        }
                        key = KEY_NUL;      // ���ٸ��½�����
                        g_ucIsUpdateMenu = 0;
                        break;
                    default:
                        break;
                }
                break;
            case KEY_UP:
                if (isSetMode)
                {
                    switch(seek)
                    {
                        case 0:
                            temp = usaCardCount[isNotRow] / 1000;
                            usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 1000 : usaCardCount[isNotRow];
                            break;
                        case 1:
                            temp = usaCardCount[isNotRow] % 1000 / 100;
                            usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 100 : usaCardCount[isNotRow];
                            break;
                        case 2:
                            temp = usaCardCount[isNotRow] % 100 / 10;
                            usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 10 : usaCardCount[isNotRow];
                            break;
                        case 3:
                            temp = usaCardCount[isNotRow] % 10;
                            usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 1 : usaCardCount[isNotRow];
                            break;
                    }
                    sprintf(str_aCardCount[isNotRow],"%04d",usaCardCount[isNotRow]);        // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����
                    for (i = 0; i < 4; i++)
                    {
                        display_GB2312_char ((cardCountShowSeek + i) * 8, isNotRow * 2, str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                    }
                    key = KEY_NUL;      // ���ٸ��½�����
                    g_ucIsUpdateMenu = 0;
                }
                else            // ���������ģʽ,���������ֵ
                {
                    if (0 < isNotRow)
                    {
                        isNotRow--;
                    }
                }
                break;
            case KEY_DOWN:
                if (isSetMode)
                {
                    switch(seek)
                    {
                        case 0:
                            temp = usaCardCount[isNotRow] / 1000;
                            usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 1000 : usaCardCount[isNotRow];
                            break;
                        case 1:
                            temp = usaCardCount[isNotRow] % 1000 / 100;
                            usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 100 : usaCardCount[isNotRow];
                            break;
                        case 2:
                            temp = usaCardCount[isNotRow] % 100 / 10;
                            usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 10 : usaCardCount[isNotRow];
                            break;
                        case 3:
                            temp = usaCardCount[isNotRow] % 10;
                            usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 1 : usaCardCount[isNotRow];
                            break;
                    }
                    sprintf(str_aCardCount[isNotRow],"%04d",usaCardCount[isNotRow]);        // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����
                    for (i = 0; i < 4; i++)
                    {
                        display_GB2312_char ((cardCountShowSeek + i) * 8, isNotRow * 2, str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                    }
                    key = KEY_NUL;      // ���ٸ��½�����
                    g_ucIsUpdateMenu = 0;
                }
                else            // ���������ģʽ,���������ֵ
                {
                    if (isNotRow < 3)
                    {
                        isNotRow++;
                    }
                }

                break;
            case KEY_LEFT:
                if (isSetMode && seek > 0)
                {
                    seek--;
                    for (i = 0; i < 4; i++)
                    {
                        display_GB2312_char ((cardCountShowSeek + i) * 8, isNotRow * 2, str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                    }
                }
                key = KEY_NUL;
                g_ucIsUpdateMenu = 0;
                break;
            case KEY_RIGHT:
                if (isSetMode && seek < 3)
                {
                    seek++;
                    for (i = 0; i < 4; i++)
                    {
                        display_GB2312_char ((cardCountShowSeek + i) * 8, isNotRow * 2, str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                    }
                }
                key = KEY_NUL;      // ���ٸ��½�����
                g_ucIsUpdateMenu = 0;
                break;
            case KEY_OK:
                isSetMode = 0;      // �˳�����ģʽ
                seek = 0;                   // ���㵱ǰ���ڵ�����λ��
                g_usaInitCardCount[isNotRow + 1] = usaCardCount[isNotRow];
                for (i = 0; i < 4; i++)     // �����޸Ĺ��Ŀ���������ʾ����
                {
                    for (j = 0; j < 4; j++)
                    {
                        g_dlg[dlgId].MsgRow[i][j + cardCountShowSeek] = str_aCardCount[i][j];
                    }
                }
                break;
            case KEY_CANCEL:
                if (!isSetMode)
                {
                    return;                     // ����Ƿ�����ģʽ,��ֱ���˳�
                }
                else
                {
                    isSetMode = 0;              // �˳�����ģʽ
                    seek = 0;                   // ���㵱ǰ���ڵ�����λ��
                }
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //��������������
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
    {                            //������,ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_RIGHT:
                doShowStatusTwo (DLG_STATUS_TWO, 5, NULL);
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //��������������
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
    {                            //������,ͨ��д��һ����ѭ��
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //��������������
    }
}



// ID���ò˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowWorkingSet (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = 1;      // ������
    unsigned char str_num[10] = {0};
    unsigned char str_id[10] = {0};

    unsigned char master1[] = "1:����   2:���� ";
    unsigned char master2[] = "1:����   2:���� ";
    unsigned char master3[] = "3:����   4:���� ";
    unsigned char master4[] = "3:����   4:���� ";

    unsigned short id = 0x7811;      // CANͨ�ŵ�ID

    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //������,ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
                key = KEY_NUL;
                break;
            case KEY_UP:
                if (1 < isNotRow)
                {
                    isNotRow--;
                }
                break;
            case KEY_DOWN:
                if (2 > isNotRow)
                {
                    isNotRow++;
                }
                break;
            case KEY_LEFT:
                switch (isNotRow)
                {
                    case 1:
                        for (i = 0; i < 16; i++)
                        {
                            g_dlg[dlgId].MsgRow[1][i] = master1[i];
                        }
                        g_usUpWorkingID = 0x7811;
                        g_usUpBackingID = 0x7812;
                        break;
                    case 2:
                        for (i = 0; i < 16; i++)
                        {
                            g_dlg[dlgId].MsgRow[2][i] = master3[i];
                        }
                        g_usDownWorkingID = 0x7813;
                        g_usDownBackingID = 0x7814;
                        break;
                    default:
                        break;
                }
                break;
            case KEY_RIGHT:
                switch (isNotRow)
                {
                    case 1:
                        for (i = 0; i < 16; i++)
                        {
                            g_dlg[dlgId].MsgRow[1][i] = master2[i];
                        }
                        g_usUpWorkingID = 0x7812;
                        g_usUpBackingID = 0x7811;
                        break;
                    case 2:
                        for (i = 0; i < 16; i++)
                        {
                            g_dlg[dlgId].MsgRow[2][i] = master4[i];
                        }
                        g_usDownWorkingID = 0x7814;
                        g_usDownBackingID = 0x7813;
                    default:
                        break;

                }
                break;
            case KEY_OK:
                switch (isNotRow)
                {
                    case 1:
                        myCANTransmit(&gt_TxMessage, (unsigned char)(g_usUpWorkingID & 0x000f), 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
                        myCANTransmit(&gt_TxMessage, (unsigned char)(g_usUpBackingID & 0x000f), 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬
                        isNotRow++;
                        break;
                    case 2:
                        myCANTransmit(&gt_TxMessage, (unsigned char)(g_usDownWorkingID & 0x000f), 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
                        myCANTransmit(&gt_TxMessage, (unsigned char)(g_usDownBackingID & 0x000f), 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬
                        isNotRow--;
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
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //��������������
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
    {                            //������,ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //��������������
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
    {                            //������,ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
                switch (isNotRow)
                {
                    case 0:
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //��������������
    }
}



// ���Բ˵�(����),�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
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
    {                            //������,ͨ��д��һ����ѭ��
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
                myCANTransmit(&gt_TxMessage, num, num, QUIT_DEBUG, num, id_h, id_l, NO_FAIL);   // �˳�����
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //��������������
    }
}

// ���Բ˵�(����),�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
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
    {                            //������,ͨ��д��һ����ѭ��
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
                myCANTransmit(&gt_TxMessage, num, num, QUIT_DEBUG, num, id_h, id_l, NO_FAIL);
                return;
                break;
            default:
                key = KEY_NUL;
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //��������������
    }
}


// ��ʾ������,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowFaultCode (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char n = 0;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = 1;      // ������
    unsigned char str_num[5] = {0};
    unsigned char str_id[5] = {0};
    unsigned short id = 0x7810 | num;       // CANͨ�ŵ�ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    unsigned char id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    unsigned char faultCodeIndex = 0;
    unsigned char faultCode = 0;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < (sizeof (g_dlg_fault_code) / sizeof (g_dlg_fault_code[0])); j++)
        {
            if (g_ucaFaultCode[i][0] != 0 && g_ucaFaultCode[i][0] == g_dlg_fault_code[j].ID) // ���ҹ������Ӧ����ʾ����
            {
                faultCode = g_ucaFaultCode[i][0];   // ���µ�ǰ��δ����Ĺ��ϺͿ�����
                faultCodeIndex = i;
                g_ucaFaultCode[i][0] = 0;
                num = i + 1;        // ��ס���ĸ������й���,���������
                sprintf(str_num,"%02d",num);
                for (n = 0; n < 2; n++)
                {
                    g_dlg_fault_code[j].MsgRow[0][n + 1] = str_num[n];
                }
                for (n = 0; n < 4; n++)
                {
                    display_GB2312_string (0, n * 2, g_dlg_fault_code[j].MsgRow[n], 0);     // ��ʾ���Ͻ���
                }
                goto while_label;
            }
        }
    }
/*
    if (i == 4)
    {
        for (n = 0; n < 4; n++)
        {
            display_GB2312_string (0, n * 2, g_dlg_fault_code[12].MsgRow[n], 0);     // ��ʾ���Ͻ���
        }
    }
*/
while_label:
    while (DEF_TRUE)
    {                            //������,ͨ��д��һ����ѭ��
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:     // �ڹ�������ʾ����,���˵������Խ������ý���,�Ҳ��������ֵ
                g_ucaFaultCode[faultCodeIndex][0] = faultCode; // ���浱ǰδ����Ĺ���,�����´��ٴδ���
                return;
                break;
            case KEY_CANCEL:
                //g_ucaFaultCode[faultCodeIndex][0] = 0;
                myCANTransmit(&gt_TxMessage, num, NO_FAIL, CLEAR_FAULT_CODE, CLEAR_FAULT, NO_FAIL, NO_FAIL, NO_FAIL);
                g_ucIsUpdateMenu = 1;
                return;
                break;
            default:
                break;
        }

        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //��������������
    }
}

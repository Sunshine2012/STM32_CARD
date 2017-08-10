#include <includes.h>
#include "menu.h"

unsigned char g_ucCurDlg;

Dlg g_dlg[] = {
                        {DLG_LOGO,           "    ****电子    ", " www.*****.com  ", "   ****发卡机   ", "   版本: V1.0   "},
                        {DLG_STATUS,         "1:              ", "2:              ", "3:              ", "4:              "},
                        {DLG_MAIN,           "     主菜单     ", "1.设置卡箱及ID  ", "2.调看卡机状态  ", "3.调机运行状态  "},
                        {DLG_CARD_ID,        "   设置卡机ID   ", "卡机号:         ", "通信ID号:       ", "                "},
                        {DLG_CARD_MAIN,      "卡机状态:1号卡机", "         2号卡机", "         3号卡机", "         4号卡机"},
                        {DLG_MECHINE_STATUS, "卡机状态:(     )", "出好卡数量:     ", "回收坏卡数:     ", "故障次数:       "},

                        {DLG_DEBUG_MAIN,     "1:              ", "2:              ", "3:              ", "4:              "},
                        {DLG_DEBUG_ONE,      "                ", "1.连续循环出卡  ", "2.循环出一张卡  ", "3.翻一张好卡    "},
                        {DLG_DEBUG_TWO,      "                ", "4.读取送卡传感器", "5.读取卡盒传感器", "6.读取取卡传感器"},

                        {255,                "                ", "                ", "                ", "                "},
                     };
// 找到显示的菜单ID，并返回其在数组中的索引
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

// 显示菜单,如果有一行需要反显示,则设置当前行反显示
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
    while (DEF_TRUE)
    {                            //任务体，通常写成一个死循环
        key = g_ucKeyValues;
        g_ucKeyValues = KEY_NUL;
        switch ( key )
        {
            case KEY_ENTRY:
                doShowMainMenu (DLG_MAIN, 1);
                break;
            case KEY_QUIT:
                g_ucCurDlg = DLG_STATUS;
                return;
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
        OSTimeDly ( 5, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}

// 显示菜单,如果有一行需要反显示,则设置当前行反显示
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
    {                            //任务体，通常写成一个死循环
        key = g_ucKeyValues;
        g_ucKeyValues = KEY_NUL;
        switch ( key )
        {
            case KEY_ENTRY:
                switch (isNotRow)
                {
                    case 0:
                        isNotRow++;
                    case 1:
                        doShowIdSetMenu (DLG_CARD_ID, 1);
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    default:
                        break;
                }
                break;
            case KEY_CANCEL:    // 退出
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
        OSTimeDly ( 5, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}


// 显示菜单,如果有一行需要反显示,则设置当前行反显示
void doShowIdSetMenu (unsigned char dlg_id, unsigned char isNotRow)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = 1;      // 卡机号
    unsigned char str_num[10] = {0};
    unsigned char str_id[10] = {0};
    unsigned char id_h = 0;          // CAN通信的ID高字节
    unsigned char id_l = 0;          // CAN通信的ID低字节
    unsigned short id = 0x7811;      // CAN通信的ID
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
    {                            //任务体，通常写成一个死循环
        key = g_ucKeyValues;
        g_ucKeyValues = KEY_NUL;
        switch ( key )
        {
            case KEY_ENTRY:
                if (1 <= num <=4 && 0x7811 <= id <= 0x7814)
                {
                    id_h = ( id >> 8 ) & 0xff;
                    id_l = id & 0xff;
                    MyCANTransmit(&gt_TxMessage, 0, 0, SET_MECHINE_ID, num, id_h, id_l, NO_FAIL);
                }
                break;
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
                if (2 > isNotRow)
                {
                    isNotRow++;
                }
                break;
            case KEY_LEFT:
                switch ( isNotRow )
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
                switch ( isNotRow )
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
                if (1 <= num <=4 && 0x7811 <= id <= 0x7814)
                {
                    id_h = ( id >> 8 ) & 0xff;
                    id_l = id & 0xff;
                    MyCANTransmit(&gt_TxMessage, 0, 0, SET_MECHINE_ID, num, id_h, id_l, NO_FAIL);
                }
                break;
            default:
                break;
        }
        if (KEY_NUL != key)
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
        OSTimeDly ( 5, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}

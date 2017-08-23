#include <includes.h>
#include "menu.h"

unsigned char g_ucCurDlg;                       // 当前显示的菜单ID
unsigned short g_usCurID = 0x7811;              // 当前通信设备的ID



Dlg g_dlg[] = {
                        {DLG_LOGO,           "    ****电子    ", " www.*****.com  ", "   ****发卡机   ", "   版本: V1.0   "},
                        {DLG_STATUS,         "1号工作         ", "2号备用         ", "3号工作         ", "4号备用         "},
                        {DLG_MAIN,           "     主菜单     ", "1.卡机状态      ", "2.卡机设置      ", "3.调机运行      "},
                        {DLG_CARD_ID,        "   设置卡机ID   ", "搜索卡机        ", "卡机号:         ", "通信ID号:       "},
                        {DLG_CARD_MAIN,      "卡机状态:1号卡机", "         2号卡机", "         3号卡机", "         4号卡机"},
                        {DLG_STATUS_ONE,     "1号出卡:        ", "2号出卡:        ", "3号出卡:        ", "4号出卡:        "},
                        {DLG_STATUS_TWO,     "1号出坏卡:      ", "2号出坏卡:      ", "3号出坏卡:      ", "4号出坏卡:      "},
                       //{DLG_STATUS_ONE,     "    卡机状态    ", "出好卡数量:     ", "回收坏卡数:     ", "故障次数:       "},

                        {DLG_DEBUG_MAIN,     "    号卡机调试  ", "1:联动运行      ", "2:单动运行      ", "                "},
                        {DLG_DEBUG_ONE,      "↑:翻一张好卡    ", "↓:翻一张坏卡    ", "←:勾一张卡      ", "→:循环出卡      "},
                        {DLG_DEBUG_TWO,      "↑:单动正翻卡    ", "↓:单动反翻卡    ", "←:单动正勾卡    ", "→:单动反勾卡    "},

                        //{DLG_DEBUG_ONE,      "    号卡机调试  ", "1.连续循环出卡  ", "2.循环出一张卡  ", "3.翻一张好卡    "},
                        //{DLG_DEBUG_TWO,      "    号卡机调试  ", "4.翻一张坏卡    ", "5.勾一张卡      ", "6.读取送卡传感器"},

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

// 显示状态菜单,如果有一行需要反显示,则设置当前行反显示,传递参数地址
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

// 显示菜单,如果有一行需要反显示,则设置当前行反显示,传递参数地址
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
    {                            //任务体，通常写成一个死循环
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
            case KEY_CANCEL:    // 退出
                return;
                break;
            default:
                key = KEY_NUL;
                break;
        }
        if (g_ucKeyValues == KEY_QUIT)      // 按QUIT键,退出,并清理按键值
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
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}


// 状态菜单1,如果有一行需要反显示,则设置当前行反显示,传递参数地址
void doShowStatusOne (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = *(unsigned char *)p_parm;      // 卡机号
    unsigned char str_num[5] = {0};
    unsigned char str_id[5] = {0};
    unsigned short id = 0x7810 | num;       // CAN通信的ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CAN通信的ID高字节
    unsigned char id_l = id & 0xff;                         // CAN通信的ID低字节
    OLED_CLS(); // 显示之前,清屏
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //任务体，通常写成一个死循环
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
        if (g_ucKeyValues == KEY_QUIT)      // 按QUIT键,直接退到主界面,避免连续的刷屏,退出,并保持按键值不变
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // 如果有按键按下,则更新界面
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}


// 状态菜单2,如果有一行需要反显示,则设置当前行反显示,传递参数地址
void doShowStatusTwo (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = *(unsigned char *)p_parm;      // 卡机号
    unsigned char str_num[5] = {0};
    unsigned char str_id[5] = {0};
    unsigned short id = 0x7810 | num;       // CAN通信的ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CAN通信的ID高字节
    unsigned char id_l = id & 0xff;                         // CAN通信的ID低字节
    OLED_CLS(); // 显示之前,清屏
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //任务体，通常写成一个死循环
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
        if (g_ucKeyValues == KEY_QUIT)      // 按QUIT键,直接退到主界面,避免连续的刷屏,退出,并保持按键值不变
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // 如果有按键按下,则更新界面
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}




// ID设置菜单,如果有一行需要反显示,则设置当前行反显示,传递参数地址
void doShowIdSetMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
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
    {                            //任务体，通常写成一个死循环
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
                        for (i = 0; i < 4; i++)         // 循环搜索四台卡机,看哪个卡机有回应
                        {
                            id_h = 0x78 ;
                            id_l = 0x11 + i;
                            myCANTransmit(&gt_TxMessage, i, i, SEARCH_CARD_MECHINE, i, id_h, id_l, NO_FAIL);
                        }
                        break;
                    //case 2:
                    case 3:
                        if (1 <= num && num <=4 && 0x7811 <= id && id <= 0x7814)    // 设置搜索到的卡机的卡机号和ID号
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
        if (g_ucKeyValues == KEY_QUIT)      // 按QUIT键,直接退到主界面,避免连续的刷屏,退出,并保持按键值不变
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // 如果有按键按下,则更新界面
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
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}


// 调试菜单主选择,如果有一行需要反显示,则设置当前行反显示,传递参数地址
void doShowDebugMain (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = 1;      // 卡机号
    unsigned char str_num[10] = {0};
    unsigned char str_id[10] = {0};
    unsigned short id = 0x7810 | num;       // CAN通信的ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CAN通信的ID高字节
    unsigned char id_l = id & 0xff;                         // CAN通信的ID低字节
    sprintf(str_num,"0%d",num);
    str_num [3] = 0;
    for (i = 0; i < 2; i++)
    {
        g_dlg[dlgId].MsgRow[0][i + 2] = str_num[i];
    }
    OLED_CLS(); // 显示之前,清屏
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //任务体，通常写成一个死循环
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:
                switch (isNotRow)
                {
                    case 0:
                        //isNotRow++;     // 只进行反显操作,不做其他操作
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
        if (g_ucKeyValues == KEY_QUIT)      // 按QUIT键,直接退到主界面,避免连续的刷屏,退出,并保持按键值不变
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // 如果有按键按下,则更新界面
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}



// 调试菜单1,如果有一行需要反显示,则设置当前行反显示,传递参数地址
void doShowDebugOne (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = *(unsigned char *)p_parm;      // 卡机号
    unsigned char str_num[5] = {0};
    unsigned char str_id[5] = {0};
    unsigned short id = 0x7810 | num;       // CAN通信的ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CAN通信的ID高字节
    unsigned char id_l = id & 0xff;                         // CAN通信的ID低字节
    OLED_CLS(); // 显示之前,清屏
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //任务体，通常写成一个死循环
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
        if (g_ucKeyValues == KEY_QUIT)      // 按QUIT键,直接退到主界面,避免连续的刷屏,退出,并保持按键值不变
        {
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // 如果有按键按下,则更新界面
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}

// 调试菜单2,如果有一行需要反显示,则设置当前行反显示,传递参数地址
void doShowDebugTwo (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = *(unsigned char *)p_parm;      // 卡机号
    unsigned char str_num[10] = {0};
    unsigned char str_id[10] = {0};
    unsigned short id = 0x7810 | num;      // CAN通信的ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CAN通信的ID高字节
    unsigned char id_l = id & 0xff;                         // CAN通信的ID低字节
    g_ucKeyContinu = 1;             // 进入单动模式
    OLED_CLS(); // 显示之前,清屏
    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //任务体，通常写成一个死循环
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
                g_ucKeyContinu = 0;             // 退出单动模式
                return;
                break;
            default:
                g_ucKeyValues = KEY_NUL;
                break;
        }
        if (g_ucKeyContinu == 0xff)
        {
            g_ucKeyContinu = 1;             // 松开按键之后,给卡机发送停止命令
            myCANTransmit(&gt_TxMessage, num, num, STOP_DEBUG, num, id_h, id_l, NO_FAIL);
        }
        if (g_ucKeyValues == KEY_QUIT)      // 按QUIT键,直接退到主界面,避免连续的刷屏,退出,并保持按键值不变
        {
            g_ucKeyContinu = 0;             // 退出单动模式
            return;
        }
        if (KEY_NUL != key || g_ucIsUpdateMenu)    // 如果有按键按下,则更新界面
        {
            g_ucIsUpdateMenu = 0;
            for (i = 0; i < 4; i++)
            {
                display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
            }
        }
        g_ucKeyValues = KEY_NUL;
        OSTimeDly ( 100, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}

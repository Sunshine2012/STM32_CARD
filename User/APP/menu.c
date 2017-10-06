#include <includes.h>
#include "menu.h"

unsigned char  g_ucCurDlg           = 0;            // 当前显示的菜单ID
unsigned short g_usCurID            = 0x7811;       // 当前通信设备的ID
unsigned short g_usUpWorkingID      = 0x7811;       // 上工位工作卡机ID
unsigned short g_usUpBackingID      = 0x7812;       // 上工位备用卡机ID
unsigned short g_usDownWorkingID    = 0x7813;       // 下工位工作卡机ID
unsigned short g_usDownBackingID    = 0x7814;       // 下工位备用卡机ID
unsigned char  g_ucaFaultCode[4][10]   = {0};       // 卡机是否有未处理的故障

Dlg g_dlg[] = {
                        {DLG_CLEAR_LCD,      "                ", "                ", "                ", "                "},

                        {DLG_LOGO,           "    ****电子    ", " www.*****.com  ", "   ****发卡机   ", "   版本: V1.0   "},
                        {DLG_STATUS,         "1.工作:         ", "2.备用:         ", "3.工作:         ", "4.备用:         "},
                        {DLG_MAIN,           "1.卡机状态      ", "2.卡数设置      ", "3.卡机设置      ", "4.调机运行      "},
                        {DLG_CARD_COUNT_SET, "1号机卡数:      ", "2号机卡数:      ", "3号机卡数:      ", "4号机卡数:      "},
                        {DLG_CARD_ID,        "   设置卡机ID   ", "搜索卡机        ", "卡机号:         ", "通信ID号:       "},
                        {DLG_WORKING_SET,    " 卡机工作设置   ", "1:工作   2:备用 ", "3:工作   4:备用 ", "                "},

                        {DLG_CARD_MAIN,      "卡机状态:1号卡机", "         2号卡机", "         3号卡机", "         4号卡机"},
                        {DLG_STATUS_ONE,     "1号出卡:        ", "2号出卡:        ", "3号出卡:        ", "4号出卡:        "},
                        {DLG_STATUS_TWO,     "1号出坏卡:      ", "2号出坏卡:      ", "3号出坏卡:      ", "4号出坏卡:      "},

                        {DLG_DEBUG_MAIN,     "     号卡机调试 ", "1:联动运行      ", "2:单动运行      ", "                "},
                        {DLG_DEBUG_ONE,      "↑:翻一张好卡    ", "↓:翻一张坏卡    ", "←:勾一张卡      ", "→:循环出卡      "},
                        {DLG_DEBUG_TWO,      "↑:单动正翻卡    ", "↓:单动反翻卡    ", "←:单动正勾卡    ", "→:单动反勾卡    "},

                        {DLG_FAULT_CODE,     "    号卡机故障  ", "故障码:         ", "说明:           ", "                "},

                     };
Dlg g_dlg_fault_code[] = {
                        {DLG_CLEAR_LCD,      "                ", "                ", "                ", "                "},

                        {FAULT_CODE01,       "    号卡机故障  ", "故障码:(01H)    ", "说明: 初始化勾卡", "电机不能回位    "},
                        {FAULT_CODE02,       "    号卡机故障  ", "故障码:(02H)    ", "说明: 初始化翻卡", "电机反转不能回位"},
                        {FAULT_CODE03,       "    号卡机故障  ", "故障码:(03H)    ", "说明: 初始化翻卡", "电机正转不能回位"},
                        {FAULT_CODE04,       "    号卡机故障  ", "故障码:(04H)    ", "说明: 工作时勾卡", "电机向前堵转    "},
                        {FAULT_CODE05,       "    号卡机故障  ", "故障码:(05H)    ", "说明: 工作时勾卡", "电机向后堵转    "},
                        {FAULT_CODE06,       "    号卡机故障  ", "故障码:(06H)    ", "说明: 工作时翻卡", "电机前翻堵转    "},
                        {FAULT_CODE07,       "    号卡机故障  ", "故障码:(07H)    ", "说明: 工作时翻卡", "电机前翻回程堵转"},
                        {FAULT_CODE08,       "    号卡机故障  ", "故障码:(08H)    ", "说明: 工作时翻卡", "电机反翻堵转    "},
                        {FAULT_CODE09,       "    号卡机故障  ", "故障码:(09H)    ", "说明: 工作时翻卡", "电机反翻回程堵转"},
                        {FAULT_NO_CARD,      "    号卡机故障  ", "说明: 卡机中无卡", "请装载IC卡      ", "                "},
                        {FAULT_BAD_CARD,     "    号卡机故障  ", "说明: 坏卡超数量", "请清理坏卡      ", "                "},
                        {255,                "    未知故障    ", "故障码:(未知)   ", "                ", "                "},
                     };

// 找到显示的菜单ID,并返回其在数组中的索引
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
    {                            //任务体,通常写成一个死循环
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}

// 设置卡机卡数量,如果有一行需要反显示,则设置当前行反显示,传递参数地址
void doShowCardCountSet (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char isSetMode = 0;   // 如果是设置模式/
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char seek = 0;        // 指示当前调节的数据位置
    unsigned char temp = 0;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = 1;      // 卡机号

    unsigned short id = 0x7810 | num;                       // CAN通信的ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CAN通信的ID高字节
    unsigned char id_l = id & 0xff;                         // CAN通信的ID低字节
    unsigned char cardCountShowSeek = 10;   // 卡数量字符从此行中的第几个显示,默认从11个字节开始显示
    unsigned short usaCardCount[4] = {1000,1001,1002,1003};     // 所有卡机初始化卡数量缓存
    unsigned char str_aCardCount[4][6] = {0};                     // 所有卡机初始化卡数量缓存显示字符串
    for (i = 0; i < 4; i++)
    {
        usaCardCount[i] = g_usaInitCardCount[i + 1];    // 复制初始化卡数量到缓存，以供修改
    }
    for (i = 0; i < 4; i++)
    {
        sprintf(str_aCardCount[i],"%04d",usaCardCount[i]);      // 格式化初始化卡数量到字符数组，以供显示,右对齐,高位不足用0补齐
    }

    for (i = 0; i < 4; i++)     // 复制初始化卡数量字符数组到显示缓存
    {
        for (j = 0; j < 4; j++)
        {
            g_dlg[dlgId].MsgRow[i][j + 10] = str_aCardCount[i][j];
        }
    }
    for (i = 0; i < 4; i++) // 显示
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    g_ucKeyValues = KEY_NUL;
    while (DEF_TRUE)
    {                            //任务体,通常写成一个死循环
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
                        isSetMode = 1;      // 进入设置模式,按取消键退出设置，再次按取消键才退回上一级菜单
                        display_GB2312_string (0, isNotRow * 2, g_dlg[dlgId].MsgRow[isNotRow], 0);
                        for (i = 0; i < 4; i++)
                        {
                            sprintf(str_aCardCount[i],"%04d",usaCardCount[i]);      // 格式化初始化卡数量到字符数组，以供显示,右对齐,高位不足用0补齐
                            display_GB2312_char ((cardCountShowSeek + i) * 8, isNotRow * 2, str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                        }
                        key = KEY_NUL;      // 不再更新界面了
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
                    sprintf(str_aCardCount[isNotRow],"%04d",usaCardCount[isNotRow]);        // 格式化初始化卡数量到字符数组，以供显示,右对齐,高位不足用0补齐
                    for (i = 0; i < 4; i++)
                    {
                        display_GB2312_char ((cardCountShowSeek + i) * 8, isNotRow * 2, str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                    }
                    key = KEY_NUL;      // 不再更新界面了
                    g_ucIsUpdateMenu = 0;
                }
                else            // 如果是设置模式,则调整的是值
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
                    sprintf(str_aCardCount[isNotRow],"%04d",usaCardCount[isNotRow]);        // 格式化初始化卡数量到字符数组，以供显示,右对齐,高位不足用0补齐
                    for (i = 0; i < 4; i++)
                    {
                        display_GB2312_char ((cardCountShowSeek + i) * 8, isNotRow * 2, str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                    }
                    key = KEY_NUL;      // 不再更新界面了
                    g_ucIsUpdateMenu = 0;
                }
                else            // 如果是设置模式,则调整的是值
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
                key = KEY_NUL;      // 不再更新界面了
                g_ucIsUpdateMenu = 0;
                break;
            case KEY_OK:
                isSetMode = 0;      // 退出设置模式
                seek = 0;                   // 归零当前调节的数据位置
                g_usaInitCardCount[isNotRow + 1] = usaCardCount[isNotRow];
                for (i = 0; i < 4; i++)     // 复制修改过的卡数量到显示缓存
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
                    return;                     // 如果是非设置模式,则直接退出
                }
                else
                {
                    isSetMode = 0;              // 退出设置模式
                    seek = 0;                   // 归零当前调节的数据位置
                }
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
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
    {                            //任务体,通常写成一个死循环
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
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
    {                            //任务体,通常写成一个死循环
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}



// ID设置菜单,如果有一行需要反显示,则设置当前行反显示,传递参数地址
void doShowWorkingSet (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = 1;      // 卡机号
    unsigned char str_num[10] = {0};
    unsigned char str_id[10] = {0};

    unsigned char master1[] = "1:工作   2:备用 ";
    unsigned char master2[] = "1:备用   2:工作 ";
    unsigned char master3[] = "3:工作   4:备用 ";
    unsigned char master4[] = "3:备用   4:工作 ";

    unsigned short id = 0x7811;      // CAN通信的ID

    for (i = 0; i < 4; i++)
    {
        display_GB2312_string (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    while (DEF_TRUE)
    {                            //任务体,通常写成一个死循环
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
                        myCANTransmit(&gt_TxMessage, (unsigned char)(g_usUpWorkingID & 0x000f), 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // 设置工作态
                        myCANTransmit(&gt_TxMessage, (unsigned char)(g_usUpBackingID & 0x000f), 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // 设置备用态
                        isNotRow++;
                        break;
                    case 2:
                        myCANTransmit(&gt_TxMessage, (unsigned char)(g_usDownWorkingID & 0x000f), 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // 设置工作态
                        myCANTransmit(&gt_TxMessage, (unsigned char)(g_usDownBackingID & 0x000f), 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // 设置备用态
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
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
    {                            //任务体,通常写成一个死循环
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
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
    {                            //任务体,通常写成一个死循环
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}



// 调试菜单(联动),如果有一行需要反显示,则设置当前行反显示,传递参数地址
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
    {                            //任务体,通常写成一个死循环
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
                myCANTransmit(&gt_TxMessage, num, num, QUIT_DEBUG, num, id_h, id_l, NO_FAIL);   // 退出调试
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}

// 调试菜单(单动),如果有一行需要反显示,则设置当前行反显示,传递参数地址
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
    {                            //任务体,通常写成一个死循环
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
                myCANTransmit(&gt_TxMessage, num, num, QUIT_DEBUG, num, id_h, id_l, NO_FAIL);
                return;
                break;
            default:
                key = KEY_NUL;
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}


// 显示故障码,如果有一行需要反显示,则设置当前行反显示,传递参数地址
void doShowFaultCode (unsigned char dlg_id, unsigned char isNotRow, void * p_parm)
{
    OS_ERR      err;
    unsigned char i = 0;
    unsigned char j = 0;
    unsigned char n = 0;
    unsigned char dlgId = check_menu(dlg_id);
    unsigned char key = KEY_NUL;
    unsigned char num = 1;      // 卡机号
    unsigned char str_num[5] = {0};
    unsigned char str_id[5] = {0};
    unsigned short id = 0x7810 | num;       // CAN通信的ID
    unsigned char id_h = ( id >> 8 ) & 0xff;                // CAN通信的ID高字节
    unsigned char id_l = id & 0xff;                         // CAN通信的ID低字节
    unsigned char faultCodeIndex = 0;
    unsigned char faultCode = 0;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < (sizeof (g_dlg_fault_code) / sizeof (g_dlg_fault_code[0])); j++)
        {
            if (g_ucaFaultCode[i][0] != 0 && g_ucaFaultCode[i][0] == g_dlg_fault_code[j].ID) // 查找故障码对应的显示界面
            {
                faultCode = g_ucaFaultCode[i][0];   // 记下当前的未处理的故障和卡机号
                faultCodeIndex = i;
                g_ucaFaultCode[i][0] = 0;
                num = i + 1;        // 记住是哪个卡机有故障,以清除故障
                sprintf(str_num,"%02d",num);
                for (n = 0; n < 2; n++)
                {
                    g_dlg_fault_code[j].MsgRow[0][n + 1] = str_num[n];
                }
                for (n = 0; n < 4; n++)
                {
                    display_GB2312_string (0, n * 2, g_dlg_fault_code[j].MsgRow[n], 0);     // 显示故障界面
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
            display_GB2312_string (0, n * 2, g_dlg_fault_code[12].MsgRow[n], 0);     // 显示故障界面
        }
    }
*/
while_label:
    while (DEF_TRUE)
    {                            //任务体,通常写成一个死循环
        key = g_ucKeyValues;
        switch (key)
        {
            case KEY_ENTRY:     // 在故障码显示界面,按菜单键可以进入设置界面,且不清除按键值
                g_ucaFaultCode[faultCodeIndex][0] = faultCode; // 保存当前未处理的故障,以免下次再次处理
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
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );     //不断阻塞该任务
    }
}

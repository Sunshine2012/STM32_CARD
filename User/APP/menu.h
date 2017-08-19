#ifndef __MENU_H
#define __MENU_H
#include <includes.h>

// dlg ID
typedef enum DLG_ID
{
    DLG_LOGO                     = 0x00,                 /*  */
    DLG_STATUS                   = 0x01,                 /*  */
    DLG_MAIN                     = 0x02,                 /*  */
    DLG_PASS                     = 0x03,                 /*  */
    DLG_CARD_ID                  = 0x04,                 /*  */

    DLG_CARD_MAIN                = 0x10,                 /*  */
    DLG_STATUS_ONE               = 0x11,                 /*  */
    DLG_STATUS_TWO               = 0x12,                 /*  */

    DLG_DEBUG_MAIN               = 0x20,                 /*  */
    DLG_DEBUG_ONE                = 0x21,                 /*  */
    DLG_DEBUG_TWO                = 0x22,                 /*  */
    DLG_DEBUG_THREE              = 0x23,                 /*  */
}DLG_ID;


// 界面结构体
typedef struct Dlg
{
   CPU_INT08U ID;                 /* 类型：正(30H), 负(31H) */
   char MsgRow[4][36];
}Dlg;


extern Dlg                              g_dlg[];
extern unsigned char g_ucCurDlg;           // 当前显示的菜单ID
extern unsigned short g_usCurID;           // 当前通信设备的ID,在设置设备ID时候使用


void doShowStatusMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowMainMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowStatusOne (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowStatusTwo (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowIdSetMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowDebugMain (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowDebugOne (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowDebugTwo (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);

#endif

#ifndef __MENU_H
#define __MENU_H
#include <includes.h>

// dlg ID
typedef enum DLG_ID
{
    DLG_CLEAR_LCD                = 0,
    DLG_LOGO                     = 1,                 /*  */
    DLG_STATUS                   = 2,                 /*  */
    DLG_MAIN                     = 3,                 /*  */
    DLG_PASS                     = 4,                 /*  */
    DLG_CARD_ID                  = 5,                 /*  */
    DLG_WORKING_SET              = 6,                 /*  */

    DLG_CARD_MAIN                = 20,                 /*  */
    DLG_STATUS_ONE               = 21,                 /*  */
    DLG_STATUS_TWO               = 22,                 /*  */

    DLG_CARD_COUNT_SET           = 30,                 /*  */

    DLG_DEBUG_MAIN               = 40,                 /*  */
    DLG_DEBUG_ONE                = 41,                 /*  */
    DLG_DEBUG_TWO                = 42,                 /*  */
    DLG_DEBUG_THREE              = 43,                 /*  */

    DLG_FAULT_CODE               = 60,                 /* 故障码 */

}DLG_ID;

// 界面结构体
typedef struct Dlg
{
   CPU_INT08U ID;
   char MsgRow[4][36];
}Dlg;

extern Dlg           g_dlg[];
extern Dlg g_dlg_fault_code[];
extern unsigned char g_ucCurDlg;            // 当前显示的菜单号
extern unsigned char g_ucCurID;             // 当前通信设备的号,在设置设备号时候使用
extern unsigned char g_ucUpWorkingID;       // 上工位工作卡机号
extern unsigned char g_ucUpBackingID;       // 上工位备用卡机号
extern unsigned char g_ucDownWorkingID;     // 下工位工作卡机号
extern unsigned char g_ucDownBackingID;     // 下工位备用卡机号
extern unsigned char g_ucaCardIsReady[4];   // 卡就绪
extern unsigned char g_ucaFaultCode[4];     // 卡机是否有未处理的故障
extern unsigned char g_ucaDeviceIsSTBY[4];  // 两个卡机处于待机(Standby)状态下,按键按下,主机收到按键信息发卡,只要有卡没有被取走,即使收到按键信息,也不再发卡.
extern unsigned char g_ucaMechineExist[4];  // 卡机是否存在并通信正常



void doShowStatusMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowMainMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowCardCountSet (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowStatusOne (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowStatusTwo (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowIdSetMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowWorkingSet (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowDebugMain (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowDebugOne (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowDebugTwo (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowFaultCode (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);

unsigned char check_menu(unsigned char ch);

#endif

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


// ����ṹ��
typedef struct Dlg
{
   CPU_INT08U ID;                 /* ���ͣ���(30H), ��(31H) */
   char MsgRow[4][36];
}Dlg;


extern Dlg                              g_dlg[];
extern unsigned char g_ucCurDlg;           // ��ǰ��ʾ�Ĳ˵�ID
extern unsigned short g_usCurID;           // ��ǰͨ���豸��ID,�������豸IDʱ��ʹ��


void doShowStatusMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowMainMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowStatusOne (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowStatusTwo (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowIdSetMenu (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowDebugMain (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowDebugOne (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);
void doShowDebugTwo (unsigned char dlg_id, unsigned char isNotRow, void * p_parm);

#endif

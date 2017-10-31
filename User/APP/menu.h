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

    DLG_FAULT_CODE               = 60,                 /* ������ */

}DLG_ID;

// ����ṹ��
typedef struct Dlg
{
   CPU_INT08U ID;
   char MsgRow[4][36];
}Dlg;

extern Dlg           g_dlg[];
extern Dlg g_dlg_fault_code[];
extern unsigned char g_ucCurDlg;            // ��ǰ��ʾ�Ĳ˵���
extern unsigned char g_ucCurID;             // ��ǰͨ���豸�ĺ�,�������豸��ʱ��ʹ��
extern unsigned char g_ucUpWorkingID;       // �Ϲ�λ����������
extern unsigned char g_ucUpBackingID;       // �Ϲ�λ���ÿ�����
extern unsigned char g_ucDownWorkingID;     // �¹�λ����������
extern unsigned char g_ucDownBackingID;     // �¹�λ���ÿ�����
extern unsigned char g_ucaCardIsReady[4];   // ������
extern unsigned char g_ucaFaultCode[4];     // �����Ƿ���δ����Ĺ���
extern unsigned char g_ucaDeviceIsSTBY[4];  // �����������ڴ���(Standby)״̬��,��������,�����յ�������Ϣ����,ֻҪ�п�û�б�ȡ��,��ʹ�յ�������Ϣ,Ҳ���ٷ���.
extern unsigned char g_ucaMechineExist[4];  // �����Ƿ���ڲ�ͨ������



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

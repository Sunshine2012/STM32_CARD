#include <includes.h>

// ����֡,Ӧ��֡��5�ֽ�     
// ��Ӧ����Ϣ(30H)֡
// ��Ӧ����Ϣ(31H)֡

/* ����PC->*/
// �ϵ���Ϣ(41H)֡


typedef enum frame
{
    POSITIVE_ACK            = '0';      /* ��Ӧ����Ϣ(30H)֡ */
    NAGATIVE_ACK            = '1';      /* ��Ӧ����Ϣ(31H)֡ */
    
    /* �Զ���������PC������Ϣ֡��PC��Ӧ�� */
    CARD_MACHINE_POWER_ON   = 'A';      /* �����ϵ���Ϣ(41H)֡ */
    CARD_MACHINE_STATUES    = 'B';      /* ״̬��Ϣ(42H)֡ */
    CARD_SPIT_OUT_CARD      = 'C';      /* �ѳ�����Ϣ(43H)֡ */
    CARD_KEY_PRESS          = 'D';      /* ��ťȡ����Ϣ(44H)֡ */
    CARD_TAKE_AWAY          = 'E';      /* ����ȡ����Ϣ(45H)֡ */
    CARD_REPORT_STATUES     = 'F';      /* �ϱ����к���Ϣ(46H) ֡ */
    
    /* PC�����Զ�����������Ϣ֡�뿨��Ӧ�� */
    PC_INIT_MECHINE         = 'a';      /* ��ʼ��������Ϣ(61H)֡ */
    PC_SPIT_OUT_CARD        = 'b';      /* ������Ϣ(62H)֡ */
    PC_SPIT_OUT_CARD        = 'C';      /* ������Ϣ(63H)֡ */
    
    
    machine
}FRAME;


typedef struct rsctl
{
    CPU_INT08U STX;                 // 
    CPU_INT08U RSCTL;
    CPU_INT08U CTL;
    CPU_INT08U RSCTL;
    CPU_INT08U ETX;
}RSCTL;

// ״̬��Ϣ(42H)֡
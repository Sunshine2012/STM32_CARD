#ifndef __FRAME_H
#define __FRAME_H

#include <includes.h>

typedef enum frame
{
    FRAME_START             = '<',      /* ֡��ʼ */
    POSITIVE_ACK            = '0',      /* ��Ӧ����Ϣ(30H)֡ 4�ֽ� */
    NAGATIVE_ACK            = '1',      /* ��Ӧ����Ϣ(31H)֡ 4�ֽ� */

    /* �Զ���������PC������Ϣ֡��PC��Ӧ�� */
    CARD_MACHINE_POWER_ON   = 'A',      /* �����ϵ���Ϣ(41H)֡ 4�ֽ� */
    CARD_MACHINE_STATUES    = 'B',      /* ״̬��Ϣ(42H)֡ 30�ֽ� */
    CARD_SPIT_OUT_CARD      = 'C',      /* �ѳ�����Ϣ(43H)֡ 6�ֽ� */
    CARD_KEY_PRESS          = 'D',      /* ��ťȡ����Ϣ(44H)֡ 6�ֽ� */
    CARD_TAKE_AWAY          = 'E',      /* ����ȡ����Ϣ(45H)֡ 6�ֽ� */
    CARD_REPORT_STATUES     = 'F',      /* �ϱ����к���Ϣ(46H)֡ */

    /* PC�����Զ�����������Ϣ֡�뿨��Ӧ�� */
    PC_INIT_MECHINE         = 'a',      /* ��ʼ��������Ϣ(61H)֡ */
    PC_SPIT_OUT_CARD        = 'b',      /* ������Ϣ(62H)֡ */
    PC_CARD_FALSE           = 'c',      /* ������Ϣ(63H)֡ */
    PC_QUERY_CARD_MECHINE   = 'e',      /* ��ѯ����״̬(65H)֡ */
    PC_QUERY_CARD_CLIP      = 'f',      /* ��ѯ����(66H)֡ */
    PC_SET_CARD_NUM         = 'g',      /* ���ÿ��п���(67H)֡ */
    FRAME_END               = '>',      /* ֡���� */
}FRAME;

// Ӧ����Ϣ֡
typedef struct rsctl
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ���ͣ���(30H), ��(31H) */
    CPU_INT08U ETX;                 /* ֡���� */
}RSCTL_FREME;

// PC->�Զ�������
// ��ʼ��֡
typedef struct init
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U ETX;                 /* ֡���� */
}INIT_FREME;

void  AppTaskWriteFrame ( void * p_arg );
void  AppTaskReadFrame ( void * p_arg );


#endif /* __FRAME_H */

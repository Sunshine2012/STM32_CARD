#ifndef __FRAME_H
#define __FRAME_H

#include <includes.h>

#define FRAME_4             4      /* ֡�ֽڳ��� */
#define FRAME_6             6      /* ֡�ֽڳ��� */

typedef enum frame
{
    FRAME_START             = '<',      /* ֡��ʼ */
    POSITIVE_ACK            = '0',      /* ��Ӧ����Ϣ(30H)֡            4�ֽ� */
    NAGATIVE_ACK            = '1',      /* ��Ӧ����Ϣ(31H)֡            4�ֽ� */

    /* �Զ���������PC������Ϣ֡��PC��Ӧ�� */
    CARD_MACHINE_POWER_ON   = 'A',      /* �����ϵ���Ϣ(41H)֡          4�ֽ� */
    CARD_MACHINE_STATUES    = 'B',      /* ״̬��Ϣ(42H)֡             30�ֽ� */
    CARD_SPIT_OUT           = 'C',      /* �ѳ�����Ϣ(43H)֡            6�ֽ� */
    CARD_KEY_PRESS          = 'D',      /* ��ťȡ����Ϣ(44H)֡          6�ֽ� */
    CARD_TAKE_AWAY          = 'E',      /* ����ȡ����Ϣ(45H)֡          6�ֽ� */
    CARD_REPORT_SPIT_STATUES= 'F',      /* �ϱ����кű����Ϣ(46H)֡   36�ֽ� */

    /* PC�����Զ�����������Ϣ֡�뿨��Ӧ�� */
    PC_INIT_MECHINE         = 'a',      /* ��ʼ��������Ϣ(61H)֡       20�ֽ� */
    PC_SPIT_OUT_CARD        = 'b',      /* ������Ϣ(62H)֡              5�ֽ� */
    PC_CARD_FALSE           = 'c',      /* ������Ϣ(63H)֡              5�ֽ� */
    PC_QUERY_CARD_MECHINE   = 'e',      /* ��ѯ����״̬(65H)֡          5�ֽ� */
    PC_QUERY_CARD_CLIP      = 'f',      /* ��ѯ����(66H)֡              5�ֽ� */
    PC_SET_CARD_NUM         = 'g',      /* ���ÿ��п���(67H)֡          8�ֽ� */
    FRAME_END               = '>',      /* ֡���� */
}FRAME;

// Ӧ����Ϣ֡ 4�ֽ�
typedef struct rsctl
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ���ͣ���(30H), ��(31H) */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}RSCTL_FREME;

///====================================
// �Զ�������->PC
// �����ϵ���Ϣ֡ 4�ֽ�
typedef struct card_machine_power_on
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}CARD_MACHINE_POWER_ON_FREME;

// ������״̬��Ϣ 6�ֽ�
typedef struct card_spit_statues
{
    CPU_INT08U status;                  /* ����״̬ */
    CPU_INT08U spitIsExist;             /* �����Ƿ�װ�� */
    CPU_INT08U cardNum[3];              /* �����п�����ֵ�����������ϵĿ� */
    CPU_INT08U antHasCard;              /* ��λ��Ϣ */
}CARD_SPIT_STATUES;


// ״̬��Ϣ֡ 30�ֽ�
typedef struct card_machine_statues
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U UP_SPIT_IS_OK;       /* �Ϲ�λ��ǰ�������� */
    CPU_INT08U DOWN_SPIT_IS_OK;     /* �¹�λ��ǰ�������� */
    CARD_SPIT_STATUES CARD_MECHINE1;/* ��λ��Ϣ */
    CARD_SPIT_STATUES CARD_MECHINE2;/* ��λ��Ϣ */
    CARD_SPIT_STATUES CARD_MECHINE3;/* ��λ��Ϣ */
    CARD_SPIT_STATUES CARD_MECHINE4;/* ��λ��Ϣ */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}CARD_MACHINE_STATUES_FRAME;


// 6�ֽ�
typedef struct card_mechine_to_pc
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U CARD_MECHINE;        /* ��λ��Ϣ */
    CPU_INT08U MECHINE_ID;          /* ������� */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}CARD_MECHINE_TO_PC_FRAME;

// �ϱ��������к�״̬֡ 29
typedef struct card_report_spit_statues
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U CARD_SPIT1[8];        /* 1#�����ڿ��б�� */
    CPU_INT08U CARD_SPIT2[8];        /* 2#�����ڿ��б�� */
    CPU_INT08U CARD_SPIT3[8];        /* 3#�����ڿ��б�� */
    CPU_INT08U CARD_SPIT4[8];        /* 4#�����ڿ��б�� */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}CARD_REPORT_SPIT_STATUES_FRAME;

///====================================
// PC->�Զ�������
// ��ʼ��֡ 20�ֽ�
typedef struct pc_to_card_init
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U CARD_NUM[3];         /* ���������� */
    CPU_INT08U DATE[14];            /* ʱ�� */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}PC_TO_CARD_INIT_FREME;

// 5�ֽ�
typedef struct pc_to_card_mechine
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U DATA;                /* ������ */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}PC_TO_CARD_MECHINE_FRAME;

// ���ÿ��п���֡ 8�ֽ�
typedef struct pc_set_card_num
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U CARD_SPIT_ID[1];     /* ���к� */
    CPU_INT08U CARD_NUM[3];         /* ���п��� */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}PC_SET_CARD_NUM_FRAME;
///====================================


// ������Ϣ�ṹ��
typedef struct msg
{
    CPU_INT08U CTL;                 /* ���ͣ���(30H), ��(31H) */
    char Msg[40];
    CPU_INT08U END;
}Print_msg;


extern Print_msg                    g_taPri_msg[16];                    /**/

extern RSCTL_FREME                  g_tP_RsctlFrame;                    /* ��Ӧ����Ϣ(30H)֡            4�ֽ� */
extern RSCTL_FREME                  g_tN_RsctlFrame;                    /* ��Ӧ����Ϣ(31H)֡            4�ֽ� */

extern CARD_MACHINE_POWER_ON_FREME      g_tCardMechinePowerOnFrame;         /* �����ϵ���Ϣ(41H)֡          4�ֽ� */
extern CARD_MACHINE_STATUES_FRAME       g_tCardMechineStatusFrame;          /* ״̬��Ϣ(42H)֡             30�ֽ� */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardSpitOutFrame;                /* �ѳ�����Ϣ(43H)֡            6�ֽ� */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardKeyPressFrame;               /* ��ťȡ����Ϣ(44H)֡          6�ֽ� */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardTakeAwayFrame;               /* ����ȡ����Ϣ(45H)֡          6�ֽ� */
extern CARD_REPORT_SPIT_STATUES_FRAME   g_tCardReportSpitStatusFrame;       /* �ϱ����кű����Ϣ(46H)֡   36�ֽ� */


extern PC_TO_CARD_INIT_FREME            g_tPcToCardInitFrame;               /* ��ʼ��������Ϣ(61H)֡       20�ֽ� */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcSpitOutCardFrame;              /* ������Ϣ(62H)֡              5�ֽ� */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcSpitOutCardFrame;              /* ������Ϣ(63H)֡              5�ֽ� */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardMechineFrame;         /* ��ѯ����״̬(65H)֡          5�ֽ� */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardCpipFrame;            /* ��ѯ����(66H)֡              5�ֽ� */
extern PC_SET_CARD_NUM_FRAME            g_tPcSetCardNumFrame;               /* ���ÿ��п���(67H)֡          8�ֽ� */

CPU_INT08U  AnalyzeCANFrame ( void * p_arg );
CPU_INT08U  AnalyzeUartFrame ( void * p_arg );


#endif /* __FRAME_H */

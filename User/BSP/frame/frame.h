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
    CARD_MACHINE_STATUES    = 'B',      /* ״̬��Ϣ(42H)֡              30�ֽ� */
    CARD_SPIT_OUT_CARD      = 'C',      /* �ѳ�����Ϣ(43H)֡            6�ֽ� */
    CARD_KEY_PRESS          = 'D',      /* ��ťȡ����Ϣ(44H)֡          6�ֽ� */
    CARD_TAKE_AWAY          = 'E',      /* ����ȡ����Ϣ(45H)֡          6�ֽ� */
    CARD_REPORT_STATUES     = 'F',      /* �ϱ����к���Ϣ(46H)֡        29�ֽ�*/

    /* PC�����Զ�����������Ϣ֡�뿨��Ӧ�� */
    PC_INIT_MECHINE         = 'a',      /* ��ʼ��������Ϣ(61H)֡        20�ֽ�*/
    PC_SPIT_OUT_CARD        = 'b',      /* ������Ϣ(62H)֡              5�ֽ�*/
    PC_CARD_FALSE           = 'c',      /* ������Ϣ(63H)֡              5�ֽ�*/
    PC_QUERY_CARD_MECHINE   = 'e',      /* ��ѯ����״̬(65H)֡          5�ֽ�*/
    PC_QUERY_CARD_CLIP      = 'f',      /* ��ѯ����(66H)֡              5�ֽ�*/
    PC_SET_CARD_NUM         = 'g',      /* ���ÿ��п���(67H)֡          8�ֽ�*/
    FRAME_END               = '>',      /* ֡���� */
}FRAME;

// Ӧ����Ϣ֡
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
// �����ϵ���Ϣ֡
typedef struct card_machine_power_on
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}CARD_MACHINE_POWER_ONFREME;

// ����״̬��Ϣ
typedef struct card_spit_statues
{
    CPU_INT08U up_spit;                 /* �Ϲ�λ��ǰ�������� */
    CPU_INT08U down_spit;               /* �¹�λ��ǰ�������� */
    CPU_INT08U status;                  /* ����״̬ */
    CPU_INT08U spitIsExist;             /* �����Ƿ�װ�� */
    CPU_INT08U cardNum[3];              /* �����п�����ֵ�����������ϵĿ� */
    CPU_INT08U antHasCard;              /* ��λ��Ϣ */
}CARD_SPIT_STATUES;


// ״̬��Ϣ֡
typedef struct card_machine_statues
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CARD_SPIT_STATUES CARD_MECHINE1;/* ��λ��Ϣ */
    CARD_SPIT_STATUES CARD_MECHINE2;/* ��λ��Ϣ */
    CARD_SPIT_STATUES CARD_MECHINE3;/* ��λ��Ϣ */
    CARD_SPIT_STATUES CARD_MECHINE4;/* ��λ��Ϣ */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}CARD_MACHINE_STATUES_FRAME;


//
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


///====================================
// PC->�Զ�������
// ��ʼ��֡
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

//
typedef struct pc_to_card_mechine
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U DATA;                /* ������ */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}PC_TO_CARD_MECHINE_FRAME;

// ���ÿ��п���֡
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
    char Msg[100];
    CPU_INT08U END;
}Print_msg;


extern Print_msg                    gta_pri_msg[16];                    /**/
extern RSCTL_FREME                  gt_P_RsctlFrame;                    /* ��Ӧ����Ϣ(30H)֡            4�ֽ� */
extern RSCTL_FREME                  gt_N_RsctlFrame;                    /* ��Ӧ����Ϣ(31H)֡            4�ֽ� */

extern PC_TO_CARD_INIT_FREME        gt_PcToCardInitFrame;               /* ��ʼ��������Ϣ(61H)֡        20�ֽ�*/
extern PC_TO_CARD_MECHINE_FRAME     gt_PcSpitOutCardFrame;              /* ������Ϣ(62H)֡              5�ֽ�*/
extern PC_TO_CARD_MECHINE_FRAME     gt_PcSpitOutCardFrame;              /* ������Ϣ(63H)֡              5�ֽ�*/
extern PC_TO_CARD_MECHINE_FRAME     gt_PcQuetyCardMechineFrame;         /* ��ѯ����״̬(65H)֡          5�ֽ�*/
extern PC_TO_CARD_MECHINE_FRAME     gt_PcQuetyCardCpipFrame;            /* ��ѯ����(66H)֡              5�ֽ�*/
extern PC_SET_CARD_NUM_FRAME        gt_PcSetCardNumFrame;               /* ���ÿ��п���(67H)֡          8�ֽ�*/

 //extern PC_INIT_FREME                gt_PCInitFrame;

CPU_INT08U  AnalyzeCANFrame( CPU_INT08U * p_arg );
CPU_INT08U  AnalyzeUartFrame ( CPU_INT08U * p_arg );


#endif /* __FRAME_H */

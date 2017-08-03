#ifndef __FRAME_H
#define __FRAME_H

#include <includes.h>

#define FRAME_4             4      /* ֡�ֽڳ��� */
#define FRAME_6             6      /* ֡�ֽڳ��� */

extern CPU_INT08U g_ucSerNum;      // ֡���    ȫ��

typedef enum FRAME
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
    PC_BAD_CARD             = 'c',      /* ������Ϣ(63H)֡              5�ֽ� */
    PC_QUERY_CARD_MECHINE   = 'e',      /* ��ѯ����״̬(65H)֡          5�ֽ� */
    PC_QUERY_CARD_CLIP      = 'f',      /* ��ѯ����(66H)֡              5�ֽ� */
    PC_SET_CARD_NUM         = 'g',      /* ���ÿ��п���(67H)֡          8�ֽ� */
    FRAME_END               = '>',      /* ֡���� */
}FRAME;

// Ӧ����Ϣ֡ 4�ֽ�
typedef struct RSCTL_FREME
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
typedef struct CARD_MACHINE_POWER_ON_FREME
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}CARD_MACHINE_POWER_ON_FREME;

// ������״̬��Ϣ 6�ֽ�
typedef struct CARD_SPIT_STATUES
{
    CPU_INT08U status;                  /* ����״̬ */
    CPU_INT08U spitIsExist;             /* �����Ƿ�װ�� */
    CPU_INT08U cardNum[3];              /* �����п�����ֵ�����������ϵĿ� */
    CPU_INT08U antHasCard;              /* ��λ��Ϣ */
}CARD_SPIT_STATUES;


// ״̬��Ϣ֡ 30�ֽ�
typedef struct CARD_MACHINE_STATUES_FRAME
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
typedef struct CARD_MECHINE_TO_PC_FRAME
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
typedef struct CARD_REPORT_SPIT_STATUES_FRAME
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
typedef struct PC_TO_CARD_INIT_FREME
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
typedef struct PC_TO_CARD_MECHINE_FRAME
{
    CPU_INT08U STX;                 /* ֡��ʼ */
    CPU_INT08U RSCTL;               /* ֡��� */
    CPU_INT08U CTL;                 /* ֡���� */
    CPU_INT08U DATA;                /* ������ */
    CPU_INT08U ETX;                 /* ֡���� */
    CPU_INT08U END;
}PC_TO_CARD_MECHINE_FRAME;

// ���ÿ��п���֡ 8�ֽ�
typedef struct PC_SET_CARD_NUM_FRAME
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
typedef struct Print_msg
{
    CPU_INT08U CTL;                 /* ���ͣ���(30H), ��(31H) */
    char Msg[40];
    CPU_INT08U END;
}Print_msg;

// ����
typedef enum CMD
{
    // 00H--0FH����ʼ������ʱ������Ϣ����
    CARD_MACHINE_INIT       = 0x01,                 /* (����--������)��ʼ������ */
    CARD_MACHINE_INIT_ACK   = 0x02,                 /* (����--������) ��ʼ���ظ���������״̬�͹������� */
    CYCLE_ASK               = 0x03,                 /* ��ʱѯ�� */
    CYCLE_ACK               = 0x04,                 /* ��ʱ�ظ� */

    // 10H--1FH�������巢������������Ϣ����
    CAR_IS_COMING           = 0x11,                 /* ������֪ͨ */
    MACHINE_STATUES         = 0x12,                 /* ָ����λ�������������״̬���� */
    WRITE_CARD_STATUS       = 0x13,                 /* д�������������״̬���� */
    YUETONG_CARD_HAS_READ   = 0x14,                 /* ��ͨ���Ѷ���֪ͨ  */

    // 20H--2FH�������巢������������Ϣ����
    MACHINE_CHECK_CARD      = 0x21,                 /* ָ����λ�鿨 */
    KEY_PRESS               = 0x22,                 /* ˾���Ѱ��� */
    CARD_SPIT_NOTICE        = 0x23,                 /* ����֪ͨ��������״̬�͹������� */
    CARD_TAKE_AWAY_NOTICE   = 0x24,                 /* ���ѱ�ȡ��֪ͨ */

    // 30H--3FH���ܵ���������Ϣ
    DEBUG_ALL               = 0x30,

    /*
    40H������ѭ������
    41H��ѭ����һ�ſ�
    42H����һ�źÿ�
    43H����һ�Ż���
    43H����һ�ſ�
    44H����ȡ�Ϳ�������״̬
    45H����ȡ���д�����״̬
    46H����ȡȡ��������״̬
    47H����ȡ����״̬
    48H����ȡ������Ϣ
    49H��ָʾ����
    4AH��ָʾ����
    4BH����ȡ���䴫����״̬
    4CH����ȡ�п�������״̬
    4DH�������������һ��
    4EH�������������һ��
    4FH�������������λ
    50H����ʼ������
    */
    // 40H--5FH����������������Ϣ
    CYCLE_SPIT_CARD         = 0x41,                 /* ����ѭ������ */
    SPIT_ONE_CARD           = 0x42,                 /* */


}CMD;

// ������״̬��00--FFH�� (����������������״̬)
typedef enum CARD_AND_MECHINE_STATUS
{
    // 00H--0FH����״̬
    ALL_IS_OK                  = 0x00,                 /* ALL IS OK */

    // 10H--1FH����״̬
    HAS_CARD                   = 0x10,                 /* �п� */
    HAS_NO_CARD                = 0x11,                 /* �޿� */
    CARD_IS_OK                 = 0x12,                 /* �ÿ� */
    CARD_IS_BAD                = 0x13,                 /* ���� */
    HAS_CHECKED_CARD           = 0x14,                 /* ���п� */
    HAS_WRITE_CARD             = 0x15,

    // 20H--2FH����״̬

    // 30H--7FH������

    // 80H--FFH����������״̬
    CYCLE_SPIT_CARD_SUCCESE    = 0x80,                 /* ����ѭ�������ɹ� */
    CYCLE_SPIT_CARD_FAIL       = 0x81,                 /* ����ѭ������ʧ�� */
    SPIT_ONE_CARD_SUCCESS      = 0x82,                 /* ѭ����һ�ſ��ɹ� */
    SPIT_ONE_CARD_FAIL         = 0x83,                 /* ѭ����һ�ſ�ʧ�� */
    TURN_ONE_CARD_SUCCESS      = 0x84,                 /* ��һ�ſ��ɹ� */
    TURN_ONE_CARD_FAIL         = 0x85,                 /* ��һ�ſ�ʧ�� */
    SEND_CARD_SUCCESS          = 0x86,                 /* ��һ�ſ��ɹ� */
    SEND_CARD_FAIL             = 0x87,                 /* ��һ�ſ�ʧ�� */
    SEND_CARD_SENSOR_STATUS    = 0x88,                 /* �Ϳ�������״̬ */
    CARD_BOX_SENSOR_STATUS     = 0x89,                 /* ���д�����״̬ */
    CARD_OUT_SENSOR_STATUS     = 0x8a,                 /* ȡ��������״̬ */

}CARD_AND_MECHINE_STATUS;

// �����룺00--�޹���
typedef enum FAULT_CODE
{
    NO_FAIL                     = 0x00,                 /*  */
    CAN_IS_FAIL                 = 0x01,                 /* CAN���߹��� */
    OUT_CARD_ELE_MOTOR_FAIL     = 0x02,                 /* ����������� */
    TURN_CARD_ELE_MOTOR_FAIL    = 0x03,                 /* ����������� */
    OUT_CARD_SENSOR_FAIL        = 0x04,                 /* �������������� */
    TURN_CARD_SENSOR_FAIL       = 0x05,                 /* �������������� */
    CHECK_CARD_SENSOR_FAIL      = 0x06,                 /* �⿨���������� */
    CARD_BOX_SENSOR_FAIL        = 0x07,                 /* ���䴫�������� */
    CARD_OUT_SENSOR_FAIL        = 0x08,                 /* ȡ������������ */
    CARD_CLIP_SENSOR_FAIL       = 0x09,                 /* ���д��������� */
    TURN_CARD_FAIL              = 0x0a,                 /* ����ʧ��(���ڿ����г�����) */
    RS232_FAIL                  = 0x0b,                 /* RS232ͨ�Ź��� */
    ULTRASONIC_FAIL             = 0x0c,                 /* �״������ */

}FAULT_CODE;


// ����ṹ��
typedef struct Dlg
{
   CPU_INT08U ID;                 /* ���ͣ���(30H), ��(31H) */
   char MsgRow0[35];
   char MsgRow1[35];
   char MsgRow2[35];
   char MsgRow3[35];
}Dlg;


extern Dlg                              g_dlg[];
extern const Print_msg                  g_taPri_msg[];                      /**/

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
extern PC_TO_CARD_MECHINE_FRAME         g_tPcBadCardFrame;                  /* ������Ϣ(63H)֡              5�ֽ� */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardMechineFrame;         /* ��ѯ����״̬(65H)֡          5�ֽ� */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardCpipFrame;            /* ��ѯ����(66H)֡              5�ֽ� */
extern PC_SET_CARD_NUM_FRAME            g_tPcSetCardNumFrame;               /* ���ÿ��п���(67H)֡          8�ֽ� */

CPU_INT08U  AnalyzeCANFrame ( void * p_arg );
CPU_INT08U  AnalyzeUartFrame ( void * p_arg );


#endif /* __FRAME_H */

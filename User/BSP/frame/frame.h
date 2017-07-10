#ifndef __FRAME_H
#define __FRAME_H

#include <includes.h>

#define FRAME_4             4      /* 帧字节长度 */
#define FRAME_6             6      /* 帧字节长度 */

typedef enum frame
{
    FRAME_START             = '<',      /* 帧开始 */
    POSITIVE_ACK            = '0',      /* 正应答信息(30H)帧            4字节 */
    NAGATIVE_ACK            = '1',      /* 负应答信息(31H)帧            4字节 */

    /* 自动发卡机至PC机的信息帧与PC机应答 */
    CARD_MACHINE_POWER_ON   = 'A',      /* 卡机上电信息(41H)帧          4字节 */
    CARD_MACHINE_STATUES    = 'B',      /* 状态信息(42H)帧             30字节 */
    CARD_SPIT_OUT           = 'C',      /* 已出卡信息(43H)帧            6字节 */
    CARD_KEY_PRESS          = 'D',      /* 按钮取卡信息(44H)帧          6字节 */
    CARD_TAKE_AWAY          = 'E',      /* 卡被取走信息(45H)帧          6字节 */
    CARD_REPORT_SPIT_STATUES= 'F',      /* 上报卡夹号编号信息(46H)帧   36字节 */

    /* PC机至自动发卡机的信息帧与卡机应答 */
    PC_INIT_MECHINE         = 'a',      /* 初始化卡机信息(61H)帧       20字节 */
    PC_SPIT_OUT_CARD        = 'b',      /* 出卡信息(62H)帧              5字节 */
    PC_CARD_FALSE           = 'c',      /* 坏卡信息(63H)帧              5字节 */
    PC_QUERY_CARD_MECHINE   = 'e',      /* 查询卡机状态(65H)帧          5字节 */
    PC_QUERY_CARD_CLIP      = 'f',      /* 查询卡夹(66H)帧              5字节 */
    PC_SET_CARD_NUM         = 'g',      /* 设置卡夹卡数(67H)帧          8字节 */
    FRAME_END               = '>',      /* 帧结束 */
}FRAME;

// 应答信息帧 4字节
typedef struct rsctl
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 类型：正(30H), 负(31H) */
    CPU_INT08U ETX;                 /* 帧结束 */
    CPU_INT08U END;
}RSCTL_FREME;

///====================================
// 自动发卡机->PC
// 卡机上电信息帧 4字节
typedef struct card_machine_power_on
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 帧类型 */
    CPU_INT08U ETX;                 /* 帧结束 */
    CPU_INT08U END;
}CARD_MACHINE_POWER_ON_FREME;

// 单卡夹状态信息 6字节
typedef struct card_spit_statues
{
    CPU_INT08U status;                  /* 卡机状态 */
    CPU_INT08U spitIsExist;             /* 卡机是否装上 */
    CPU_INT08U cardNum[3];              /* 卡机中卡计数值，包含天线上的卡 */
    CPU_INT08U antHasCard;              /* 工位信息 */
}CARD_SPIT_STATUES;


// 状态信息帧 30字节
typedef struct card_machine_statues
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 帧类型 */
    CPU_INT08U UP_SPIT_IS_OK;       /* 上工位当前就绪卡机 */
    CPU_INT08U DOWN_SPIT_IS_OK;     /* 下工位当前就绪卡机 */
    CARD_SPIT_STATUES CARD_MECHINE1;/* 工位信息 */
    CARD_SPIT_STATUES CARD_MECHINE2;/* 工位信息 */
    CARD_SPIT_STATUES CARD_MECHINE3;/* 工位信息 */
    CARD_SPIT_STATUES CARD_MECHINE4;/* 工位信息 */
    CPU_INT08U ETX;                 /* 帧结束 */
    CPU_INT08U END;
}CARD_MACHINE_STATUES_FRAME;


// 6字节
typedef struct card_mechine_to_pc
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 帧类型 */
    CPU_INT08U CARD_MECHINE;        /* 工位信息 */
    CPU_INT08U MECHINE_ID;          /* 卡机编号 */
    CPU_INT08U ETX;                 /* 帧结束 */
    CPU_INT08U END;
}CARD_MECHINE_TO_PC_FRAME;

// 上报卡夹序列号状态帧 29
typedef struct card_report_spit_statues
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 帧类型 */
    CPU_INT08U CARD_SPIT1[8];        /* 1#卡机内卡夹编号 */
    CPU_INT08U CARD_SPIT2[8];        /* 2#卡机内卡夹编号 */
    CPU_INT08U CARD_SPIT3[8];        /* 3#卡机内卡夹编号 */
    CPU_INT08U CARD_SPIT4[8];        /* 4#卡机内卡夹编号 */
    CPU_INT08U ETX;                 /* 帧结束 */
    CPU_INT08U END;
}CARD_REPORT_SPIT_STATUES_FRAME;

///====================================
// PC->自动发卡机
// 初始化帧 20字节
typedef struct pc_to_card_init
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 帧类型 */
    CPU_INT08U CARD_NUM[3];         /* 卡夹最大计数 */
    CPU_INT08U DATE[14];            /* 时间 */
    CPU_INT08U ETX;                 /* 帧结束 */
    CPU_INT08U END;
}PC_TO_CARD_INIT_FREME;

// 5字节
typedef struct pc_to_card_mechine
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 帧类型 */
    CPU_INT08U DATA;                /* 数据域 */
    CPU_INT08U ETX;                 /* 帧结束 */
    CPU_INT08U END;
}PC_TO_CARD_MECHINE_FRAME;

// 设置卡夹卡数帧 8字节
typedef struct pc_set_card_num
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 帧类型 */
    CPU_INT08U CARD_SPIT_ID[1];     /* 卡夹号 */
    CPU_INT08U CARD_NUM[3];         /* 卡夹卡数 */
    CPU_INT08U ETX;                 /* 帧结束 */
    CPU_INT08U END;
}PC_SET_CARD_NUM_FRAME;
///====================================


// 常规信息结构体
typedef struct msg
{
    CPU_INT08U CTL;                 /* 类型：正(30H), 负(31H) */
    char Msg[40];
    CPU_INT08U END;
}Print_msg;


extern Print_msg                    g_taPri_msg[16];                    /**/

extern RSCTL_FREME                  g_tP_RsctlFrame;                    /* 正应答信息(30H)帧            4字节 */
extern RSCTL_FREME                  g_tN_RsctlFrame;                    /* 负应答信息(31H)帧            4字节 */

extern CARD_MACHINE_POWER_ON_FREME      g_tCardMechinePowerOnFrame;         /* 卡机上电信息(41H)帧          4字节 */
extern CARD_MACHINE_STATUES_FRAME       g_tCardMechineStatusFrame;          /* 状态信息(42H)帧             30字节 */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardSpitOutFrame;                /* 已出卡信息(43H)帧            6字节 */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardKeyPressFrame;               /* 按钮取卡信息(44H)帧          6字节 */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardTakeAwayFrame;               /* 卡被取走信息(45H)帧          6字节 */
extern CARD_REPORT_SPIT_STATUES_FRAME   g_tCardReportSpitStatusFrame;       /* 上报卡夹号编号信息(46H)帧   36字节 */


extern PC_TO_CARD_INIT_FREME            g_tPcToCardInitFrame;               /* 初始化卡机信息(61H)帧       20字节 */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcSpitOutCardFrame;              /* 出卡信息(62H)帧              5字节 */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcSpitOutCardFrame;              /* 坏卡信息(63H)帧              5字节 */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardMechineFrame;         /* 查询卡机状态(65H)帧          5字节 */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardCpipFrame;            /* 查询卡夹(66H)帧              5字节 */
extern PC_SET_CARD_NUM_FRAME            g_tPcSetCardNumFrame;               /* 设置卡夹卡数(67H)帧          8字节 */

CPU_INT08U  AnalyzeCANFrame ( void * p_arg );
CPU_INT08U  AnalyzeUartFrame ( void * p_arg );


#endif /* __FRAME_H */

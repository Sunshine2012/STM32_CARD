#ifndef __FRAME_H
#define __FRAME_H

#include <includes.h>

#define FRAME_4             4      /* 帧字节长度 */
#define FRAME_6             6      /* 帧字节长度 */

extern CPU_INT08U g_ucSerNum;      // 帧序号    全局

typedef enum FRAME
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
    PC_BAD_CARD             = 'c',      /* 坏卡信息(63H)帧              5字节 */
    PC_QUERY_CARD_MECHINE   = 'e',      /* 查询卡机状态(65H)帧          5字节 */
    PC_QUERY_CARD_CLIP      = 'f',      /* 查询卡夹(66H)帧              5字节 */
    PC_SET_CARD_NUM         = 'g',      /* 设置卡夹卡数(67H)帧          8字节 */
    FRAME_END               = '>',      /* 帧结束 */
}FRAME;

// 应答信息帧 4字节
typedef struct RSCTL_FREME
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
typedef struct CARD_MACHINE_POWER_ON_FREME
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 帧类型 */
    CPU_INT08U ETX;                 /* 帧结束 */
    CPU_INT08U END;
}CARD_MACHINE_POWER_ON_FREME;

// 单卡夹状态信息 6字节
typedef struct CARD_SPIT_STATUES
{
    CPU_INT08U status;                  /* 卡机状态 */
    CPU_INT08U spitIsExist;             /* 卡机是否装上 */
    CPU_INT08U cardNum[3];              /* 卡机中卡计数值，包含天线上的卡 */
    CPU_INT08U antHasCard;              /* 工位信息 */
}CARD_SPIT_STATUES;


// 状态信息帧 30字节
typedef struct CARD_MACHINE_STATUES_FRAME
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
typedef struct CARD_MECHINE_TO_PC_FRAME
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
typedef struct CARD_REPORT_SPIT_STATUES_FRAME
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 帧类型 */
    CPU_INT08U CARD_SPIT1[8];       /* 1#卡机内卡夹编号 */
    CPU_INT08U CARD_SPIT2[8];       /* 2#卡机内卡夹编号 */
    CPU_INT08U CARD_SPIT3[8];       /* 3#卡机内卡夹编号 */
    CPU_INT08U CARD_SPIT4[8];       /* 4#卡机内卡夹编号 */
    CPU_INT08U ETX;                 /* 帧结束 */
    CPU_INT08U END;
}CARD_REPORT_SPIT_STATUES_FRAME;

///====================================
// PC->自动发卡机
// 初始化帧 20字节
typedef struct PC_TO_CARD_INIT_FREME
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
typedef struct PC_TO_CARD_MECHINE_FRAME
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 帧类型 */
    CPU_INT08U DATA;                /* 数据域 */
    CPU_INT08U ETX;                 /* 帧结束 */
    CPU_INT08U END;
}PC_TO_CARD_MECHINE_FRAME;

// 设置卡夹卡数帧 8字节
typedef struct PC_SET_CARD_NUM_FRAME
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
typedef struct Print_msg
{
    CPU_INT08U CTL;
    char Msg[40];
    CPU_INT08U END;
}Print_msg;

// 命令
typedef enum CMD
{
    // 00H--0FH：初始化、定时来往信息命令
    CARD_MACHINE_INIT       = 0x01,                 /* (主机--》卡机)初始化命令 */
    CARD_MACHINE_INIT_ACK   = 0x02,                 /* (卡机--》主机) 初始化回复。内容在状态和故障码内 */
    CYCLE_ASK               = 0x03,                 /* 定时询问 */
    CYCLE_ACK               = 0x04,                 /* 定时回复 */
    SET_MECHINE_STATUS      = 0x05,                 /* 卡机工作状态设置 */
    CAR_NOTIFICATION        = 0x06,                 /* 来车去车通知 */

    // 10H--1FH：主机板发送至卡机板信息命令
    SET_MECHINE_ID          = 0x10,                 /* 设置卡机的ID号 */
    STOP_WORKING            = 0x11,                 /* 停止工作 */
    MACHINE_STATUES         = 0x12,                 /* 指定工位卡情况，内容在状态码内 */
    WRITE_CARD_STATUS       = 0x13,                 /* 写卡情况，内容在状态码内 */
    YUETONG_CARD_HAS_READ   = 0x14,                 /* 粤通卡已读卡通知  */
    SEARCH_CARD_MECHINE     = 0x15,                 /* 搜索询问卡机号和ID码  */
    SET_CARD_COUNT          = 0x16,                 /* 设置IC卡数量  */

    // 20H--2FH：卡机板发送至主机板信息命令
    MACHINE_CHECK_CARD      = 0x21,                 /* 指定工位验卡 */
    KEY_PRESS               = 0x22,                 /* 司机已按键 */
    CARD_SPIT_NOTICE        = 0x23,                 /* 出卡通知，内容在状态和故障码内 */
    CARD_TAKE_AWAY_NOTICE   = 0x24,                 /* 卡已被取走通知 */
    CARD_IS_READY           = 0x25,                 /* 卡已就绪 */
    SERCH_CARD_MECHINE_ACK  = 0x26,                 /* 搜索询问回答 */
    MECHINE_WARNING         = 0x27,                 /* 报警 */
    BAD_CARD_FULL           = 0x28,                 /* 坏卡满报警 */

    // 30H--3FH：总调机命令信息
    DEBUG_ALL               = 0x30,
    ENTER_DEBUG             = 0x31,
    QUIT_DEBUG              = 0x32,

    // 40H--5FH：卡机调机命令信息
    STOP_DEBUG              = 0x40,                 /* 停止调试 */
    CYCLE_SPIT_ONE_CARD     = 0x41,                 /* 循环出一张卡 */
    SPIT_ONE_OK_CARD        = 0x42,                 /* 翻一张好卡 */
    SPIT_ONE_BAD_CARD       = 0x43,                 /* 翻一张坏卡 */
    HOOK_ONE_CARD           = 0x44,                 /* 勾一张卡 */

    //
    SPIT_MOTOR_POSITIVE_STEP= 0x45,                 /* 翻卡电机正单步 */
    SPIT_MOTOR_NEGATIVE_STEP= 0x46,                 /* 翻卡电机反单步 */

    HOOK_MOTOR_POSITIVE_STEP= 0x47,                 /* 勾卡电机正单步 */
    HOOK_MOTOR_NEGATIVE_STEP= 0x48,                 /* 勾卡电机反单步 */

    SPIT_MOTOR_POSITIVE     = 0x49,                 /* 翻卡电机正翻到底 */
    SPIT_MOTOR_NEGATIVE     = 0x4A,                 /* 翻卡电机反翻到底 */
    SPIT_MOTOR_ZERO         = 0x4B,                 /* 翻卡电机回零位 */

    HOOK_MOTOR_POSITIVE     = 0x4C,                 /* 勾卡电机正翻到底 */
    HOOK_MOTOR_NEGATIVE     = 0x4D,                 /* 勾卡电机反翻到底 */
    HOOK_MOTOR_ZERO         = 0x4E,                 /* 勾卡电机回零位 */

    READ_KEY_INFO           = 0x4F,                 /* 读取按键信息 */
    LED_ON                  = 0x50,                 /* 指示灯亮 */
    LED_OFF                 = 0x51,                 /* 指示灯灭 */
    READ_SENSOR             = 0x52,                 /* 读取传感器状态 */




}CMD;

// 卡、机状态：00--FFH： (卡、机、传感器等状态)
typedef enum CARD_AND_MECHINE_STATUS
{
    // 00H--0FH：总状态
    ALL_IS_OK                  = 0x00,                 /* ALL IS OK */

    WORKING_STATUS             = 0x01,                 /* 工作状态 */
    BACKING_STATUS             = 0x02,                 /* 备机状态 */
    DEBUGING_STATUS            = 0x03,                 /* 调试状态 */
                                                       /* 已来车 */
                                                       /* 车已走 */
                                                       /* 使用粤通卡 */


    // 10H--1FH：卡状态
    HAS_CARD                   = 0x10,                 /* 有卡 */
    HAS_NO_CARD                = 0x11,                 /* 无卡 */
    CARD_IS_OK                 = 0x12,                 /* 好卡 */
    CARD_IS_BAD                = 0x13,                 /* 坏卡 */
    HAS_CHECKED_CARD           = 0x14,                 /* 已判卡 */
    HAS_WRITE_CARD             = 0x15,                 /* 已写卡 */
    IS_WORKING                 = 0x20,
    IS_BACKING                 = 0x21,
    // 20H--2FH：机状态

    // 30H--7FH：备用

    // 80H--FFH：调机命令状态
    CYCLE_SPIT_CARD_SUCCESE    = 0x80,                 /* 连续循环出卡成功 */
    CYCLE_SPIT_CARD_FAIL       = 0x81,                 /* 连续循环出卡失败 */
    SPIT_ONE_CARD_SUCCESS      = 0x82,                 /* 循环出一张卡成功 */
    SPIT_ONE_CARD_FAIL         = 0x83,                 /* 循环出一张卡失败 */
    TURN_ONE_CARD_SUCCESS      = 0x84,                 /* 翻一张卡成功 */
    TURN_ONE_CARD_FAIL         = 0x85,                 /* 翻一张卡失败 */
    SEND_CARD_SUCCESS          = 0x86,                 /* 送一张卡成功 */
    SEND_CARD_FAIL             = 0x87,                 /* 送一张卡失败 */
    SEND_CARD_SENSOR_OK        = 0x88,                 /* 送卡传感器状态正常 */
    SEND_CARD_SENSOR_FAULT     = 0x89,                 /* 送卡传感器状态故障 */
    CARD_BOX_SENSOR_OK         = 0x8a,                 /* 卡盒传感器状态正常 */
    CARD_BOX_SENSOR_FAULT      = 0x8b,                 /* 卡盒传感器状态故障 */
    CARD_OUT_SENSOR_OK         = 0x8c,                 /* 取卡传感器状态正常 */
    CARD_OUT_SENSOR_FAULT      = 0x8d,                 /* 取卡传感器状态故障 */

}CARD_AND_MECHINE_STATUS;

// 故障码：00--无故障
typedef enum FAULT_CODE
{
    NO_FAIL                     = 0x00,                 /*  */
    CAN_IS_FAIL                 = 0x01,                 /* CAN总线故障 */
    OUT_CARD_ELE_MOTOR_FAIL     = 0x02,                 /* 出卡电机故障 */
    TURN_CARD_ELE_MOTOR_FAIL    = 0x03,                 /* 翻卡电机故障 */
    OUT_CARD_SENSOR_FAIL        = 0x04,                 /* 出卡传感器故障 */
    TURN_CARD_SENSOR_FAIL       = 0x05,                 /* 翻卡传感器故障 */
    CHECK_CARD_SENSOR_FAIL      = 0x06,                 /* 测卡传感器故障 */
    CARD_BOX_SENSOR_FAIL        = 0x07,                 /* 卡箱传感器故障 */
    CARD_OUT_SENSOR_FAIL        = 0x08,                 /* 取卡传感器故障 */
    CARD_CLIP_SENSOR_FAIL       = 0x09,                 /* 卡盒传感器故障 */
    TURN_CARD_FAIL              = 0x0a,                 /* 翻卡失败(卡在卡盒中出不来) */
    RS232_FAIL                  = 0x0b,                 /* RS232通信故障 */
    ULTRASONIC_FAIL             = 0x0c,                 /* 雷达测距故障 */

}FAULT_CODE;

extern const Print_msg                  g_taPri_msg[];                      /**/

extern RSCTL_FREME                      g_tP_RsctlFrame;                    /* 正应答信息(30H)帧            4字节 */
extern RSCTL_FREME                      g_tN_RsctlFrame;                    /* 负应答信息(31H)帧            4字节 */

extern CARD_MACHINE_POWER_ON_FREME      g_tCardMechinePowerOnFrame;         /* 卡机上电信息(41H)帧          4字节 */
extern CARD_MACHINE_STATUES_FRAME       g_tCardMechineStatusFrame;          /* 状态信息(42H)帧             30字节 */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardSpitOutFrame;                /* 已出卡信息(43H)帧            6字节 */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardKeyPressFrame;               /* 按钮取卡信息(44H)帧          6字节 */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardTakeAwayFrame;               /* 卡被取走信息(45H)帧          6字节 */
extern CARD_REPORT_SPIT_STATUES_FRAME   g_tCardReportSpitStatusFrame;       /* 上报卡夹号编号信息(46H)帧   36字节 */


extern PC_TO_CARD_INIT_FREME            g_tPcToCardInitFrame;               /* 初始化卡机信息(61H)帧       20字节 */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcSpitOutCardFrame;              /* 出卡信息(62H)帧              5字节 */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcBadCardFrame;                  /* 坏卡信息(63H)帧              5字节 */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardMechineFrame;         /* 查询卡机状态(65H)帧          5字节 */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardCpipFrame;            /* 查询卡夹(66H)帧              5字节 */
extern PC_SET_CARD_NUM_FRAME            g_tPcSetCardNumFrame;               /* 设置卡夹卡数(67H)帧          8字节 */

CPU_INT08U  AnalyzeCANFrame ( void * p_arg );
CPU_INT08U  AnalyzeUartFrame ( void * p_arg );
CPU_INT08U * CheckShowStatusMsg (CPU_INT08U ch);
CPU_INT08U * CheckShowMsg (CPU_INT08U ch);
CPU_INT08U * CheckPriMsg (CPU_INT08U ch);
void copyMenu (CPU_INT08U num, CPU_INT08U id, CPU_INT08U values, CPU_INT08U addr, CPU_INT08U count);
void copyStatusMsg (CPU_INT08U num, CPU_INT08U id, CPU_INT08U values, CPU_INT08U addr, CPU_INT08U count);
void copyWarningMsg (CPU_INT08U num, CPU_INT08U cmd, CPU_INT08U values, CPU_INT08U addr, CPU_INT08U count);

#endif /* __FRAME_H */

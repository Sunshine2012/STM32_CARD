#ifndef __FRAME_H
#define __FRAME_H

#include <includes.h>

typedef enum frame
{
    FRAME_START             = '<',      /* 帧开始 */
    POSITIVE_ACK            = '0',      /* 正应答信息(30H)帧 4字节 */
    NAGATIVE_ACK            = '1',      /* 负应答信息(31H)帧 4字节 */

    /* 自动发卡机至PC机的信息帧与PC机应答 */
    CARD_MACHINE_POWER_ON   = 'A',      /* 卡机上电信息(41H)帧 4字节 */
    CARD_MACHINE_STATUES    = 'B',      /* 状态信息(42H)帧 30字节 */
    CARD_SPIT_OUT_CARD      = 'C',      /* 已出卡信息(43H)帧 6字节 */
    CARD_KEY_PRESS          = 'D',      /* 按钮取卡信息(44H)帧 6字节 */
    CARD_TAKE_AWAY          = 'E',      /* 卡被取走信息(45H)帧 6字节 */
    CARD_REPORT_STATUES     = 'F',      /* 上报卡夹号信息(46H)帧 */

    /* PC机至自动发卡机的信息帧与卡机应答 */
    PC_INIT_MECHINE         = 'a',      /* 初始化卡机信息(61H)帧 */
    PC_SPIT_OUT_CARD        = 'b',      /* 出卡信息(62H)帧 */
    PC_CARD_FALSE           = 'c',      /* 坏卡信息(63H)帧 */
    PC_QUERY_CARD_MECHINE   = 'e',      /* 查询卡机状态(65H)帧 */
    PC_QUERY_CARD_CLIP      = 'f',      /* 查询卡夹(66H)帧 */
    PC_SET_CARD_NUM         = 'g',      /* 设置卡夹卡数(67H)帧 */
    FRAME_END               = '>',      /* 帧结束 */
}FRAME;

// 应答信息帧
typedef struct rsctl
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 类型：正(30H), 负(31H) */
    CPU_INT08U ETX;                 /* 帧结束 */
}RSCTL_FREME;

// PC->自动发卡机
// 初始化帧
typedef struct init
{
    CPU_INT08U STX;                 /* 帧开始 */
    CPU_INT08U RSCTL;               /* 帧序号 */
    CPU_INT08U CTL;                 /* 帧类型 */
    CPU_INT08U ETX;                 /* 帧结束 */
}INIT_FREME;

void  AppTaskWriteFrame ( void * p_arg );
void  AppTaskReadFrame ( void * p_arg );


#endif /* __FRAME_H */

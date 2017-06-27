#include <includes.h>

// 控制帧,应答帧，5字节     
// 正应答信息(30H)帧
// 负应答信息(31H)帧

/* 卡机PC->*/
// 上电信息(41H)帧


typedef enum frame
{
    POSITIVE_ACK            = '0';      /* 正应答信息(30H)帧 */
    NAGATIVE_ACK            = '1';      /* 负应答信息(31H)帧 */
    
    /* 自动发卡机至PC机的信息帧与PC机应答 */
    CARD_MACHINE_POWER_ON   = 'A';      /* 卡机上电信息(41H)帧 */
    CARD_MACHINE_STATUES    = 'B';      /* 状态信息(42H)帧 */
    CARD_SPIT_OUT_CARD      = 'C';      /* 已出卡信息(43H)帧 */
    CARD_KEY_PRESS          = 'D';      /* 按钮取卡信息(44H)帧 */
    CARD_TAKE_AWAY          = 'E';      /* 卡被取走信息(45H)帧 */
    CARD_REPORT_STATUES     = 'F';      /* 上报卡夹号信息(46H) 帧 */
    
    /* PC机至自动发卡机的信息帧与卡机应答 */
    PC_INIT_MECHINE         = 'a';      /* 初始化卡机信息(61H)帧 */
    PC_SPIT_OUT_CARD        = 'b';      /* 出卡信息(62H)帧 */
    PC_SPIT_OUT_CARD        = 'C';      /* 坏卡信息(63H)帧 */
    
    
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

// 状态信息(42H)帧
#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#include <includes.h>

extern unsigned int g_uiSerNum; // 帧序号,全局,卡机与主机之间的帧序号


typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


//volatile TestStatus TestRx;



unsigned char myCANTransmit (void * p_Msg, unsigned char mechine_id, unsigned char boxNum, unsigned char cmd, unsigned char status,
                      unsigned char data_H, unsigned char data_L, unsigned char errNum);

static void CAN_RCC_Config(void);
static void CAN_GPIO_Config(void);
static void CAN_NVIC_Config(void);

void CAN_init( void );
void CAN_Interrupt(void);
void CAN_Polling(void);
void CANTransmit (void * p_Msg);

void USB_LP_CAN1_RX0_IRQHandler(void);


extern CanTxMsg gt_TxMessage;      // CAN发送数据缓存

#endif

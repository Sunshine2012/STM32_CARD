#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__


#include <includes.h>

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


//volatile TestStatus TestRx;



unsigned char MyCANTransmit (void * p_Msg, unsigned char mechine_id, unsigned char boxNum, unsigned char cmd, unsigned char status, 
                      unsigned char data_H, unsigned char data_L, unsigned char errNum);

static void CAN_RCC_Config(void);
static void CAN_GPIO_Config(void);
static void CAN_NVIC_Config(void);

void CAN_init( void );
void CAN_Interrupt(void);
void CAN_Polling(void);
void CANTransmit (void * p_Msg);

void USB_LP_CAN1_RX0_IRQHandler(void);


extern CanTxMsg gt_TxMessage;      // CAN·¢ËÍÊý¾Ý»º´æ

#endif

#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__


#include <includes.h>

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


//volatile TestStatus TestRx;



void MyCANTransmit (void);

static void CAN_RCC_Config(void);
static void CAN_GPIO_Config(void);
static void CAN_NVIC_Config(void);

void CAN_init( void );
void CAN_Interrupt(void);
void CAN_Polling(void);

void USB_LP_CAN1_RX0_IRQHandler(void);
#endif

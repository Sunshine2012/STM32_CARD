#ifndef __BSP_MATRIX_KEY_H_
#define __BSP_MATRIX_KEY_H_

#include "stm32f10x.h"
/* 4*4矩阵键盘 */
void matrix_keyboard_init(void);
u8 matrix_update_key(void);
//extern unsigned char key[4][4];

#endif

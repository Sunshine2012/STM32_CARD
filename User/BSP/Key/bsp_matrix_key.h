#ifndef __BSP_MATRIX_KEY_H_
#define __BSP_MATRIX_KEY_H_

#include "stm32f10x.h"

typedef enum KEY_VALUES
{
    KEY_UP      = 11,
    KEY_DOWN    = 21,
    KEY_LEFT    = 31,
    KEY_RIGHT   = 41,

    KEY_ENTRY   = 12,
    KEY_OK      = 22,
    KEY_CANCEL  = 32,
    KEY_QUIT    = 42,

    KEY_NUL    = 0xff

}KEY_VALUES;

extern unsigned char g_ucKeyValues;      // ��ǰ����ֵ,ȫ��
extern unsigned char g_ucKeyContinu;     // ���������ı�־


/* 4*4������� */
void matrix_keyboard_init(void);
u8 matrix_update_key(void);
extern unsigned char matrix_key[4][4];

#endif

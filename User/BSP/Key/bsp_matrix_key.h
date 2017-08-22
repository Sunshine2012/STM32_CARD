#ifndef __BSP_MATRIX_KEY_H_
#define __BSP_MATRIX_KEY_H_

#include "stm32f10x.h"
//#define OLD_KEY
#define NEW_KEY     /* 新按键 */

#ifdef OLD_KEY
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

    KEY_NUL     = 0xff,

}KEY_VALUES;

#else

typedef enum KEY_VALUES
{
    KEY_UP      = 0x29,//41
    KEY_DOWN    = 0x1f,//31
    KEY_LEFT    = 0x15,//21
    KEY_RIGHT   = 0x0b,//11

    KEY_ENTRY   = 0x2A,//42
    KEY_OK      = 0x20,//32
    KEY_CANCEL  = 0x16,//22
    KEY_QUIT    = 0x0c,//12

    KEY_NUL    = 0xff,

}KEY_VALUES;

#endif

extern unsigned char g_ucKeyValues;      // 当前按键值,全局
extern unsigned char g_ucKeyContinu;     // 连续按键的标志


/* 4*4矩阵键盘 */
void matrix_keyboard_init(void);
u8 matrix_update_key(void);
extern unsigned char matrix_key[4][4];

#endif

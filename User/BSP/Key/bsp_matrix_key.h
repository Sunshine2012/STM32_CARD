#ifndef __BSP_MATRIX_KEY_H_
#define __BSP_MATRIX_KEY_H_

#include "stm32f10x.h"
//#define OLD_KEY   1
//#define NEW_KEY   1  /* 新按键 */
//#define NEW_BOARD_NEW_KEY  1   /* 新板新按键 */
#define NEW_BOARD_NEW_KEY_JTAG  1   /* 新板新按键JTAG使能 */

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

#elif NEW_KEY

typedef enum KEY_VALUES
{
    KEY_UP      = 0x29,
    KEY_DOWN    = 0x1f,
    KEY_LEFT    = 0x15,
    KEY_RIGHT   = 0x0b,

    KEY_ENTRY   = 0x2A,
    KEY_OK      = 0x20,
    KEY_CANCEL  = 0x16,
    KEY_QUIT    = 0x0c,

    KEY_NUL     = 0xff,

}KEY_VALUES;

#elif NEW_BOARD_NEW_KEY

typedef enum KEY_VALUES
{
    KEY_UP      = 0x0c,
    KEY_DOWN    = 0x16,
    KEY_LEFT    = 0x20,
    KEY_RIGHT   = 0x2A,

    KEY_ENTRY   = 0x0b,
    KEY_OK      = 0x15,
    KEY_CANCEL  = 0x1f,
    KEY_QUIT    = 0x29,

    KEY_NUL     = 0xff,

}KEY_VALUES;

#elif NEW_BOARD_NEW_KEY_JTAG

typedef enum KEY_VALUES
{
    KEY_UP      = 0x0c,
    KEY_DOWN    = 0x16,
    KEY_LEFT    = 0x20,
    KEY_RIGHT   = 0x2A,

    KEY_ENTRY   = 0x0b,
    KEY_OK      = 0x15,
    KEY_CANCEL  = 0x1f,
    KEY_QUIT    = 0x29,

    KEY_NUL     = 0xff,

}KEY_VALUES;

#endif

extern unsigned char g_ucKeyValues;      // 当前按键值,全局
extern unsigned char g_ucKeyContinu;     // 连续按键的标志


/* 4*4矩阵键盘 */
void matrix_keyboard_init(void);
u8 matrix_update_key(void);
extern unsigned char matrix_key[4][4];

#endif

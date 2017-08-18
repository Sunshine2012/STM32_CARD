#include "bsp_matrix_key.h"
#include "includes.h"

/**************矩阵键盘.c文件*****************************/
unsigned char matrix_key[4][4];
unsigned char g_ucKeyValues = KEY_NUL;      // 当前按键值,全局
struct io_port
{
    GPIO_TypeDef *GPIO_x;
    unsigned short GPIO_pin;
};
static struct io_port matrix_key_output[4] =
{
    {GPIOB, GPIO_Pin_4 },   {GPIOA, GPIO_Pin_13},
    {GPIOA, GPIO_Pin_14},   {GPIOA, GPIO_Pin_15}
};
static struct io_port matrix_key_input[4] =
{
    {GPIOB, GPIO_Pin_3}, {GPIOB, GPIO_Pin_2},
    //{GPIOD, GPIO_Pin_6}, {GPIOD, GPIO_Pin_7}
};

void matrix_keyboard_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    unsigned char i;
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );    // 打开时钟,并且打开管脚复用
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);           // 关闭JTAG和SW模式,管脚复用

    /* 键盘行扫描输出线 输出高电平 */
    /* PA0 PA1 PA2 PA3 输出*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* 键盘列扫描输入线 键被按时输入高电平 放开输入低电平 */
    /* PA4 PA5 PA6 PA7 输入*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    for(i = 0; i < 4; i++)
    {
        GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
    }

}

u8 matrix_update_key(void)
{
    unsigned char i, j;
    OS_ERR      err;
    if (g_ucKeyValues != KEY_NUL)
    {
        return 0;
    }
    for(i = 0; i < 4; i++)             //i是输出口，依次置低电平
    {
        GPIO_ResetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
        //OSTimeDly ( 2, OS_OPT_TIME_DLY, & err );
        for(j = 0; j < 2; j++)            //j是输入口，当键按下时导通被置为低电平
        {
            //matrix_key[i][j] = 0;
            if(GPIO_ReadInputDataBit(matrix_key_input[j].GPIO_x, matrix_key_input[j].GPIO_pin) == 0)
            {
                while(!GPIO_ReadInputDataBit(matrix_key_input[j].GPIO_x, matrix_key_input[j].GPIO_pin))
                {
                    OSTimeDly ( 1, OS_OPT_TIME_DLY, & err );
                }
                //matrix_key[i][j] = 1;
                //macLED2_TOGGLE ();
                GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
                g_ucKeyValues = (i + 1) * 10 + (j + 1);
                return 0;
            }
        }
        GPIO_SetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
    }
    return 1;
}

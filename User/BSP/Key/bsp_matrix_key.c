#include "bsp_matrix_key.h"
#include "includes.h"

/**************�������.c�ļ�*****************************/
unsigned char matrix_key[4][4];
unsigned char g_ucKeyValues = KEY_NUL;      // ��ǰ����ֵ,ȫ��
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
    RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );    // ��ʱ��,���Ҵ򿪹ܽŸ���
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);           // �ر�JTAG��SWģʽ,�ܽŸ���

    /* ������ɨ������� ����ߵ�ƽ */
    /* PA0 PA1 PA2 PA3 ���*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* ������ɨ�������� ������ʱ����ߵ�ƽ �ſ�����͵�ƽ */
    /* PA4 PA5 PA6 PA7 ����*/
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
    for(i = 0; i < 4; i++)             //i������ڣ������õ͵�ƽ
    {
        GPIO_ResetBits(matrix_key_output[i].GPIO_x, matrix_key_output[i].GPIO_pin);
        //OSTimeDly ( 2, OS_OPT_TIME_DLY, & err );
        for(j = 0; j < 2; j++)            //j������ڣ���������ʱ��ͨ����Ϊ�͵�ƽ
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

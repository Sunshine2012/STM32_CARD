#include "includes.h"
#include "port.h"

u8 Task=0;				 //任务
u8 Order=0;				 //命令，0：自动运行，1：单步运行
u8 KaOn=1;				 //好坏卡，1：好卡，0：坏卡
u8 Ka_state=0;			 //卡机运行所处状态
u8 RunStep;				 //卡机运行步数

OS_ERR      err;

void NVIC0_IRQHandler(void)
{
    //确保是否产生了EXTI Line中断
    if (EXTI_GetITStatus(NVIC0_INT_EXTI_LINE) != RESET)
    {
        // LED1 取反
        // macLED1_TOGGLE();
        DK1=1;                              //到达位置1，顶卡电机刹车
        DK2=1;

        // 这里要延时

        DK1=0;                              //到达位置1，顶卡电机停
        DK2=0;
        Ka_state=1;
        //清除中断标志位
        EXTI_ClearITPendingBit(NVIC0_INT_EXTI_LINE);
    }
}

void NVIC1_IRQHandler(void)
{
    //确保是否产生了EXTI Line中断
    if (EXTI_GetITStatus(NVIC1_INT_EXTI_LINE) != RESET)
    {
        // LED2 取反
        // macLED2_TOGGLE();
        //清除中断标志位
        EXTI_ClearITPendingBit(NVIC1_INT_EXTI_LINE);
    }
}

void NVIC2_IRQHandler(void)
{
    //确保是否产生了EXTI Line中断
    if (EXTI_GetITStatus(NVIC2_INT_EXTI_LINE) != RESET)
    {
        // LED3 取反
        // macLED1_TOGGLE();
        //清除中断标志位
        EXTI_ClearITPendingBit(NVIC2_INT_EXTI_LINE);
    }
}

void NVIC3_IRQHandler(void)
{
    //确保是否产生了EXTI Line中断
    if (EXTI_GetITStatus(NVIC3_INT_EXTI_LINE) != RESET)
    {
        // LED4 取反
        // macLED2_TOGGLE();
        //清除中断标志位
        EXTI_ClearITPendingBit(NVIC3_INT_EXTI_LINE);
    }
}



void PORT_Init(void)
{
    RCC->APB2ENR|=1<<2;     //使能PORTA时钟
    RCC->APB2ENR|=1<<3;     //使能PORTB时钟
    RCC->APB2ENR|=1<<4;     //使能PORTC时钟

    GPIOA->CRL&=0X0000FFF0; //PA0,4,5,6,7设置成输入
    GPIOA->CRL|=0X88880008; //PA0,4,5,6,7设置成上下拉
    GPIOB->CRL&=0XFFFFFF00; //PB0,1设置成输入
    GPIOB->CRL|=0X00000088; //PB0,1设置成上下拉
    GPIOB->CRH&=0X00FF00FF; //PB10,11,14,15设置成输入
    GPIOB->CRH|=0X88008800; //PB10,11,14,15设置成上下拉
    GPIOC->CRH&=0X00000FFF; //PC11,12,13,14,15设置成输入
    GPIOC->CRH|=0X88888000; //PC11,12,13,14,15设置成上下拉
    GPIOC->CRL&=0XFFFF0000; //PC0,1,2,3设置成输入
    GPIOC->CRL|=0X00008888; //PC0,1,2,3设置成上下拉
    GPIOB->CRH&=0X0000FFFF; //PB12,PB13为输出, PB14,PB15设置成输入
    GPIOB->CRH|=0X88110000; //PB12,PB13为推挽输出, PB14,PB15设置成上下拉
    GPIOC->CRH&=0XFFFFFF00; //PC8,9设置成输出
    GPIOC->CRH|=0X00000011; //PC8,9为推挽输出

    GPIOA->ODR|=0X000000F1;	//PA0,4,5,6,7上拉
    GPIOB->ODR|=0X0000CC03;	//PB0,1,10,11,14,15上拉
    GPIOC->ODR|=0X0000F80F;	//PC0,1,2,3,11,12,13,14,15上拉

    DK1=0;
    DK2=0;
    FK1=0;
    FK2=0;

    // Ex_NVIC_Config(GPIO_A,5,FTIR); //下降沿触发
    // Ex_NVIC_Config(GPIO_A,6,FTIR); //下降沿触发
    // Ex_NVIC_Config(GPIO_A,7,FTIR); //下降沿触发
    // Ex_NVIC_Config(GPIO_B,0,FTIR); //下降沿触发
    // Ex_NVIC_Config(GPIO_B,1,FTIR); //下降沿触发
    // Ex_NVIC_Config(GPIO_B,10,FTIR); //下降沿触发
    // Ex_NVIC_Config(GPIO_B,11,FTIR); //下降沿触发

    // MY_NVIC_Init(0,1,EXTI0_IRQChannel,0);        //抢占0，子优先级1，组0
    // MY_NVIC_Init(0,2,EXTI1_IRQChannel,0);        //抢占0，子优先级2，组0
    // MY_NVIC_Init(0,3,EXTI9_5_IRQChannel,0);      //抢占0，子优先级3，组0
    // MY_NVIC_Init(0,4,EXTI15_10_IRQChannel,0);    //抢占0，子优先级4，组0
}

#if 0
//外部中断0服务程序
void EXTI0_IRQHandler(void)
{
    DK1=1;                              //到达位置1，顶卡电机刹车
    DK2=1;
    OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
    DK1=0;                              //到达位置1，顶卡电机停
    DK2=0;
    Ka_state=1;
    EXTI->PR=1<<0;                      //清除LINE0上的中断标志位
}
//
//外部中断1服务程序
void EXTI1_IRQHandler(void)
{
    DK1=1;                              //到达位置1，顶卡电机刹车
    DK2=1;
    OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
    DK1=0;                              //到达位置2，顶卡电机停
    DK2=0;
    Ka_state=2;
    EXTI->PR=1<<1;                      //清除LINE1上的中断标志位
}
//
//外部中断10,11服务程序
void EXTI15_10_IRQHandler(void)
{
    if (DI3==0)                         //到达位置3，顶卡电机停
    {
        DK1=1;                          //到达位置1，顶卡电机刹车
        DK2=1;
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
        DK1=0;
        DK2=0;
        Ka_state=3;
        EXTI->PR=1<<10;                 //清除LINE10上的中断标志位
    }
    else if (DI4==0)                    //到达位置4，顶卡电机停
    {
        DK1=1;                          //到达位置1，顶卡电机刹车
        DK2=1;
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
        DK1=0;
        DK2=0;
        Ka_state=4;
        EXTI->PR=1<<11;                 //清除LINE11上的中断标志位
    }
}
//
//外部中断5,6,7服务程序
void EXTI9_5_IRQHandler(void)
{
    if (FI1==0)                         //好卡翻转到位，翻卡电机反转回位
    {
        FK1=1;                          //到达位置1，翻卡电机刹车
        FK2=1;
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
        FK1=0;
        FK2=1;
        Ka_state=5;
        EXTI->PR=1<<5;                  //清除LINE10上的中断标志位
    }
    else if (FI2==0)                    //翻卡电机回0位
    {
        FK1=1;                          //到达位置1，翻卡电机刹车
        FK2=1;
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
        FK1=0;
        FK2=0;
        Ka_state=6;
        EXTI->PR=1<<6;                  //清除LINE11上的中断标志位
    }
    else if (FI3==0)                    //坏卡翻转到位，翻卡电机反转回位
    {
        FK1=1;                          //到达位置1，翻卡电机刹车
        FK2=1;
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
        FK1=1;
        FK2=0;
        Ka_state=7;
        EXTI->PR=1<<7;                  //清除LINE11上的中断标志位
    }
}
#endif

//下一步运行判断
void Next_Step1(void)                   //第一步翻卡结束后判断
{
    if (Order==0||Order==2)
    {
        RunStep=2;                      //进入第二步取卡
        Ka_state=0;
    }
}
//
void Next_Step2(void)                   //第二步取卡结束后判断
{
    if (Order==0&&QK==1)                //当在自动运行时，取完卡再进入第三步
    {
        RunStep=3;
        Ka_state=0;
    }
}
//
void Next_Step3(void)                   //第三步顶卡结束后判断
{
    if (Order==0)
    {
        RunStep=4;
        Ka_state=0;
    }
}
//
void Next_Step4(void)                   //第四步结束后判断
{
    if (Order==0&&YK==0)                //当在自动运行时，卡盒中有卡再返回第一步
    {
        RunStep=1;                      //返回第一步
        Ka_state=0;
    }
}
//
void K_Take(void)
{
    u8 k;
    for (k=0; k<=1; k++)                //取卡延时
    {
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
    }
}
//
u8 kc=0;
void K_Onoff(void)
{
    u8 k;
    for (k=0; k<=1; k++)                //判卡延时
        OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
    if (++kc>=10)                       //好坏卡交替
    {
        KaOn=0;
        kc=0;
    }
    else
        KaOn=1;
}
//
//送卡运行函数
void K_run(void)
{
    switch (RunStep)
    {
    case 1:
        if (KaOn&&!Ka_state)            //第一步
        {
            FK1=1;                      //正翻卡，有好卡
            FK2=0;
            Ka_state=8;                 //翻卡电机正转，为状态9
        }
        else if (!KaOn&&!Ka_state)      //反翻卡，有坏卡
        {
            FK1=0;
            FK2=1;
            Ka_state=9;                 //翻卡电机反转，为状态8
        }
        if (Ka_state==6)                //状态6，翻卡电机回0位
        {
            Next_Step1();
        }
        break;
    case 2:
        if (!Ka_state)                  //第二步
        {
            K_Take();                   //取卡判断
            Next_Step2();
        }
        break;
    case 3:
        if (!Ka_state)                  //第三步
        {
            DK1=0;
            DK2=1;
            Ka_state=10;                //顶卡电机运行
        }
        if (Ka_state==1||Ka_state==2||Ka_state==3||Ka_state==4)
        {
            OSTimeDly ( 200, OS_OPT_TIME_DLY, & err );
            Next_Step3();               //顶卡电机到位后，判下一步
        }
        break;
    case 4:
        if (!Ka_state)                  //第四步
        {
            K_Onoff();                  //好坏卡判断
            Next_Step4();
        }
        break;
    }
}
//
//按键扫描函数
//返回按键值
//0，没有任何按键按下
//1，2,3,4,5，有KEY按下
u8 KEY_Scan(void)
{
    static u8 key_up=1;                       //按键按松开标志
    if (key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0))
    {
        OSTimeDly ( 10, OS_OPT_TIME_DLY, & err );                         //去抖动
        key_up=0;
        if (KEY0==0)
        {
            return 1;
        }
        else if (KEY1==0)
        {
            return 2;
        }
        else if (KEY2==0)
        {
            return 3;
        }
        else if (KEY3==0)
        {
            return 4;
        }
    }
    else if (KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1)
    {
        key_up=1;
    }
    return 0;                                    // 无按键按下
}
//
//按键处理函数
void KEY_deal(u8 step)
{
    switch (step)
    {
    case 1:
        if (LED1==0||LED0==0)           //主按键，运行切换
        {
            RunStep=0;
            LED1=1;         //关LED
            LED0=1;
            DK1=0;
            DK2=0;
            FK1=0;
            FK2=0;
        }
        else
        {
            RunStep=1;      //从第一步开始不断循环运行
            Ka_state=0;
            LED1=0;         //亮LED1
            LED0=1;
            Order=0;
        }
        break;
    case 2:
        RunStep=1;          //单独翻好卡
        LED1=1;
        LED0=0;         //亮LED0
        Ka_state=0;
        KaOn=1;
        Order=1;
        break;
    case 4:
        RunStep=3;          //单独顶一张卡
        LED1=1;
        LED0=0;         //亮LED0
        Ka_state=0;
        Order=1;
        break;
    case 3:
        RunStep=1;          //单独翻坏卡
        LED1=1;
        LED0=0;         //亮LED0
        Ka_state=0;
        KaOn=0;
        Order=1;
        break;
    }
}

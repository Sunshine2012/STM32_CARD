/**********************************************************
* @ File name -> lcd12864.h
* @ Version   -> V1.1.2
* @ Date      -> 11-30-2013
* @ Brief     -> LCD12864驱动函数头文件
*                适用于ST7920驱动芯片

 V1.1
* @ Revise    -> A、修正操作命令宏定义。
* @           -> B、增加串口、并口转换宏定义，增加对高速度MCU控制通讯频率延时函数
* @           -> C、显示字符串函数增加显示长度选择，即显示多少个ASCII可显示字符，汉字x2即可
* @           -> D、去掉显示数组函数，合并在字符串显示里

 V1.1.1
* @ Revise    -> 增加字符串显示函数，可以从要显示的数组中任意位置显示一定长度字符串

 V1.1.2
* @ Revise    -> A、增加对低速晶振系统的MCU通讯频率是否使用的宏定义
* @           -> B、增加在检测液晶忙超时退出，预防死循环
**********************************************************/

#ifndef bsp_lcd12864_h_
#define bsp_lcd12864_h_

/**********************************************************
                     外部函数头文件
         应用到不同的外设头文件请在这里修改即可
**********************************************************/

#include "includes.h"

extern unsigned char CGROM_BAIHEJIU[];
extern unsigned char CGROM_YIFUYUAN[];

/**********************************************************
                    LCD工作模式选择
       LCD并口和串口选择请修改此参数和硬件电路即可
**********************************************************/
//#define LCD_Work_Mode           0       //0：为串口工作模式
                                        //1：为并口工作模式

#define LCD_USE_DELAY           0       //0: 为不使用延时调整通讯频率，对于12MHz以内的晶振
                                        //1: 为使用延时调整通讯频率，对于12MHz以上的晶振

#define Soft_PSB                0       //0：使用硬件控制串口
                                        //1：使用软件控制串口

/**********************************************************
                  延时调整通讯频率参数
          需要使用不同频率的请修改此延时函数值
**********************************************************/

#define LCD_Delay()             delay_us(100)

/**********************************************************
                      定义接口信息
**********************************************************/
#ifdef Soft_PSB //定义了则使用软件控制

#define LCD_PSB                 GPIO_Pin_3    //LCD并口和串口通讯选择口

#endif

//#define LCD_DATA_PORT   GPIOE
#define LCD_DATA_PORT           GPIOB
//#define LCD_RST                 GPIO_Pin_8    //LCD复位脚

//=========================================================

#ifdef  LCD_Work_Mode                /*    并口工作模式    */

//=========================================================

#define ktdata                  GPIOE    //8位数据接口

#define LCD_EN                  GPIO_Pin_2
#define LCD_RW                  GPIO_Pin_1
#define LCD_RS                  GPIO_Pin_0

#define LCD_EN_SET      GPIO_SetBits(LCD_DATA_PORT, LCD_EN)
#define LCD_RW_SET      GPIO_SetBits(LCD_DATA_PORT, LCD_RW)
#define LCD_RS_SET      GPIO_SetBits(LCD_DATA_PORT, LCD_RS)
#define LCD_PSB_SET     GPIO_SetBits(LCD_DATA_PORT, LCD_PSB)
#define LCD_RST_SET     GPIO_SetBits(LCD_DATA_PORT, LCD_RST)

#define LCD_EN_RESET    GPIO_ResetBits(LCD_DATA_PORT, LCD_EN)
#define LCD_RW_RESET    GPIO_ResetBits(LCD_DATA_PORT, LCD_RW)
#define LCD_RS_RESET    GPIO_ResetBits(LCD_DATA_PORT, LCD_RS)
#define LCD_PSB_RESET   GPIO_ResetBits(LCD_DATA_PORT, LCD_PSB)
#define LCD_RST_RESET   GPIO_ResetBits(LCD_DATA_PORT, LCD_RST)
//=========================================================

#else    /*    串口工作模式    */

//=========================================================
/***********串口模式时接口定义***********/
#define LCD_SCL			GPIO_Pin_13     //对应管脚为EN
#define LCD_SDA			GPIO_Pin_15     //对应管脚为RW
#define LCD_CS          GPIO_Pin_12     //对应管脚为RS
#define LCD_RST			GPIO_Pin_5      //对应管脚为RST
//#define LCD_BAK			GPIO_Pin_8      //对应管脚为背光

//=========================================================
#define LCD_SCL_SET     GPIO_SetBits(LCD_DATA_PORT, LCD_SCL)
#define LCD_SDA_SET     GPIO_SetBits(LCD_DATA_PORT, LCD_SDA)
#define LCD_CS_SET      GPIO_SetBits(LCD_DATA_PORT, LCD_CS)
#define LCD_RST_SET     GPIO_SetBits(LCD_DATA_PORT, LCD_RST)
#define LCD_PSB_SET     GPIO_SetBits(LCD_DATA_PORT, LCD_PSB)
//#define LCD_BAK_SET     GPIO_SetBits(LCD_DATA_PORT, LCD_BAK)


#define LCD_SCL_RESET   GPIO_ResetBits(LCD_DATA_PORT, LCD_SCL)
#define LCD_SDA_RESET   GPIO_ResetBits(LCD_DATA_PORT, LCD_SDA)
#define LCD_CS_RESET    GPIO_ResetBits(LCD_DATA_PORT, LCD_CS)
#define LCD_RST_RESET   GPIO_ResetBits(LCD_DATA_PORT, LCD_RST)
#define LCD_PSB_RESET   GPIO_ResetBits(LCD_DATA_PORT, LCD_PSB)
//#define LCD_BAK_RESET   GPIO_ResetBits(LCD_DATA_PORT, LCD_BAK)  // 背光引脚

#endif    //结束串口、并口模式选择


//=========================================================

/**********************************************************
                        参数宏定义
**********************************************************/
#define LCD_Busy                0x80    //LCD忙标志

#define Graph_BassADD           0x00    //显示GRAM基地址

//CGRAM显示地址单元
#define ANT_Graph               0x00

//#define Temperature_Graph     0x02
#define Bluetooth_Graph         0x02
#define CD_Graph                0x04
#define Hand_Graph              0x06

//////////////////////////////////////////////////////
//LCD12864指令集
//////////////////////////////////////////////////////
#define One_LineAddress         0x80    //第一行基地址
#define Two_LineAddress         0x90    //第二行基地址
#define Three_LineAddress       0x88    //第三行基地址
#define Four_LineAddress        0x98    //第四行基地址

//基本指令宏定义
#define MPU_Busy                0x80    //LCD忙，同时也读出了地址计数器（AC）数值
#define Clear_Display           0x01    //清屏指令
#define Zero_Address            0x02    //显示地址归零，AC = 0

//扩充指令宏定义
#define LCD_Standby             0x01    //待机指令，先设置RE = 1（功能设定寄存器），在送此命令即进入待机模式

//退出待机，写入任意指令可以退出，再将RE = 0即可

/***********基本指令***********/

//进入设定点（entry mode set）
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   0   1   I/D  S

#define Entry_ModeSet           0x04    //基数

#define Vernier_ShiftRight      (Entry_ModeSet | (1<<1))        //游标右移，AC+1
#define Vernier_ShiftLeft       (Entry_ModeSet | (0<<1))        //游标右移，AC-1

#define Frames_ShiftRight       (Vernier_ShiftLeft | 0x01)      //画面整体右移
#define Frames_ShiftLeft        (Vernier_ShiftRight | 0x01)     //画面整体左移

//显示开关设置（display status）
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   1   D   C   B
#define Display_Status          0x08    //基数

#define Open_Display            (Display_Status | (1<<2))       //整体显示开
#define Close_Display           (Display_Status | (0<<2))       //整体显示关

#define Open_Vernier            (Display_Status | (1<<1))       //游标显示开
#define Close_Vernier           (Display_Status | (0<<1))       //游标显示关

#define Vernier_White_Open      (Display_Status | 0x01)         //游标位置显示反白开
#define Vernier_White_Close     (Display_Status | 0x00)         //游标位置显示反白关，正常显示

//游标或显示移位控制（cursor and display shift control）
// b7  b6  b5  b4  b3    b2   b1  b0
// 0   0   0   1   S/C  R/L1  X   X
#define Cursor_Display_Control  0x10    //基数

#define Cursor_ShiftLeft        (Cursor_Display_Control | (0<<2))    //游标向左移动，AC = AC - 1
#define Cursor_ShiftRight       (Cursor_Display_Control | (1<<2))    //游标向右移动，AC = AC + 1
#define Display_ShiftLeft       (Cursor_Display_Control | (2<<2))    //显示向左移动，游标跟着移动，AC = AC
#define Display_ShiftRight      (Cursor_Display_Control | (3<<2))    //显示向右移动，游标跟着移动，AC = AC

//功能设定（function set）
// b7  b6  b5  b4  b3  b2    b1  b0
// 0   0   1   DL  X   0/RE  X      X
#define Function_SetBasic       0x20    //基数

#define MPU_8bit                (Function_SetBasic | (1<<4))        //8bit MPU接口
#define MPU_4bit                (Function_SetBasic | (0<<4))        //4bit MPU接口

#define Instruct_Basic4         (MPU_4bit | (0<<2))                 //4bit MPU接口，基本指令集
#define Instruct_Basic8         (MPU_8bit | (0<<2))                 //8bit MPU接口，基本指令集

#define Instruct_Expansion4     (MPU_4bit | (1<<2))                 //4bit MPU接口，扩充指令集
#define Instruct_Expansion8     (MPU_8bit | (1<<2))                 //8bit MPU接口，扩充指令集


//设定CGRAM地址
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   1   A5  A4  A3  A2  A1  A0
#define CGRAM_Set               0x40    //基数

//设定CGRAM地址到地址计数器（AC），AC范围为0x00 ~ 0x3f，需确认扩充指令中SR = 0（卷动位置或RAM地址选择）
//设定DDRAM地址
// b7  b6  b5  b4  b3  b2  b1  b0
// 1   0   A5  A4  A3  A2  A1  A0
#define DDRAM_Set               0x80    //基数

//设定CGRAM地址到地址计数器（AC）
//写显示数据到RAM（RS = 1 RW = 0）
// b7  b6  b5  b4  b3  b2  b1  b0
// D7  D6  D5  D4  D3  D2  D1  D0
//写入数据后会使AC改变，每个RAM（CGRAM、DDRAM、IRAM）地址都可以连续写入2字节的显示数据，
//当写入第二个字节时，地址计数器（AC）的值自动加1
//读取显示数据到RAM（RS = 1 RW = 1）
// b7  b6  b5  b4  b3  b2  b1  b0
// D7  D6  D5  D4  D3  D2  D1  D0
//读取数据后会使AC改变，设定RAM（CGRAM、DDRAM、IRAM）地址后，先要Dummy read一次后才能读取到正确的显示数据，
//第二次读取不需要Dummy read，除非重新设定了RAM地址

/***********扩充指令***********/
//卷动位置或RAM地址选择
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   0   0   1   SR
#define Write_Roll_Address_Allow 0x03    //允许输入垂直卷动地址
#define Write_IRAM_Address_Allow 0x02    //允许输入IRAM地址（扩充指令），及允许设定CGRAM地址（基本指令）

//反白显示
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   0   1   0   R0
#define Display_White_One       0x04    //第一行反白显示
#define Display_White_Two       0x05    //第二行反白显示

//睡眠模式
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   0   0   1   SL  0   0
#define LCD_Sleep_Open          (0x08 | (1<<2))    //进入睡眠模式
#define LCD_Sleep_Close         (0x08 | (0<<2))    //退出睡眠模式

//扩充功能设定
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   0   1   DL  X   RE  G   X
#define MPU_4bit_Basic_Draw_Open    (Instruct_Basic4 | (1<<1))    //4bit MPU接口，基本指令，绘图开
#define MPU_4bit_Basic_Draw_Close   (Instruct_Basic4 | (0<<1))    //4bit MPU接口，基本指令，绘图关

#define MPU_8bit_Basic_Draw_Open    (Instruct_Basic8 | (1<<1))    //8bit MPU接口，基本指令，绘图开
#define MPU_8bit_Basic_Draw_Close   (Instruct_Basic8 | (0<<1))    //8bit MPU接口，基本指令，绘图关

#define MPU_8bit_Expansion_Draw_Open    (Instruct_Expansion8 | (1<<1))    //8bit MPU接口，扩充指令，绘图开
#define MPU_8bit_Expansion_Draw_Close   (Instruct_Expansion8 | (0<<1))    //8bit MPU接口，扩充指令，绘图关

#define MPU_4bit_Expansion_Draw_Open    (Instruct_Expansion4 | (1<<1))    //4bit MPU接口，扩充指令，绘图开
#define MPU_4bit_Expansion_Draw_Close   (Instruct_Expansion4 | (0<<1))    //4bit MPU接口，扩充指令，绘图关

//设定IRAM地址或卷动地址
// b7  b6  b5  b4  b3  b2  b1  b0
// 0   1   A5  A4  A3  A2  A1  A0
#define IRAM_Set                0x40    //基数

//SR = 1，A5 ~ A0为垂直卷动地址，SR = 0，A3 ~ A0为IRAM地址
//设定绘图RAM地址
// b7  b6  b5  b4  b3  b2  b1  b0
// 1   0   0   0   A3  A2  A1  A0 --->水平地址范围：A3 ~ A0
// 1   A6  A5  A4  A3  A2  A1  A0 --->垂直地址范围：A6 ~ A0
#define Draw_RAMSet             0x80    //基数

//设定GDRAM地址到地址计数器（AC），先设置垂直位置再设置水平位置（连续写入2字节数据来完成垂直与水平坐标的设置）。

/**********************************************************
                      外部功能函数
**********************************************************/

void LCD12864_Init(void);    //LCD YB12864ZB初始化

void LCD12864_Clear(void);    //LCD YB12864ZB清屏

void LCD_Write_Byte(u8 dat,u8 ord);    //LCD写入一个字节命令或者数据，判断忙标志

void displayGB2312tring(u8 x, u8 y, u8 * text, u8 isTurn);    //LCD显示字符串

void displayGB2312StingLen(u8 x, u8 y, u8 * text, u8 isTurn, u8 len);

void Display_String(u8 x,u8 y,u8 *str,u8 len);

void Display_StringFromIndex(u8 x,u8 y,u8 *str,u8 starsum,u8 len);

void Display_CGRAM(u8 x,u8 y,u8 add_h,u8 add_l);    //显示一个CGRAM内容

void Display_GDRAM(u8 x,u8 y,u8 width,u8 height,u8 *bmp,u8 dis_mode);    //LCD写入GDRAM绘图

void delay_us(int delay);

void isTurnShow(u8 x, u8 y);



/**********************************************************
* 函数功能 ---> 写入自定义字符到LCD的CGRAM
* 入口参数 ---> *data_code：写入的数组
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void LCD_Write_CGRAM(u8 *data_code);

#endif


/*********************************************************/

/*

 1、指令表1（RE = 0，基本指令集）
  ___________________________________________________________________________
 |               |  控制信号      |              控 制 代 码                  | 执行 |
 |               |____________|_______________________________________|      |
 |   指令名称        | RS | R/W   | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 | 时间 |     HEX
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   清除显示        | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 1.6ms|     0x01
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   地址归0        | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 1  | X  | 72us |     0x02 //不改变DDRAM内容
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |  进入设定点        | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 1  | I/D| S  | 72us |     0x06 游标右移    0x07 画面整体左移
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x04 游标左移    0x05 画面整体右移
 | 显示开关设置  | 0  |         0   | 0  | 0  | 0  | 0  | 1  | D  | C  | S  | 72us |     D = 1; 整体显示开  D = 0; 整体显示关 // 不改变DDRAM内容
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     C = 1; 游标显示开  C = 0; 游标显示关  S = 1; 游标位置显示反白 S = 0; 正常显示
 |   移位控制        | 0  |   0   | 0  | 0  | 0  | 1  | S/C| R/L| X  | X  | 72us |     0x10 游标向左移动 AC = AC - 1   0x14 游标向右移动 AC = AC + 1
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x18 显示向左移动 AC = AC       0x1c 显示向右移动 AC = AC
 |   功能设定        | 0  |   0   | 0  | 0  | 1  | DL | X  |0/RE| X  | X  | 72us |     0x30 8bit接口，基本指令   0x34 8bit接口，扩充指令
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x20 4bit接口，基本指令   0x24 4bit接口，扩充指令
 | 设定CGRAM地址 | 0  |   0   | 0  | 1  | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______| 设定CGRAM地址到地址计数器（AC），AC范围0x00 ~ 0x3f，需确认扩充指令中SR = 0
 | 设定DDRAM地址 | 0  |   0   | 1  | 0  | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______| 设定DDRAM地址到地址计数器（AC），第一行AC范围 0x80 ~ 0x8f    第二行AC范围 0x90 ~ 0x9f
 | 读忙标志和地址| 0  |   1          | BF | A6 | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     BF = 0; //可以接受新数据
 |  写显示数据        | 1  |   0   |                显示数据                   | 72us |
 |_______________|____|_______|_______________________________________|______| 每个RAM（CGRAM、DDRAM、IRAM）地址都可以连续写入2个字节，第二个字节时AC自动增加1
 |  读显示数据        | 1  |   1   |                显示数据                   | 72us |
 |_______________|____|_______|_______________________________________|______| 设定RAM（CGRAM、DDRAM、IRAM）地址后，先要Dummy read（假读）一次后才能读到正确的显示
                                                                               数据，第二次读取不需要再Dummy read（假读）一次，除非重新设置了RAM地址



 2、指令表2（RE = 1，扩充指令集）
  ___________________________________________________________________________
 |               |  控制信号      |              控 制 代 码                  | 执行 |
 |               |____________|_______________________________________|      |
 |   指令名称        | RS | R/W   | D7 | D6 | D5 | D4 | D3 | D2 | D1 | D0 | 时间 |     HEX
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   待命模式        | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 0  | 1  | 72us |     0x01   该指令不能改变RAM内容
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |   卷动地址或       | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 0  | 1  | SR | 72us |
 |  RAM地址选择      |    |       |    |    |    |    |    |    |    |    |      |     初始值0x02， 0x03 允许输入垂直卷动地址
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|              0x02 允许输入IRAM地址（扩充指令），及允许设定CGRAM地址（基本指令）
 |   反白显示        | 0  |   0   | 0  | 0  | 0  | 0  | 0  | 1  | R1 | R0 | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     初始值0x04   0x05反白显示
 |   睡眠模式        | 0  |   0   | 0  | 0  | 0  | 0  | 1  | SL | X  | X  | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     0x0c 脱离睡眠模式    0x08 进入睡眠模式
 |  扩充功能设定 | 0  |   0         | 0  | 0  | 1  | DL | X  |1/RE| G  | 0  | 72us |
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|
 |设定IRAM地址或 | 0  |   0        | 0  | 1  | A5 | A4 | A3 | A2 | A1 | A0 | 72us |
 |   卷动地址        |    |       |    |    |    |    |    |    |    |    |      |     SR = 1;    //A5 ~ A0为垂直卷动地址
 |_______________|____|_______|____|____|____|____|____|____|____|____|______|     SR = 0;    //A3 ~ A0为IRAM地址
 |               |    |       |    | 0  | 0  | 0  | A3 | A2 | A1 | A0 |      |
 |               |    |       |    |____|____|____|____|____|____|____|      | 设定GDRAM地址到计数器地址（AC），先设置垂直地址再设置水平地址，
 |设定绘图RAM地址| 0  |   0         | 1  | A6 | A5 | A4 | A3 | A2 | A1 | A0 | 72us | （连续写入2个字节数据来完成垂直和水平地址设置）
 |_______________|____|_______|____|____|____|____|____|____|____|____|______| 垂直地址范围：AC6 ~ AC0
                                                                               水平地址范围：AC3 ~ AC0

    扩充功能设定：

    DL = 1;    //8bit MPU接口         DL = 0;    //4bit MPU接口

    RE = 1;    //扩充指令               RE = 0;    //基本指令

    G  = 1;    //绘图显示开              G = 0;    // 绘图显示关

*/

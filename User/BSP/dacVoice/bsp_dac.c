#include "includes.h"
#include "stm32f10x.h"


#define DMA_IT_STATU(dma, chn, it)         ( ( (uint32_t)(dma) << 28) | ( (uint32_t)(it) << ( (uint32_t)(chn - 1) * 4) ) )
#define NVIC_DAC_PRIO				1
#define NVIC_DAC_SUB_PRIO		1

void DMA2_Channel3_IRQHandler(void)
{
    ITStatus    sts;

    sts = DMA_GetITStatus(DMA_IT_STATU(1, 3, DMA_IT_TC));
    if (SET == sts)
    {
        DMA_ClearITPendingBit(DMA_IT_STATU(1,3, DMA_IT_TC));
        DMA_Cmd(DMA2_Channel3, DISABLE);
        TIM_Cmd(TIM6, DISABLE);
        DAC_Cmd(DAC_Channel_1, DISABLE);
        //OSSemPost(VoicePlayedSem);
    }
}
void dacClose(void)
{
    DMA_Cmd(DMA2_Channel3, DISABLE);

    DAC_Cmd(DAC_Channel_1, DISABLE);
    /* Enable DMA for DAC Channel1 */
    DAC_DMACmd(DAC_Channel_1, DISABLE);
    TIM_Cmd(TIM6, DISABLE);
    DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, DISABLE);                // 使能DMA2通道3发送完中断
}
/*********************************************************************************************************
** Function name:            dacSet(uint8_t *pbuf,uint32_t len)
**
** Descriptions:            set the dac output by dma
**
** input parameters:
** output parameters:

** Returned value:
**
**
** Created by:              Yanghaiyang
** Created Date:             2011-10-05
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#define DAC_DHR8R1_Address      0x40007410
// 开启DAC转换
void dacSet(const unsigned char *pbuf,uint32_t len)
{

    DMA_InitTypeDef  DMA_InitStructure;

    /* DMA2 channel3 configuration */
    DMA_DeInit(DMA2_Channel3);

    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR8R1_Address;// DAC输出缓冲区
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pbuf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = len;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA2_Channel3, &DMA_InitStructure);
    /* Enable DMA2 Channel3 */
    DMA_Cmd(DMA2_Channel3, ENABLE);
    /* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
     automatically connected to the DAC converter. */
    DAC_Cmd(DAC_Channel_1, ENABLE);
    /* Enable DMA for DAC Channel1 */
    DAC_DMACmd(DAC_Channel_1, ENABLE);

    //TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE);
    /* TIM6 enable counter */
    TIM_Cmd(TIM6, ENABLE);
    DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);                // 使能DMA2通道3发送完中断

}

// DAC初始化
static void DAC_Config(void)
{
	DAC_InitTypeDef  DAC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	//TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

	/* Once the DAC channel is enabled, the corresponding GPIO pin is automatically
	 connected to the DAC converter. In order to avoid parasitic consumption,
	 the GPIO pin should be configured in analog */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;//DAC_Trigger_Software;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits8_0;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	/* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is
	 automatically connected to the DAC converter. */
	DAC_Cmd(DAC_Channel_1, ENABLE);

	/* TIM6 Configuration */
	//PrescalerValue = (uint16_t) (SystemCoreClock / 8000000) - 1;
	TIM_PrescalerConfig(TIM6, 8, TIM_PSCReloadMode_Update);				// 基准时钟8M
	TIM_SetAutoreload(TIM6, 1000);										// 再分频1000
	/* TIM6 TRGO selection */
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_DAC_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_DAC_SUB_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel = 58;							/* 使能DMA2_3中断通道号       */
    NVIC_Init(&NVIC_InitStructure);
}


void DAC_init( void )
{
    DAC_Config ();      // 初始化DAC
}

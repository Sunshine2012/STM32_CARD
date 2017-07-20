#include "includes.h"
//#include "WavArray.h"
#include "TOTAL_wav.h"

#include "WAV_C_anjian.h"
#include "WAV_C_xiexie.h"
#include "WAV_C_anjianquka.h"






#define DMA_IT_STATU(dma, chn, it)	     ( ( (uint32_t)(dma) << 28) | ( (uint32_t)(it) << ( (uint32_t)(chn - 1) * 4) ) )

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
				OSSemPost(VoicePlayedSem);
    }
}
static void closeDac(void)
{
	DMA_Cmd(DMA2_Channel3, DISABLE);

	DAC_Cmd(DAC_Channel_1, DISABLE);
	/* Enable DMA for DAC Channel1 */
	DAC_DMACmd(DAC_Channel_1, DISABLE);
	TIM_Cmd(TIM6, DISABLE);
	DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, DISABLE);				// 使能DMA2通道3发送完中断
}
/*********************************************************************************************************
** Function name:			dacSet(uint8_t *pbuf,uint32_t len)
**
** Descriptions:			set the dac output by dma
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

static void dacSet(const unsigned char *pbuf,uint32_t len)
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
	DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);				// 使能DMA2通道3发送完中断

}

 
/*
const AUDIO_TYPE audioData = {
{
	
wav0,wav1,wav2,wav3,wav4,
wav5,wav6,wav7,wav8,wav9,wav10,
	
},
		
{sizeof(wav0),sizeof(wav1),sizeof(wav2),sizeof(wav3),sizeof(wav4),sizeof(wav5),
 sizeof(wav6),sizeof(wav7),sizeof(wav8),sizeof(wav9),sizeof(wav10),

}
};
*/
const unsigned long Voice_Addr[]=
{
	0X0050,0X0FA0,0X1F41,0X2EE1,0X3E82,0X4E22,0X5DC3,0X6D63,0X7D04,0X8CA5,0X9C45,0XABE6,0XBB86,0XCB27,
	0XDAC7,0XEA68,0XFA08,0X11309,0X12C0A,0X1482B,0X1644C,0X17D4D,0X1964E,0X1AF4F,0X1C850,0X1E151,0X1F732,
	0X20D12,0X21CB3,0X22C53,0X23BF4,0X254F5,0X26DF6,0X286F7,0X29507,0X2A7C8,0X2CA29,0X2E00A,0X2FF4B,0X3139C,
	0X32FBD,0X34EFE,0X3666F,0X3792F,0X390A0,0X3A681,0X3BAD2,0X3CF22,0X3E373,0X3F7C4,0X40A85,0X41ED5,0X43E16,
	0X453F7,0X466B8,0X47C99,0X490E9,0X4A53A,0X4B98B,0X4DA5C,0X4E9FD,0X4F99D,0X53B30,
}; 



extern u8 read_array[25];
u8 testFlashWriteAndRead(void);
void VoicePlayDeal(void)
{
		
		unsigned int err,i,err0;
		INT16U AddrIndex=0;
		SYS_DATA_STR *pSyDataStr=gSysDataRx;
		//VoiceCmdSem = OSSemCreate(0);//语命令
		//VoicePlayedSem=OSSemCreate(0);//语音播放完毕

	err0=0;
	CommLED(ON); 
	
	//SST39V040_BYTES_SIZE
	//if(Sst39vf040ReadId()!=SST39VF040_MF_ID)									err0 |= 0x01u;
	//if(Sst39vf040ChipCheck(0,SST39V040_BYTES_SIZE)!=0u)				err0 |= 0X02u;
	//---SRAM --test
	//if(MEM_addr(Bank1_SRAM3_ADDR,Bank1_SRAM3_SIZE)!=0x0000u)  err0 |= 0X04u;
	//---at24c02n  
	//if(TestAT24C02N(0,256)!=0u)																err0 |= 0X08u;
	//--rx8025sa
	//if(TestRX8025sa()!=0x00u)																	err0 |= 0X10u;
	CommLED(OFF); 
        
		while(1)
		{
				//OSSemPend(VoiceCmdSem,100,&err);	
			
				//测试指示灯
				//if( err0 !=0)
				//{
				//	CommLED(OFF); 
				//	OSTimeDly(200);
				//	CommLED(ON); 	
				//	OSTimeDly(100);
				
				//}
#if 0
				//if(err==OS_ERR_NONE)
				{
					for(i=0;i<400;i++)
					{
						//if(pSyDataStr->SysVoiceBuff[i]!=0xfe)
						{
								AddrIndex=(pSyDataStr->SysVoiceBuff[i])%(sizeof(Voice_Addr)/sizeof(Voice_Addr[0]));
								//AddrIndex%=(sizeof(Voice_Addr)/sizeof(Voice_Addr[0]));
//#if VOICE_EN==1							
								dacSet(&TOTAL_wav[Voice_Addr[AddrIndex]],Voice_Addr[AddrIndex+1]-Voice_Addr[AddrIndex]);								
								//OSSemPend(VoicePlayedSem,0,&err);//等待单段播放完成 
//#endif							
								AddrIndex++;	
						}
						//else 
						//{
						//	closeDac();
						//	break;
						//}
							
					}
#endif
                    //for(i=0;i<400;i++)
                    //{
                        
                        //dacSet(DATA_xiexie,SOUND_LENGTH_xiexie);								
                        //OSTimeDly(2500);
                        dacSet(DATA_anjianquka,SOUND_LENGTH_anjianquka);
                        OSTimeDly(2000);
                        dacSet(DATA_anjian,SOUND_LENGTH_anjian);
                        OSTimeDly(1000);
				//}
				
		}	
	
	
}

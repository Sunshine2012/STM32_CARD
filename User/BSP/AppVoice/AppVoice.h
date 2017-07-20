#ifndef APP_VOICE_H
#define APP_VOICE_H

#include "stm32f10x.h"

#define VOICE_EN						  0					//
#define DEVICE_USE_UART2_EN		0					// 如果称重采用USART2

#define AUDIO_TYPE_MAX_NUM		0x40//20					// 语音段种类总数
#define AUDIO_PLAY_MAX_LEN		10					// 一段语音最大长度，以单位语音种类为单位

#define WAV_TYPE_DATA_SHIFT		0x2b				// WAV格式文件，数据偏移量
#define AUDIO_PLAY_SIGNLE		1					// 单次播放
#define AUDIO_PLAY_CIRCURT		2					// 循环播放
#define AUDIO_PLAY_PARA_ERR		0xff				// 语音播放参数输入错误
#define AUDIO_PLAY_SET_OK		1					// 语音播放设置成功
#define AUDIO_CALL_MAX_NUMS		100					// 同时记录呼叫信息条数




typedef struct {
	const unsigned char *Audio[AUDIO_TYPE_MAX_NUM];	// 语音段首地址
	const unsigned int AudioLen[AUDIO_TYPE_MAX_NUM];// 语音段长度
}AUDIO_TYPE;


typedef struct {
	uint8_t		playType;							// 播放类型
	uint8_t 	audioLen;							// 播放语音种类长度
	uint8_t  	audioIndex[AUDIO_PLAY_MAX_LEN];		// 语音播放索引号
}AUDIO_PLAY;

















void VoicePlayDeal(void);

#endif

#ifndef __BSP_DAC_H__
#define __BSP_DAC_H__


#include <includes.h>

void DAC_init( void );
void dacClose(void);
void dacSet(const unsigned char *pbuf,uint32_t len);

#endif

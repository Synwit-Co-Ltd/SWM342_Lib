#ifndef __SWM342_CACHE_H__
#define __SWM342_CACHE_H__


void CACHE_Open(void);
void CACHE_Close(void);
void CACHE_Reset(void);

uint32_t CACHE_Prefet(uint32_t addr);
uint32_t CACHE_Unlock(uint32_t addr);


#define CACHE_RES_OK	0
#define CACHE_RES_FAIL	1


#endif //__SWM342_CACHE_H__

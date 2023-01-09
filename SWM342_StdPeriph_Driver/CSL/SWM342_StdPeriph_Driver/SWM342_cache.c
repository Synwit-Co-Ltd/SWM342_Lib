/****************************************************************************************************************************************** 
* 文件名称: SWM342_cache.c
* 功能说明:	SWM342单片机的CACHE管理驱动函数
* 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项:
* 版本日期:	V1.0.0		2016年1月30日
* 升级记录:  
*
*
*******************************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology
*******************************************************************************************************************************************/
#include "SWM342.h"
#include "SWM342_cache.h"


/****************************************************************************************************************************************** 
* 函数名称:	CACHE_Open()
* 功能说明:	CHCHE开启
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CACHE_Open(void)
{
	SYS->CLKEN1 |= (1 << SYS_CLKEN1_SFC_Pos);	// 使用 SFC 的时钟使能
	
	for(int i = 0; i < CyclesPerUs; i++) __NOP();
	
	CACHE_Reset();
	
	for(int i = 0; i < CyclesPerUs; i++) __NOP();
	
	SYS->CACHECR = 0;
}


/****************************************************************************************************************************************** 
* 函数名称:	CACHE_Close()
* 功能说明:	CHCHE关闭
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CACHE_Close(void)
{
	SYS->CACHECR = 1;
}


/****************************************************************************************************************************************** 
* 函数名称:	CACHE_Reset()
* 功能说明:	CHCHE复位，清除CHCHE所有标志和内容
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CACHE_Reset(void)
{
	while((CACHEC->CR & CACHEC_CR_IDLE_Msk) == 0) __NOP();
	
	CACHEC->CR |= (1 << CACHEC_CR_RST_Pos);
}


/****************************************************************************************************************************************** 
* 函数名称:	CACHE_Prefet()
* 功能说明:	CHCHE预取锁存
* 输    入: uint32_t addr		将Flash中addr开始的128字节代码预取到Cache中并锁定
* 输    出: uint32_t			CACHE_RES_OK、CACHE_RES_FAIL
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t CACHE_Prefet(uint32_t addr)
{
	uint32_t res;
	
	CACHEC->IE |= (1 << CACHEC_IE_PREFET_Pos);	// 只有中断使能时相应中断状态才能置起
	
	while((CACHEC->PREFCR & CACHEC_PREFCR_RDY_Msk) == 0) __NOP();
	
	CACHEC->PREFCR = (1 << CACHEC_PREFCR_PREFET_Pos) |
					(addr << CACHEC_PREFCR_ADDR_Pos);
	
	while((CACHEC->IF & (CACHEC_IF_PREOK_Msk | CACHEC_IF_PREFAIL_Msk)) == 0) __NOP();
	
	if(CACHEC->IF & CACHEC_IF_PREOK_Msk) res = CACHE_RES_OK;
	else							     res = CACHE_RES_FAIL;
	
	CACHEC->IF = CACHEC_IF_PREOK_Msk | CACHEC_IF_PREFAIL_Msk;	//清除标志
		
	return res;
}

/****************************************************************************************************************************************** 
* 函数名称:	CACHE_Unlock()
* 功能说明:	释放锁存在Cache中的代码
* 输    入: uint32_t addr		要解锁释放的代码的地址
* 输    出: uint32_t			CACHE_RES_OK、CACHE_RES_FAIL
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t CACHE_Unlock(uint32_t addr)
{
	uint32_t res;
	
	CACHEC->IE |= (1 << CACHEC_IE_UNLOCK_Pos);	// 只有中断使能时相应中断状态才能置起
	
	while((CACHEC->PREFCR & CACHEC_PREFCR_RDY_Msk) == 0) __NOP();
	
	CACHEC->PREFCR = (1 << CACHEC_PREFCR_UNLOCK_Pos) |
					(addr << CACHEC_PREFCR_ADDR_Pos);
	
	while((CACHEC->IF & (CACHEC_IF_UNLOK_Msk | CACHEC_IF_UNLFAIL_Msk)) == 0) __NOP();
	
	if(CACHEC->IF & CACHEC_IF_UNLOK_Msk) res = CACHE_RES_OK;
	else 							     res = CACHE_RES_FAIL;
	
	CACHEC->IF = CACHEC_IF_UNLOK_Msk | CACHEC_IF_UNLFAIL_Msk;	//清除标志
	
	return res;
}

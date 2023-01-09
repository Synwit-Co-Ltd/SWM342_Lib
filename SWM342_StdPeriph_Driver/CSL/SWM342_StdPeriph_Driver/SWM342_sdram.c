/****************************************************************************************************************************************** 
* 文件名称: SWM342_sdram.c
* 功能说明:	SWM342单片机的SDRAM驱动程序
* 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项:
* 版本日期: V1.1.0		2017年10月25日
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
#include "SWM342_sdram.h"


/****************************************************************************************************************************************** 
* 函数名称:	SDRAM_Init()
* 功能说明:	SDRAM控制器初始化
* 输    入: SDRAM_InitStructure * initStruct    包含 SDRAM 控制器相关设定值的结构体
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SDRAM_Init(SDRAM_InitStructure * initStruct)
{
	uint32_t cyclesPerUs = CyclesPerUs / (initStruct->ClkDiv + 1);	// 1us有多少个SDRAM_CLK周期
	
	SYS->CLKEN1 |= (1 << SYS_CLKEN1_SDRAM_Pos);
	
	SDRAMC->TIM = (initStruct->TimeTRC  << SDRAMC_TIM_TRC_Pos)  |
				  (initStruct->TimeTRRD << SDRAMC_TIM_TRRD_Pos) |
				  (initStruct->TimeTRCD << SDRAMC_TIM_TRCD_Pos) |
				  (initStruct->TimeTRAS << SDRAMC_TIM_TRAS_Pos) |
				  (initStruct->TimeTRP  << SDRAMC_TIM_TRP_Pos)  |
				  (2                    << SDRAMC_TIM_TWR_Pos)  |
				  ((initStruct->TimeTRC + 1) << SDRAMC_TIM_TRFC_Pos);
	SDRAMC->TIM2 = (50 << SDRAMC_TIM2_TXSR_Pos);
	
	SDRAMC->T64 = 200 * cyclesPerUs / 8;
	
	SDRAMC->RDDELAY = initStruct->CASLatency;
	SDRAMC->CR = (initStruct->NbrBank    << SDRAMC_CR_BANKCNT_Pos) |
				 (initStruct->NbrColAddr << SDRAMC_CR_COLBIT_Pos)  |
				 (initStruct->CASLatency << SDRAMC_CR_CASDELAY_Pos);
	
	for(int i = 0; i < 200 * CyclesPerUs; i++) __NOP();
	
	SDRAMC->T64 = 64 * 1000 * cyclesPerUs / (1 << initStruct->NbrRowAddr);
}


/****************************************************************************************************************************************** 
* 函数名称:	SDRAM_Enable()
* 功能说明:	SDRAM使能，使能后可读写SDRAM
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SDRAM_Enable(void)
{
	SDRAMC->CR |= SDRAMC_CR_PDEXIT_Msk;
	
	for(int i = 0; i < 100; i++) __NOP();
}


/****************************************************************************************************************************************** 
* 函数名称:	SDRAM_Disable()
* 功能说明:	SDRAM禁能，禁能后SDRAM颗粒进入低功耗模式、并自刷新，不可读写
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SDRAM_Disable(void)
{
	SDRAMC->CR |= SDRAMC_CR_PD_Msk | SDRAMC_CR_SR_Msk;
	
	for(int i = 0; i < 100; i++) __NOP();
}

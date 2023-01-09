/****************************************************************************************************************************************** 
* �ļ�����: SWM342_sdram.c
* ����˵��:	SWM342��Ƭ����SDRAM��������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����: V1.1.0		2017��10��25��
* ������¼: 
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
* ��������:	SDRAM_Init()
* ����˵��:	SDRAM��������ʼ��
* ��    ��: SDRAM_InitStructure * initStruct    ���� SDRAM ����������趨ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void SDRAM_Init(SDRAM_InitStructure * initStruct)
{
	uint32_t cyclesPerUs = CyclesPerUs / (initStruct->ClkDiv + 1);	// 1us�ж��ٸ�SDRAM_CLK����
	
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
* ��������:	SDRAM_Enable()
* ����˵��:	SDRAMʹ�ܣ�ʹ�ܺ�ɶ�дSDRAM
* ��    ��: ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void SDRAM_Enable(void)
{
	SDRAMC->CR |= SDRAMC_CR_PDEXIT_Msk;
	
	for(int i = 0; i < 100; i++) __NOP();
}


/****************************************************************************************************************************************** 
* ��������:	SDRAM_Disable()
* ����˵��:	SDRAM���ܣ����ܺ�SDRAM��������͹���ģʽ������ˢ�£����ɶ�д
* ��    ��: ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void SDRAM_Disable(void)
{
	SDRAMC->CR |= SDRAMC_CR_PD_Msk | SDRAMC_CR_SR_Msk;
	
	for(int i = 0; i < 100; i++) __NOP();
}

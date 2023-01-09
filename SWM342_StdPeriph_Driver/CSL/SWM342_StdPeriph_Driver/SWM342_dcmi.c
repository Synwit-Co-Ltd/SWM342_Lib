/****************************************************************************************************************************************** 
* �ļ�����: SWM342_dcmi.c
* ����˵��:	SWM342��Ƭ����DCMI�ӿ���������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����:	V1.0.0		2016��1��30��
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
#include "SWM342_dcmi.h"


/****************************************************************************************************************************************** 
* ��������:	DCMI_Init()
* ����˵��:	DCMI�ӿڳ�ʼ��
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
*			DCMI_InitStructure * initStruct    ����DCMI�ӿ�����趨ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DCMI_Init(DCMI_TypeDef * DCMIx, DCMI_InitStructure * initStruct)
{
	SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_DCMI_Pos);
	
	DCMI_Close(DCMIx);
	
	DCMI->CR = (initStruct->Mode        << DCMI_CR_CAPMODE_Pos)  |
			   (0                       << DCMI_CR_CROPEN_Pos)   |		// crop function disable
			   (initStruct->Format      << DCMI_CR_JPEG_Pos)     |
			   (0                       << DCMI_CR_EMBSYNC_Pos)  |		// hardware sync (DCMI_VSYNC & DCMI_HSYNC)
			   (initStruct->PCKPolarity << DCMI_CR_PCKPOL_Pos)   |
			   (initStruct->HSPolarity  << DCMI_CR_HSPOL_Pos)    |
			   (initStruct->VSPolarity  << DCMI_CR_VSPOL_Pos)    |
			   (initStruct->BusWidth    << DCMI_CR_BUSWIDTH_Pos) |
			   (0                       << DCMI_CR_LINESEL_Pos)  |		// all line
			   (0                       << DCMI_CR_BYTESEL_Pos);		// all byte
	
	DCMI->ICR = 0x1F;
	DCMI->IER = (initStruct->IntEOCEn << DCMI_IER_FRAME_Pos);
	
	if(initStruct->IntEOCEn)
		NVIC_EnableIRQ(DCMI_IRQn);
}


/****************************************************************************************************************************************** 
* ��������:	DCMI_Open()
* ����˵��:	DCMI�ӿڿ���
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DCMI_Open(DCMI_TypeDef * DCMIx)
{
	DCMIx->CR |= DCMI_CR_ENA_Msk;
}


/****************************************************************************************************************************************** 
* ��������:	DCMI_Close()
* ����˵��:	DCMI�ӿڹر�
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DCMI_Close(DCMI_TypeDef * DCMIx)
{
	DCMIx->CR &= ~DCMI_CR_ENA_Msk;
}


/****************************************************************************************************************************************** 
* ��������:	DCMI_Start()
* ����˵��:	DCMI�ӿ��������񣬼�⵽֡��ʼ��ʼ��������
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DCMI_Start(DCMI_TypeDef * DCMIx)
{
	DCMIx->CR |= DCMI_CR_CAPTURE_Msk;
}


/****************************************************************************************************************************************** 
* ��������:	DCMI_Stop()
* ����˵��:	DCMI�ӿ�ֹͣ����
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
* ��    ��: ��
* ע������: ֡���ݲ��������ִ�д˺��������������������ֹͣ�����ǵȵ���ǰ֡������ɺ�Ż�����ֹͣ
******************************************************************************************************************************************/
void DCMI_Stop(DCMI_TypeDef * DCMIx)
{
	DCMIx->CR &= ~DCMI_CR_CAPTURE_Msk;
}


/****************************************************************************************************************************************** 
* ��������:	DCMI_DataAvaliable()
* ����˵��:	DCMI�ӿ������ݿɶ�
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
bool DCMI_DataAvaliable(DCMI_TypeDef * DCMIx)
{
	return DCMIx->SR & DCMI_SR_FIFONE_Msk;
}

/****************************************************************************************************************************************** 
* ��������:	DCMI_SetCropWindow()
* ����˵��:	Set Crop Window
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
*			uint16_t start_x		start pixel point in line
*			uint16_t start_y		start line
*			uint16_t count_x		pixel count in line
*			uint16_t count_y		line count
* ��    ��: ��
* ע������: Valid only for 8-bit parallel data
******************************************************************************************************************************************/
void DCMI_SetCropWindow(DCMI_TypeDef * DCMIx, uint16_t start_x, uint16_t start_y, uint16_t count_x, uint16_t count_y)
{
	DCMIx->CWPOS = ( start_y      << DCMI_CWPOS_LINE_Pos) |
				   ( start_x      << DCMI_CWPOS_PIXEL_Pos);
	
	DCMIx->CWSIZ = ((count_y - 1) << DCMI_CWSIZ_LINE_Pos) |
				   ((count_x - 1) << DCMI_CWSIZ_PIXEL_Pos);
}


/****************************************************************************************************************************************** 
* ��������:	DCMI_CropSwitch()
* ����˵��:	DCMI ���д��ڹ��ܿ���
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
*			bool on					true �������й���   false �رղ��й���
* ��    ��: ��
* ע������: Valid only for 8-bit parallel data
******************************************************************************************************************************************/
void DCMI_CropSwitch(DCMI_TypeDef * DCMIx, bool on)
{
	if(on)
		DCMIx->CR |=  DCMI_CR_CROPEN_Msk;
	else
		DCMIx->CR &= ~DCMI_CR_CROPEN_Msk;
}


/****************************************************************************************************************************************** 
* ��������:	DCMI_DownSampling()
* ����˵��:	DCMI ������
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
*			DCMI_DownSampling_Line line_sel	��������ѡ��
*			DCMI_DownSampling_Byte byte_sel	�������ֽ�ѡ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DCMI_DownSampling(DCMI_TypeDef * DCMIx, DCMI_DownSampling_Line line_sel, DCMI_DownSampling_Byte byte_sel)
{
	DCMIx->CR &= ~(DCMI_CR_LINESEL_Msk | DCMI_CR_LINE2nd_Msk |
				   DCMI_CR_BYTESEL_Msk | DCMI_CR_BYTE2nd_Msk);
	
	DCMIx->CR |= (line_sel << DCMI_CR_LINESEL_Pos) |
				 (byte_sel << DCMI_CR_BYTESEL_Pos);
}


/****************************************************************************************************************************************** 
* ��������:	DCMI_EmbeddedSync()
* ����˵��:	���� Embedded Synchronization Code�������� Embedded synchronization code sync
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
*			uint8_t frame_start		Frame start delimiter code
*			uint8_t line_start		Line start delimiter code
*			uint8_t line_end		Line end delimiter code
*			uint8_t frame_end		Frame end delimiter code
* ��    ��: ��
* ע������: Valid only for 8-bit parallel data
******************************************************************************************************************************************/
void DCMI_EmbeddedSync(DCMI_TypeDef * DCMIx, uint8_t frame_start, uint8_t line_start, uint8_t line_end, uint8_t frame_end)
{
	DCMIx->ESC = (frame_start << DCMI_ESC_FRMSTA_Pos) |
				 (line_start  << DCMI_ESC_LINSTA_Pos) |
				 (line_end    << DCMI_ESC_LINEND_Pos) |
				 (frame_end   << DCMI_ESC_FRMEND_Pos);
	
	DCMIx->ESM = (0xFF        << DCMI_ESM_FRMSTA_Pos) |
				 (0xFF		  << DCMI_ESM_LINSTA_Pos) |
				 (0xFF		  << DCMI_ESM_LINEND_Pos) |
				 (0xFF		  << DCMI_ESM_FRMEND_Pos);
	
	DCMIx->CR |= DCMI_CR_EMBSYNC_Msk;
}


/****************************************************************************************************************************************** 
* ��������:	DCMI_INTEn()
* ����˵��:	�ж�ʹ��
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
*			uint32_t it				interrupt type����Чֵ DCMI_IT_FRAME��DCMI_IT_FIFOOV��DCMI_IT_ESCERR��DCMI_IT_VSYNC��DCMI_IT_HSYNC ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DCMI_INTEn(DCMI_TypeDef * DCMIx, uint32_t it)
{
	DCMI->IER |=  it;
}

/****************************************************************************************************************************************** 
* ��������:	DCMI_INTDis()
* ����˵��:	�жϽ�ֹ
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
*			uint32_t it				interrupt type����Чֵ DCMI_IT_FRAME��DCMI_IT_FIFOOV��DCMI_IT_ESCERR��DCMI_IT_VSYNC��DCMI_IT_HSYNC ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DCMI_INTDis(DCMI_TypeDef * DCMIx, uint32_t it)
{
	DCMI->IER &= ~it;
}

/****************************************************************************************************************************************** 
* ��������:	DCMI_INTClr()
* ����˵��:	�жϱ�־���
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
*			uint32_t it				interrupt type����Чֵ DCMI_IT_FRAME��DCMI_IT_FIFOOV��DCMI_IT_ESCERR��DCMI_IT_VSYNC��DCMI_IT_HSYNC ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void DCMI_INTClr(DCMI_TypeDef * DCMIx, uint32_t it)
{
	DCMI->ICR = it;
}

/****************************************************************************************************************************************** 
* ��������:	DCMI_INTStat()
* ����˵��:	�ж�״̬��ѯ
* ��    ��: DCMI_TypeDef * DCMIx	ָ��Ҫ�����õ�DCMI�ӿڣ���Чֵ����DCMI
*			uint32_t it				interrupt type����Чֵ DCMI_IT_FRAME��DCMI_IT_FIFOOV��DCMI_IT_ESCERR��DCMI_IT_VSYNC��DCMI_IT_HSYNC ���䡰��
* ��    ��: uint32_t				true �жϷ���    false �ж�δ����
* ע������: ��
******************************************************************************************************************************************/
bool DCMI_INTStat(DCMI_TypeDef * DCMIx, uint32_t it)
{
	return DCMI->RIS & it;
}

/****************************************************************************************************************************************** 
* 文件名称: SWM342_dcmi.c
* 功能说明:	SWM342单片机的DCMI接口驱动函数
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
#include "SWM342_dcmi.h"


/****************************************************************************************************************************************** 
* 函数名称:	DCMI_Init()
* 功能说明:	DCMI接口初始化
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
*			DCMI_InitStructure * initStruct    包含DCMI接口相关设定值的结构体
* 输    出: 无
* 注意事项: 无
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
* 函数名称:	DCMI_Open()
* 功能说明:	DCMI接口开启
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void DCMI_Open(DCMI_TypeDef * DCMIx)
{
	DCMIx->CR |= DCMI_CR_ENA_Msk;
}


/****************************************************************************************************************************************** 
* 函数名称:	DCMI_Close()
* 功能说明:	DCMI接口关闭
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void DCMI_Close(DCMI_TypeDef * DCMIx)
{
	DCMIx->CR &= ~DCMI_CR_ENA_Msk;
}


/****************************************************************************************************************************************** 
* 函数名称:	DCMI_Start()
* 功能说明:	DCMI接口启动捕获，检测到帧起始后开始捕获数据
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void DCMI_Start(DCMI_TypeDef * DCMIx)
{
	DCMIx->CR |= DCMI_CR_CAPTURE_Msk;
}


/****************************************************************************************************************************************** 
* 函数名称:	DCMI_Stop()
* 功能说明:	DCMI接口停止捕获
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
* 输    出: 无
* 注意事项: 帧数据捕获过程中执行此函数，捕获操作不会立即停止，而是等到当前帧捕获完成后才会真正停止
******************************************************************************************************************************************/
void DCMI_Stop(DCMI_TypeDef * DCMIx)
{
	DCMIx->CR &= ~DCMI_CR_CAPTURE_Msk;
}


/****************************************************************************************************************************************** 
* 函数名称:	DCMI_DataAvaliable()
* 功能说明:	DCMI接口有数据可读
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
bool DCMI_DataAvaliable(DCMI_TypeDef * DCMIx)
{
	return DCMIx->SR & DCMI_SR_FIFONE_Msk;
}

/****************************************************************************************************************************************** 
* 函数名称:	DCMI_SetCropWindow()
* 功能说明:	Set Crop Window
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
*			uint16_t start_x		start pixel point in line
*			uint16_t start_y		start line
*			uint16_t count_x		pixel count in line
*			uint16_t count_y		line count
* 输    出: 无
* 注意事项: Valid only for 8-bit parallel data
******************************************************************************************************************************************/
void DCMI_SetCropWindow(DCMI_TypeDef * DCMIx, uint16_t start_x, uint16_t start_y, uint16_t count_x, uint16_t count_y)
{
	DCMIx->CWPOS = ( start_y      << DCMI_CWPOS_LINE_Pos) |
				   ( start_x      << DCMI_CWPOS_PIXEL_Pos);
	
	DCMIx->CWSIZ = ((count_y - 1) << DCMI_CWSIZ_LINE_Pos) |
				   ((count_x - 1) << DCMI_CWSIZ_PIXEL_Pos);
}


/****************************************************************************************************************************************** 
* 函数名称:	DCMI_CropSwitch()
* 功能说明:	DCMI 裁切窗口功能开关
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
*			bool on					true 开启裁切功能   false 关闭裁切功能
* 输    出: 无
* 注意事项: Valid only for 8-bit parallel data
******************************************************************************************************************************************/
void DCMI_CropSwitch(DCMI_TypeDef * DCMIx, bool on)
{
	if(on)
		DCMIx->CR |=  DCMI_CR_CROPEN_Msk;
	else
		DCMIx->CR &= ~DCMI_CR_CROPEN_Msk;
}


/****************************************************************************************************************************************** 
* 函数名称:	DCMI_DownSampling()
* 功能说明:	DCMI 降采样
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
*			DCMI_DownSampling_Line line_sel	降采样行选择
*			DCMI_DownSampling_Byte byte_sel	降采样字节选择
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void DCMI_DownSampling(DCMI_TypeDef * DCMIx, DCMI_DownSampling_Line line_sel, DCMI_DownSampling_Byte byte_sel)
{
	DCMIx->CR &= ~(DCMI_CR_LINESEL_Msk | DCMI_CR_LINE2nd_Msk |
				   DCMI_CR_BYTESEL_Msk | DCMI_CR_BYTE2nd_Msk);
	
	DCMIx->CR |= (line_sel << DCMI_CR_LINESEL_Pos) |
				 (byte_sel << DCMI_CR_BYTESEL_Pos);
}


/****************************************************************************************************************************************** 
* 函数名称:	DCMI_EmbeddedSync()
* 功能说明:	设置 Embedded Synchronization Code，并开启 Embedded synchronization code sync
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
*			uint8_t frame_start		Frame start delimiter code
*			uint8_t line_start		Line start delimiter code
*			uint8_t line_end		Line end delimiter code
*			uint8_t frame_end		Frame end delimiter code
* 输    出: 无
* 注意事项: Valid only for 8-bit parallel data
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
* 函数名称:	DCMI_INTEn()
* 功能说明:	中断使能
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
*			uint32_t it				interrupt type，有效值 DCMI_IT_FRAME、DCMI_IT_FIFOOV、DCMI_IT_ESCERR、DCMI_IT_VSYNC、DCMI_IT_HSYNC 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void DCMI_INTEn(DCMI_TypeDef * DCMIx, uint32_t it)
{
	DCMI->IER |=  it;
}

/****************************************************************************************************************************************** 
* 函数名称:	DCMI_INTDis()
* 功能说明:	中断禁止
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
*			uint32_t it				interrupt type，有效值 DCMI_IT_FRAME、DCMI_IT_FIFOOV、DCMI_IT_ESCERR、DCMI_IT_VSYNC、DCMI_IT_HSYNC 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void DCMI_INTDis(DCMI_TypeDef * DCMIx, uint32_t it)
{
	DCMI->IER &= ~it;
}

/****************************************************************************************************************************************** 
* 函数名称:	DCMI_INTClr()
* 功能说明:	中断标志清除
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
*			uint32_t it				interrupt type，有效值 DCMI_IT_FRAME、DCMI_IT_FIFOOV、DCMI_IT_ESCERR、DCMI_IT_VSYNC、DCMI_IT_HSYNC 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void DCMI_INTClr(DCMI_TypeDef * DCMIx, uint32_t it)
{
	DCMI->ICR = it;
}

/****************************************************************************************************************************************** 
* 函数名称:	DCMI_INTStat()
* 功能说明:	中断状态查询
* 输    入: DCMI_TypeDef * DCMIx	指定要被设置的DCMI接口，有效值包括DCMI
*			uint32_t it				interrupt type，有效值 DCMI_IT_FRAME、DCMI_IT_FIFOOV、DCMI_IT_ESCERR、DCMI_IT_VSYNC、DCMI_IT_HSYNC 及其“或”
* 输    出: uint32_t				true 中断发生    false 中断未发生
* 注意事项: 无
******************************************************************************************************************************************/
bool DCMI_INTStat(DCMI_TypeDef * DCMIx, uint32_t it)
{
	return DCMI->RIS & it;
}

/****************************************************************************************************************************************** 
* 文件名称: SWM342_sra.c
* 功能说明:	SWM342单片机的SRA（Scaling、Rotate）驱动函数
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
#include <math.h>
#include <stdlib.h>
#include "SWM342.h"
#include "SWM342_sra.h"


/****************************************************************************************************************************************** 
* 函数名称:	SRA_Init()
* 功能说明:	SRA初始化
* 输    入: SRA_InitStructure * initStruct
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SRA_Init(SRA_InitStructure * initStruct)
{
	SYS->CLKEN1 |= (1 << SYS_CLKEN1_SRA_Pos);
	
	SRA->IFE = 0xF;		// Interrupt Flag Enable
	SRA->IF  = 0xF;		// Interrupt Clear
	SRA->IE  = (initStruct->IntEOTEn << SRA_IE_FRAME_Pos);
	
	if(initStruct->IntEOTEn)
		NVIC_EnableIRQ(SRA_IRQn);
}


#define PI 3.141592653589793
static double deg2rad(double degree)
{
	return degree / 180 * PI;
}

/****************************************************************************************************************************************** 
* 函数名称:	SRA_Transform()
* 功能说明:	SRA缩放、旋转变换参数配置，之后执行 SRA_Start() 执行变换
* 输    入: SRA_TransformParam * transParam		缩放、旋转变换参数
* 输    出: bool								true 参数正确，可以执行缩放、旋转变换   false 参数错误，无法执行缩放、旋转变换
*			uint16_t *dstWidth					变换后图片的宽度
*			uint16_t *dstHeight					变换后图片的高度
* 注意事项: 无
******************************************************************************************************************************************/
bool SRA_Transform(SRA_TransformParam * transParam, uint16_t *dstWidth, uint16_t *dstHeight)
{
	int degree = transParam->RotateAngle;
	int mirror = transParam->RotateAngle - 360;
	if((mirror == 1) || (mirror == 2) || (mirror == 3))		// 镜像
		degree = 0;
	else													// 旋转
		mirror = 0;
	
	double real_factor_x = transParam->ScaleWidth  * 1.0 / transParam->ImageWidth,
		   real_factor_y = transParam->ScaleHeight * 1.0 / transParam->ImageHeight;
	if((real_factor_x > 4) | (real_factor_x < 0.5) |
	   (real_factor_y > 4) | (real_factor_y < 0.5))
		return false;
	
	int src_width_m1  = transParam->ImageWidth  - 1,
		src_height_m1 = transParam->ImageHeight - 1,
		scl_width_m1  = transParam->ScaleWidth  - 1,
		scl_height_m1 = transParam->ScaleHeight - 1;
	
	double scl_factor_x = src_width_m1  * 1.0 / scl_width_m1,
		   scl_factor_y = src_height_m1 * 1.0 / scl_height_m1;
	int    scl_factor_x_para = round(scl_factor_x * pow(2, 13)),
		   scl_factor_y_para = round(scl_factor_y * pow(2, 13));
	
	double sra_sin = sin(deg2rad(degree)),
		   sra_cos = cos(deg2rad(degree));
	int    sra_sin_para = round(sra_sin * pow(2, 13)),
		   sra_cos_para = round(sra_cos * pow(2, 13));
	double sra_sin_fp = sra_sin_para / pow(2, 13),
		   sra_cos_fp = sra_cos_para / pow(2, 13);
	
	double sra_sin_xfact = sra_sin_para * scl_factor_x_para,
		   sra_sin_yfact = sra_sin_para * scl_factor_y_para,
		   sra_cos_xfact = sra_cos_para * scl_factor_x_para,
		   sra_cos_yfact = sra_cos_para * scl_factor_y_para;
	int    sra_sin_xfact_para = floor(sra_sin_xfact / pow(2, 13)),
		   sra_sin_yfact_para = floor(sra_sin_yfact / pow(2, 13)),
		   sra_cos_xfact_para = floor(sra_cos_xfact / pow(2, 13)),
		   sra_cos_yfact_para = floor(sra_cos_yfact / pow(2, 13));
	double sra_sin_xfact_fp = sra_sin_xfact_para / pow(2, 13),
		   sra_sin_yfact_fp = sra_sin_yfact_para / pow(2, 13),
		   sra_cos_xfact_fp = sra_cos_xfact_para / pow(2, 13),
		   sra_cos_yfact_fp = sra_cos_yfact_para / pow(2, 13);
	
	if(sra_sin < 0)
	{
		sra_sin_xfact_para = pow(2, 16) + sra_sin_xfact_para;
		sra_sin_yfact_para = pow(2, 16) + sra_sin_yfact_para;
	}
	if(sra_cos < 0)
	{
		sra_cos_xfact_para = pow(2, 16) + sra_cos_xfact_para;
		sra_cos_yfact_para = pow(2, 16) + sra_cos_yfact_para;
	}

	double dst_offset_x,
		   dst_offset_y,
		   dst_tile_offset_x,
		   dst_tile_offset_y;
	if(mirror == 1)			// x-mirror
	{
		dst_offset_x      = scl_width_m1;
		dst_offset_y      = 0;
		dst_tile_offset_x = 7 * sra_cos_xfact_fp;
		dst_tile_offset_y = 0;
	}
	else if(mirror == 2)	// y-mirror
	{
		dst_offset_x      = 0;
		dst_offset_y      = scl_height_m1;
		dst_tile_offset_x = 0;
		dst_tile_offset_y = 7 * sra_cos_yfact_fp;
	}
	else if(mirror == 3)	// xy-mirror
	{
		dst_offset_x      = scl_width_m1;
		dst_offset_y      = scl_height_m1;
		dst_tile_offset_x = 7 * sra_cos_xfact_fp;
		dst_tile_offset_y = 7 * sra_cos_yfact_fp;
	}
	else
	{
		if((degree >= 0) && (degree < 90))
		{
			dst_offset_x      = - sra_sin_fp * scl_height_m1;
			dst_offset_y      = 0;
			dst_tile_offset_x = 0;
			dst_tile_offset_y = 7 * sra_sin_yfact_fp;
		}
		else if((degree >= 90) && (degree < 180))
		{
			dst_offset_x      = sra_cos_fp * scl_width_m1 - sra_sin_fp * scl_height_m1;
			dst_offset_y      = sra_cos_fp * scl_height_m1;
			dst_tile_offset_x = -7 * sra_cos_xfact_fp;
			dst_tile_offset_y = -7 * sra_cos_yfact_fp + 7 * sra_sin_yfact_fp;
		}
		else if((degree >= 180) && (degree < 270))
		{
			dst_offset_x      = sra_cos_fp * scl_width_m1;
			dst_offset_y      = sra_cos_fp * scl_height_m1 + sra_sin_fp * scl_width_m1;
			dst_tile_offset_x = -7 * sra_cos_xfact_fp - 7 * sra_sin_xfact_fp;
			dst_tile_offset_y = -7 * sra_cos_yfact_fp;
		}
		else
		{
			dst_offset_x      = 0;
			dst_offset_y      = sra_sin_fp * scl_width_m1;
			dst_tile_offset_x = -7 * sra_sin_xfact_fp;
			dst_tile_offset_y = 0;
		}
	}

	double dst_tile_remap_stx,
		   dst_tile_remap_sty;
	if(mirror == 1)			// x-mirror
	{
		dst_tile_remap_stx = (-7 + dst_offset_x) * sra_cos_xfact_fp; 	// (t,0)
		dst_tile_remap_sty = ( 0 + dst_offset_y) * sra_cos_yfact_fp; 	// (t,0)
	}
	else if(mirror == 2)	// y-mirror 
	{
		dst_tile_remap_stx = ( 0 + dst_offset_x) * sra_cos_xfact_fp; 	// (0,t)
		dst_tile_remap_sty = (-7 + dst_offset_y) * sra_cos_yfact_fp; 	// (0,t)
	}
	else if(mirror == 3)	// xy-mirror 
	{
		dst_tile_remap_stx = (-7 + dst_offset_x) * sra_cos_xfact_fp; 	// (t,0)
		dst_tile_remap_sty = (-7 + dst_offset_y) * sra_cos_yfact_fp; 	// (0,t)
	}
	else
	{
		if((degree >= 0) && (degree < 90))
		{
			dst_tile_remap_stx = (0 + dst_offset_x) * (+sra_cos_xfact_fp) +(0 + dst_offset_y) * sra_sin_xfact_fp; 	// (0,0)
			dst_tile_remap_sty = (7 + dst_offset_x) * (-sra_sin_yfact_fp) +(0 + dst_offset_y) * sra_cos_yfact_fp; 	// (t,0)
		}
		else if((degree >= 90) && (degree < 180))
		{
			dst_tile_remap_stx = (7 + dst_offset_x) * (+sra_cos_xfact_fp) +(0 + dst_offset_y) * sra_sin_xfact_fp;	// (t,0)
			dst_tile_remap_sty = (7 + dst_offset_x) * (-sra_sin_yfact_fp) +(7 + dst_offset_y) * sra_cos_yfact_fp;	// (t,t)
		}
		else if((degree >= 180) && (degree < 270))
		{
			dst_tile_remap_stx = (7 + dst_offset_x) * (+sra_cos_xfact_fp) +(7 + dst_offset_y) * sra_sin_xfact_fp;	// (t,t)
			dst_tile_remap_sty = (0 + dst_offset_x) * (-sra_sin_yfact_fp) +(7 + dst_offset_y) * sra_cos_yfact_fp;	// (0,t)
		}
		else
		{
			dst_tile_remap_stx = (0 + dst_offset_x) * (+sra_cos_xfact_fp) +(7 + dst_offset_y) * sra_sin_xfact_fp;	// (0,t)
			dst_tile_remap_sty = (0 + dst_offset_x) * (-sra_sin_yfact_fp) +(0 + dst_offset_y) * sra_cos_yfact_fp;	// (0,0)
		}
	}

	// SRC first tile start position
	double src_tile_stx    = dst_tile_remap_stx,
		   src_tile_sty    = dst_tile_remap_sty;
	double src_tile_stx_fp = round(src_tile_stx * pow(2, 13)) / pow(2, 13),
		   src_tile_sty_fp = round(src_tile_sty * pow(2, 13)) / pow(2, 13);
	int    src_tile_stx_para,
		   src_tile_sty_para;
	
#define IMG_SIZE_W 12
	if(src_tile_stx < 0)
		src_tile_stx_para = round(src_tile_stx * pow(2, 13)) + pow(2, IMG_SIZE_W+13+1);
	else
		src_tile_stx_para = round(src_tile_stx * pow(2, 13));
	
	if(src_tile_sty < 0)
		src_tile_sty_para = round(src_tile_sty * pow(2, 13)) + pow(2, IMG_SIZE_W+13+1);
	else
		src_tile_sty_para = round(src_tile_sty * pow(2, 13));

	// DST tile first pixel offset
	int dst_tile_offset_x_para,
		dst_tile_offset_y_para;
	
	if(dst_tile_offset_x < 0)
		return false;
	else
		dst_tile_offset_x_para = round(dst_tile_offset_x * pow(2, 13));

	if(dst_tile_offset_y < 0)
		return false;
	else
		dst_tile_offset_y_para = round(dst_tile_offset_y * pow(2, 13));

	int dst_width  = ceil(scl_width_m1 * fabs(sra_cos_fp) + scl_height_m1 * fabs(sra_sin_fp)) + 1,
	    dst_height = ceil(scl_width_m1 * fabs(sra_sin_fp) + scl_height_m1 * fabs(sra_cos_fp)) + 1;

	int src_stride, dst_stride;
	switch(transParam->Format & 3)
	{
	case SRA_FMT_MONO8b:
		src_stride = transParam->ImageWidth / 4;
		dst_stride = ceil(dst_width / 4.0);
		dst_width  = ceil(dst_width / 4.0) * 4;
		break;
	
	case SRA_FMT_RGB565:
		src_stride = transParam->ImageWidth / 2;
		dst_stride = ceil(dst_width / 2.0);
		dst_width  = ceil(dst_width / 2.0) * 2;
		break;
	
	case SRA_FMT_RGB888:
		src_stride = transParam->ImageWidth * 3 / 4;
		dst_stride = ceil(dst_width * 3.0 / 4);
		dst_width  = dst_width;
		break;
	
	case SRA_FMT_RGB888X:
		src_stride = transParam->ImageWidth;
		dst_stride = dst_width;
		dst_width  = dst_width;
		break;
	}
	
	SRA->CFG = (transParam->Format << SRA_CFG_PIXFMT_Pos)  |
			   (mirror			   << SRA_CFG_ROTTYP_Pos)  |
			   (0				   << SRA_CFG_SXSWAP_Pos)  |
			   (0				   << SRA_CFG_DXSWAP_Pos)  |
			   (1				   << SRA_CFG_ROTAAEN_Pos) |
			   (0				   << SRA_CFG_DUMMYBP_Pos);
	
	SRA->SINP = (sra_sin_xfact_para << SRA_SINP_SINX_Pos) |
				(sra_sin_yfact_para << SRA_SINP_SINY_Pos);
	SRA->COSP = (sra_cos_xfact_para << SRA_COSP_COSX_Pos) |
				(sra_cos_yfact_para << SRA_COSP_COSY_Pos);
	
	SRA->BGCOLOR = transParam->BGColor;
	
	SRA->SRCBASE = transParam->ImageAddr;
	SRA->SRCSIZE = ( src_width_m1    << SRA_SRCSIZE_WIDTH_Pos) |
				   ( src_height_m1   << SRA_SRCSIZE_HEIGHT_Pos);
	SRA->SRCSTRIDE = src_stride;
	
	SRA->DSTBASE = transParam->OutputAddr;
	SRA->DSTSIZE = ((dst_width  - 1) << SRA_DSTSIZE_WIDTH_Pos) |
				   ((dst_height - 1) << SRA_DSTSIZE_HEIGHT_Pos);
	SRA->DSTSTRIDE = dst_stride;
	
	SRA->SRC_TILE_POSX = src_tile_stx_para;
	SRA->SRC_TILE_POSY = src_tile_sty_para;
	SRA->DST_TILE_OFFX = dst_tile_offset_x_para;
	SRA->DST_TILE_OFFY = dst_tile_offset_y_para;
	
	*dstWidth  = dst_width;
	*dstHeight = dst_height;
	
	return true;
}

/****************************************************************************************************************************************** 
* 函数名称:	SRA_Start()
* 功能说明:	启动变换
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SRA_Start(void)
{
	SRA->CR = SRA_CR_START_Msk;
}

/****************************************************************************************************************************************** 
* 函数名称:	SRA_Abort()
* 功能说明:	终止变换
* 输    入: 无
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SRA_Abort(void)
{
	SRA->CR = SRA_CR_ABORT_Msk;
	
	while(SRA->SR & SRA_SR_ABORT_Msk) __NOP();
}

/****************************************************************************************************************************************** 
* 函数名称:	SRA_Busy()
* 功能说明:	查询是否正在变换
* 输    入: 无
* 输    出: uint32_t				1 正在执行变换操作    0 未在执行变换操作
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t SRA_Busy(void)
{
	return (SRA->SR & SRA_SR_BUSY_Msk) ? 1 : 0;
}


/****************************************************************************************************************************************** 
* 函数名称:	SRA_INTEn()
* 功能说明:	中断使能
* 输    入: uint32_t it				interrupt type，有效值 SRA_IT_FRAME、SRA_IT_8LINE、SRA_IT_ROTERR、SRA_IT_BUSERR 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SRA_INTEn(uint32_t it)
{
	SRA->IE |=  (it << 4);
}

/****************************************************************************************************************************************** 
* 函数名称:	SRA_INTDis()
* 功能说明:	中断禁止
* 输    入: uint32_t it				interrupt type，有效值 SRA_IT_FRAME、SRA_IT_8LINE、SRA_IT_ROTERR、SRA_IT_BUSERR 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SRA_INTDis(uint32_t it)
{
	SRA->IE &= ~(it << 4);
}

/****************************************************************************************************************************************** 
* 函数名称:	SRA_INTClr()
* 功能说明:	中断标志清除
* 输    入: uint32_t it				interrupt type，有效值有效值 SRA_IT_FRAME、SRA_IT_8LINE、SRA_IT_ROTERR、SRA_IT_BUSERR 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void SRA_INTClr(uint32_t it)
{
	SRA->IF = it;
}

/****************************************************************************************************************************************** 
* 函数名称:	SRA_INTStat()
* 功能说明:	中断状态查询
* 输    入: uint32_t it				interrupt type，有效值 SRA_IT_FRAME、SRA_IT_8LINE、SRA_IT_ROTERR、SRA_IT_BUSERR 及其“或”
* 输    出: uint32_t				1 中断发生    0 中断未发生
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t SRA_INTStat(uint32_t it)
{
	return (SRA->IF & it) ? 1 : 0;
}

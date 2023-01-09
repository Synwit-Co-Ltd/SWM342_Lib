/****************************************************************************************************************************************** 
* 文件名称: SWM342_canfd.c
* 功能说明:	SWM342单片机的CANFD模块驱动库
* 技术支持:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* 注意事项: 
* 版本日期:	V1.1.0		2017年10月25日
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
#include <string.h>
#include "SWM342.h"
#include "SWM342_canfd.h"


static int CANFD_Mode = CANFD_MODE_CONFIG;


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_Init()
* 功能说明:	CAN接口初始化
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			CANFD_InitStructure * initStruct    包含CAN接口相关设定值的结构体
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CANFD_Init(CANFD_TypeDef * CANx, CANFD_InitStructure * initStruct)
{	
	switch((uint32_t)CANx)
	{
	case ((uint32_t)CAN0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_CAN0_Pos);
		break;
	}
	
	CANFD_Close(CANx);	//一些关键寄存器只能在CAN关闭时设置
	
	CANFD_Mode = initStruct->Mode;
	
	CANx->CR = (CANFD_MODE_CONFIG        << CANFD_CR_MODE_Pos)   |
			   (CANFD_RETRY_Always       << CANFD_CR_RETX_Pos)   |
			   (initStruct->RXSuccessIEn << CANFD_CR_IERXOK_Pos) |
			   (initStruct->RXOverrunIEn << CANFD_CR_IERXOV_Pos) |
			   (initStruct->TXSuccessIEn << CANFD_CR_IETXOK_Pos) |
			   (initStruct->ErrorIEn     << CANFD_CR_IEERR_Pos);
	
	CANFD_SetBaudrate(CANx, 0, initStruct->Baudrate, initStruct->CAN_bs1, initStruct->CAN_bs2, initStruct->CAN_sjw);
	
	if(initStruct->RXSuccessIEn | initStruct->RXOverrunIEn | initStruct->TXSuccessIEn | initStruct->ErrorIEn)
	{
		switch((uint32_t)CANx)
		{
		case ((uint32_t)CAN0): NVIC_EnableIRQ(CAN0_IRQn); break;
		}
	}
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_Open()
* 功能说明:	CAN接口打开
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CANFD_Open(CANFD_TypeDef * CANx)
{
	CANx->CR = CANx->CR & (~CANFD_CR_MODE_Msk) | (CANFD_Mode << CANFD_CR_MODE_Pos);
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_Close()
* 功能说明:	CAN接口关闭
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CANFD_Close(CANFD_TypeDef * CANx)
{
	CANx->CR = CANx->CR & (~CANFD_CR_MODE_Msk) | (CANFD_MODE_CONFIG << CANFD_CR_MODE_Pos);
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_Transmit()
* 功能说明:	CAN发送数据
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t txbuf		指定要使用哪个TX Buffer，有效值包括CANFD_TXBUF_0、CANFD_TXBUF_1、CANFD_TXBUF_2
*			CANFD_Message * msg	要发送的消息存储在此结构体变量中
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CANFD_Transmit(CANFD_TypeDef * CANx, uint32_t txbuf, CANFD_Message * msg)
{
	uint16_t can_BID, can_EID;
	if((msg->format & CANFD_FRAME_EXT) == 0)
	{
		can_BID = msg->id & 0x7FF;
		can_EID = 0;
	}
	else
	{
		can_BID = msg->id >> 18;
		can_EID = msg->id & 0x3FFFF;
	}
	
	CANx->TX[txbuf].CR0 = (msg->remote               << CANFD_CR0_RTR_Pos)  |
						  ((msg->format & 1 ? 1 : 0) << CANFD_CR0_IDE_Pos)  |
						  ((msg->format & 2 ? 1 : 0) << CANFD_CR0_FDEN_Pos) |
						  ((msg->format & 4 ? 1 : 0) << CANFD_CR0_BRSW_Pos) |
						  (((can_EID >>  0) & 0x3FF) << CANFD_CR0_EID0T9_Pos);
	
	int dlc = msg->size;
	switch(dlc)
	{
	case 12: dlc =  9;  break;
	case 16: dlc = 10;  break;
	case 20: dlc = 11;  break;
	case 24: dlc = 12;  break;
	case 32: dlc = 13;  break;
	case 48: dlc = 14;  break;
	case 64: dlc = 15;  break;
	}
	
	CANx->TX[txbuf].CR1 = (((can_EID >> 10) & 0x0FF) << CANFD_CR1_EID10T17_Pos) |
						  (can_BID                   << CANFD_CR1_BID_Pos)      |
						  (dlc                       << CANFD_CR1_DLC_Pos);
	
	memcpy((void *)CANx->TXBUF[txbuf], msg->buff, msg->size);
	
	CANx->CR |= (1 << (CANFD_CR_TBF0REQ_Pos - txbuf));
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_Receive()
* 功能说明:	CAN接收数据
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t rxbuf		指定要使用哪个的RX Buffer，有效值包括CANFD_RXBUF_0、CANFD_RXBUF_1
*			CANFD_Message * msg	接收到的消息存储在此结构体变量中
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CANFD_Receive(CANFD_TypeDef * CANx, uint32_t rxbuf, CANFD_Message * msg)
{
	uint32_t cr0 = CANx->RX[rxbuf].CR0,
			 cr1 = CANx->RX[rxbuf].CR1;
	
	msg->format = ((cr0 & CANFD_CR0_IDE_Msk  ? 1 : 0) << 0) |
				  ((cr0 & CANFD_CR0_FDEN_Msk ? 1 : 0) << 1) |
				  ((cr0 & CANFD_CR0_BRSW_Msk ? 1 : 0) << 2);
	
	if((msg->format & CANFD_FRAME_EXT) == 0)
		msg->id = (cr1 & CANFD_CR1_BID_Msk) >> CANFD_CR1_BID_Pos;
	else
		msg->id = (((cr0 & CANFD_CR0_EID0T9_Msk  ) >> CANFD_CR0_EID0T9_Pos  ) <<  0) |
				  (((cr1 & CANFD_CR1_EID10T17_Msk) >> CANFD_CR1_EID10T17_Pos) << 10) |
				  (((cr1 & CANFD_CR1_BID_Msk     ) >> CANFD_CR1_BID_Pos     ) << 18);
	
	if((msg->format & 1) == 0)  msg->id &= 0x7FF;
	
	msg->remote =  (cr0 & CANFD_CR0_RTR_Msk  ? 1 : 0);
	msg->passive = (cr0 & CANFD_CR0_RESI_Msk ? 1 : 0);
	
	msg->size = (cr1 & CANFD_CR1_DLC_Msk) >> CANFD_CR1_DLC_Pos;
	if((msg->format & 2) && (msg->size > 8))
	{
		switch(msg->size)
		{
		case 9:  msg->size = 12;  break;
		case 10: msg->size = 16;  break;
		case 11: msg->size = 20;  break;
		case 12: msg->size = 24;  break;
		case 13: msg->size = 32;  break;
		case 14: msg->size = 48;  break;
		case 15: msg->size = 64;  break;
		}
	}
	
	memcpy(msg->buff, (void *)CANx->RXBUF[rxbuf], msg->size);
	
	CANx->CR |= (1 << (CANFD_CR_RFF0POP_Pos - rxbuf));
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_TXAbort()
* 功能说明:	终止发送
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t txbuf		指定要终止哪个TX Buffer，有效值包括CANFD_TXBUF_0、CANFD_TXBUF_1、CANFD_TXBUF_2
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CANFD_TXAbort(CANFD_TypeDef * CANx, uint32_t txbuf)
{
	CANx->SR |= (1 << (CANFD_SR_TBF0ARORT_Pos - txbuf));
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_TXStatus()
* 功能说明:	发送状态查询
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t txbuf		指定要查询哪个TX Buffer，有效值包括CANFD_TXBUF_0、CANFD_TXBUF_1、CANFD_TXBUF_2
* 输    出: uint32_t			CANFD_TXST_BUSY、CANFD_TXST_ARBL、CANFD_TXST_SUCC、CANFD_TXST_ERR
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t CANFD_TXStatus(CANFD_TypeDef * CANx, uint32_t txbuf)
{
	if(CANx->CR & (1 << (CANFD_CR_TBF0REQ_Pos - txbuf)))
		return CANFD_TXST_BUSY;
	
	if(CANx->SR & (1 << (CANFD_SR_TBF0ARBLOST_Pos - txbuf)))
		return CANFD_TXST_ARBL;
	
	if(CANx->IF & (1 << (CANFD_IF_TBF0OK_Pos - txbuf)))
		return CANFD_TXST_SUCC;
	
	if(CANx->ERRSR & (1 << (CANFD_ERRSR_TXBF0_Pos - txbuf)))
		return CANFD_TXST_ERR;
	
	return CANFD_TXST_ERR;
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_RXDataAvailable()
* 功能说明:	RX FIFO中是否有数据可读出
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t rxbuf		指定要查询的RX Buffer，有效值包括CANFD_RXBUF_0、CANFD_RXBUF_1
* 输    出: uint32_t			1 有数据可读出    0 没有数据
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t CANFD_RXDataAvailable(CANFD_TypeDef * CANx, uint32_t rxbuf)
{
	return (CANx->SR & (3 << (CANFD_SR_RFF0LVL_Pos - rxbuf * 2))) ? 1 : 0;
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_SetBaudrate()
* 功能说明:	设置波特率
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t CANFD_data 0 baudrate for non-FD frame or FD frame non-Data phase   1 baudrate for FD frame Data phase
*			uint32_t baudrate	波特率，即位传输速率
*			uint32_t CAN_bs1	CANFD_data = 0 时：取值 2--96   CANFD_data = 1 时：取值 1--16
*			uint32_t CAN_bs2	CANFD_data = 0 时：取值 1--32	CANFD_data = 1 时：取值 1--8
*			uint32_t CAN_sjw	CANFD_data = 0 时：取值 1--32	CANFD_data = 1 时：取值 1--4
* 输    出: 无
* 注意事项: 设置前需要先调用CANFD_Close()关闭CAN模块
******************************************************************************************************************************************/
void CANFD_SetBaudrate(CANFD_TypeDef * CANx, uint32_t CANFD_data, uint32_t baudrate, uint32_t CAN_bs1, uint32_t CAN_bs2, uint32_t CAN_sjw)
{
	int p_prop, p_seg1;
	int brp = (SystemCoreClock/2) / baudrate / (1 + CAN_bs1 + CAN_bs2) - 1;
	
	if(CANFD_data == 0)
	{
		if(CAN_bs1 > 32)
		{
			p_prop = CAN_bs1 - 32;
			p_seg1 = 32;
		}
		else
		{
			p_prop = 1;
			p_seg1 = CAN_bs1 - 1;
		}
		
		CANx->NBT0 = (brp           << CANFD_NBT0_BRP_Pos)  |
					 ((p_prop  - 1) << CANFD_NBT0_PROP_Pos) |
					 ((p_seg1  - 1) << CANFD_NBT0_SEG1_Pos) |
					 ((CAN_sjw - 1) << CANFD_NBT0_SJW_Pos);
		
		CANx->NBT1 = ((CAN_bs2 - 1) << CANFD_NBT1_SEG2_Pos);
	}
	else
	{
		if(CAN_bs1 > 8)
		{
			p_prop = CAN_bs1 - 8;
			p_seg1 = 8;
		}
		else
		{
			p_prop = 0;
			p_seg1 = CAN_bs1;
		}
		
		CANx->DBT  = (brp           << CANFD_DBT_BRP_Pos)  |
					 (p_prop        << CANFD_DBT_PROP_Pos) |
					 ((p_seg1  - 1) << CANFD_DBT_SEG1_Pos) |
					 ((CAN_bs2 - 1) << CANFD_DBT_SEG2_Pos) |
					 ((CAN_sjw - 1) << CANFD_DBT_SJW_Pos);
	}
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_SetIDFilter()
* 功能说明:	设置接收 ID 过滤器
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t rxbuf		指定要设置的RX Buffer，有效值包括CANFD_RXBUF_0、CANFD_RXBUF_1
*			CANFD_IDFilter * idf	ID 过滤器设置
* 输    出: 无
* 注意事项: 设置前需要先调用CANFD_Close()关闭CAN模块
******************************************************************************************************************************************/
void CANFD_SetIDFilter(CANFD_TypeDef * CANx, uint32_t rxbuf, CANFD_IDFilter * idf)
{
	CANx->AFCR &= ~(CANFD_AFCR_FD_Msk | CANFD_AFCR_IDE_Msk | CANFD_AFCR_RTR_Msk);
	CANx->AFCR |= (idf->canfd  << CANFD_AFCR_FD_Pos)  |
				  (idf->extid  << CANFD_AFCR_IDE_Pos) |
				  (idf->remote << CANFD_AFCR_RTR_Pos);
	
	CANx->AFMID[rxbuf] = idf->mask << CANFD_AFMID_EID_Pos;
	
	if(rxbuf == 0)
	{
		for(int i = 0; i < 4; i++)
			CANx->AFID[i] = idf->id[i] << CANFD_AFID_EID_Pos;
		
		CANx->AFCR  |= CANFD_AFCR_RBF0ID_Msk;
		CANx->AFMCR |= CANFD_AFMCR_RBF0ID_Msk;
	}
	else
	{
		for(int i = 0; i < 2; i++)
			CANx->AFID[4+i] = idf->id[i] << CANFD_AFID_EID_Pos;
		
		CANx->AFCR  |= CANFD_AFCR_RBF1ID_Msk;
		CANx->AFMCR |= CANFD_AFMCR_RBF1ID_Msk;
	}
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_SetDataFilter()
* 功能说明:	设置接收 Data 过滤器
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t rxbuf		指定要设置的RX Buffer，有效值包括CANFD_RXBUF_0、CANFD_RXBUF_1
*			uint32_t index		指定要过滤的数据，0 第一个字节   1 第二个字节
*			uint8_t byte
*			uint8_t mask
* 输    出: 无
* 注意事项: 设置前需要先调用CANFD_Close()关闭CAN模块
******************************************************************************************************************************************/
void CANFD_SetDataFilter(CANFD_TypeDef * CANx, uint32_t rxbuf, uint32_t index, uint8_t byte, uint8_t mask)
{
	int offset = rxbuf * 16 + index * 8;
	
	CANx->AFDB  &= ~(0xFF << offset);
	CANx->AFDB  |=  (byte << offset);
	CANx->AFMDB &= ~(0xFF << offset);
	CANx->AFMDB |=  (mask << offset);
	
	offset = (1 - rxbuf) * 2 + (1 - index);
	
	CANx->AFCR  |= (1 << (2 + offset));
	CANx->AFMCR |= (1 << (1 + offset));
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_INTEn()
* 功能说明:	使能指定中断
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t it			interrupt type，有效值包括 CANFD_IT_RXOK、CANFD_IT_TXOK、CANFD_IT_ERR、CANFD_IT_RXOV、CANFD_IT_WKUP 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CANFD_INTEn(CANFD_TypeDef * CANx, uint32_t it)
{
	CANx->CR |= (it << 8);
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_INTDis()
* 功能说明:	关闭指定中断
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t it			interrupt type，有效值包括 CANFD_IT_RXOK、CANFD_IT_TXOK、CANFD_IT_ERR、CANFD_IT_RXOV、CANFD_IT_WKUP 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CANFD_INTDis(CANFD_TypeDef * CANx, uint32_t it)
{
	CANx->CR &= ~(it << 8);
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_INTClr()
* 功能说明:	清除中断标志
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t it			interrupt type，有效值包括 CANFD_IT_RX0OK、CANFD_IT_RX1OK、CANFD_IT_TX0OK、CANFD_IT_TX1OK、CANFD_IT_TX2OK、
*									CANFD_IT_ERR、CANFD_IT_RXOV、CANFD_IT_WKUP 及其“或”
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void CANFD_INTClr(CANFD_TypeDef * CANx, uint32_t it)
{
	CANx->IF = it;
}


/****************************************************************************************************************************************** 
* 函数名称:	CANFD_INTStat()
* 功能说明:	查询中断状态
* 输    入: CANFD_TypeDef * CANx	指定要被设置的CAN接口，有效值包括CAN0
*			uint32_t it			interrupt type，有效值包括 CANFD_IT_RX0OK、CANFD_IT_RX1OK、CANFD_IT_TX0OK、CANFD_IT_TX1OK、CANFD_IT_TX2OK、
*									CANFD_IT_ERR、CANFD_IT_RXOV、CANFD_IT_WKUP 及其“或”
* 输    出: uint32_t			1 指定中断已发生    0 指定中断未发生
* 注意事项: 无
******************************************************************************************************************************************/
uint32_t CANFD_INTStat(CANFD_TypeDef * CANx, uint32_t it)
{
	return (CANx->IF & it);
}

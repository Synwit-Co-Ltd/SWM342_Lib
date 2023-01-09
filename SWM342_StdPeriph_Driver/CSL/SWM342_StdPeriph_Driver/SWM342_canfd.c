/****************************************************************************************************************************************** 
* �ļ�����: SWM342_canfd.c
* ����˵��:	SWM342��Ƭ����CANFDģ��������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������: 
* �汾����:	V1.1.0		2017��10��25��
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
#include <string.h>
#include "SWM342.h"
#include "SWM342_canfd.h"


static int CANFD_Mode = CANFD_MODE_CONFIG;


/****************************************************************************************************************************************** 
* ��������:	CANFD_Init()
* ����˵��:	CAN�ӿڳ�ʼ��
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			CANFD_InitStructure * initStruct    ����CAN�ӿ�����趨ֵ�Ľṹ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void CANFD_Init(CANFD_TypeDef * CANx, CANFD_InitStructure * initStruct)
{	
	switch((uint32_t)CANx)
	{
	case ((uint32_t)CAN0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_CAN0_Pos);
		break;
	}
	
	CANFD_Close(CANx);	//һЩ�ؼ��Ĵ���ֻ����CAN�ر�ʱ����
	
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
* ��������:	CANFD_Open()
* ����˵��:	CAN�ӿڴ�
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void CANFD_Open(CANFD_TypeDef * CANx)
{
	CANx->CR = CANx->CR & (~CANFD_CR_MODE_Msk) | (CANFD_Mode << CANFD_CR_MODE_Pos);
}


/****************************************************************************************************************************************** 
* ��������:	CANFD_Close()
* ����˵��:	CAN�ӿڹر�
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void CANFD_Close(CANFD_TypeDef * CANx)
{
	CANx->CR = CANx->CR & (~CANFD_CR_MODE_Msk) | (CANFD_MODE_CONFIG << CANFD_CR_MODE_Pos);
}


/****************************************************************************************************************************************** 
* ��������:	CANFD_Transmit()
* ����˵��:	CAN��������
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t txbuf		ָ��Ҫʹ���ĸ�TX Buffer����Чֵ����CANFD_TXBUF_0��CANFD_TXBUF_1��CANFD_TXBUF_2
*			CANFD_Message * msg	Ҫ���͵���Ϣ�洢�ڴ˽ṹ�������
* ��    ��: ��
* ע������: ��
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
* ��������:	CANFD_Receive()
* ����˵��:	CAN��������
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t rxbuf		ָ��Ҫʹ���ĸ���RX Buffer����Чֵ����CANFD_RXBUF_0��CANFD_RXBUF_1
*			CANFD_Message * msg	���յ�����Ϣ�洢�ڴ˽ṹ�������
* ��    ��: ��
* ע������: ��
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
* ��������:	CANFD_TXAbort()
* ����˵��:	��ֹ����
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t txbuf		ָ��Ҫ��ֹ�ĸ�TX Buffer����Чֵ����CANFD_TXBUF_0��CANFD_TXBUF_1��CANFD_TXBUF_2
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void CANFD_TXAbort(CANFD_TypeDef * CANx, uint32_t txbuf)
{
	CANx->SR |= (1 << (CANFD_SR_TBF0ARORT_Pos - txbuf));
}


/****************************************************************************************************************************************** 
* ��������:	CANFD_TXStatus()
* ����˵��:	����״̬��ѯ
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t txbuf		ָ��Ҫ��ѯ�ĸ�TX Buffer����Чֵ����CANFD_TXBUF_0��CANFD_TXBUF_1��CANFD_TXBUF_2
* ��    ��: uint32_t			CANFD_TXST_BUSY��CANFD_TXST_ARBL��CANFD_TXST_SUCC��CANFD_TXST_ERR
* ע������: ��
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
* ��������:	CANFD_RXDataAvailable()
* ����˵��:	RX FIFO���Ƿ������ݿɶ���
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t rxbuf		ָ��Ҫ��ѯ��RX Buffer����Чֵ����CANFD_RXBUF_0��CANFD_RXBUF_1
* ��    ��: uint32_t			1 �����ݿɶ���    0 û������
* ע������: ��
******************************************************************************************************************************************/
uint32_t CANFD_RXDataAvailable(CANFD_TypeDef * CANx, uint32_t rxbuf)
{
	return (CANx->SR & (3 << (CANFD_SR_RFF0LVL_Pos - rxbuf * 2))) ? 1 : 0;
}


/****************************************************************************************************************************************** 
* ��������:	CANFD_SetBaudrate()
* ����˵��:	���ò�����
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t CANFD_data 0 baudrate for non-FD frame or FD frame non-Data phase   1 baudrate for FD frame Data phase
*			uint32_t baudrate	�����ʣ���λ��������
*			uint32_t CAN_bs1	CANFD_data = 0 ʱ��ȡֵ 2--96   CANFD_data = 1 ʱ��ȡֵ 1--16
*			uint32_t CAN_bs2	CANFD_data = 0 ʱ��ȡֵ 1--32	CANFD_data = 1 ʱ��ȡֵ 1--8
*			uint32_t CAN_sjw	CANFD_data = 0 ʱ��ȡֵ 1--32	CANFD_data = 1 ʱ��ȡֵ 1--4
* ��    ��: ��
* ע������: ����ǰ��Ҫ�ȵ���CANFD_Close()�ر�CANģ��
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
* ��������:	CANFD_SetIDFilter()
* ����˵��:	���ý��� ID ������
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t rxbuf		ָ��Ҫ���õ�RX Buffer����Чֵ����CANFD_RXBUF_0��CANFD_RXBUF_1
*			CANFD_IDFilter * idf	ID ����������
* ��    ��: ��
* ע������: ����ǰ��Ҫ�ȵ���CANFD_Close()�ر�CANģ��
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
* ��������:	CANFD_SetDataFilter()
* ����˵��:	���ý��� Data ������
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t rxbuf		ָ��Ҫ���õ�RX Buffer����Чֵ����CANFD_RXBUF_0��CANFD_RXBUF_1
*			uint32_t index		ָ��Ҫ���˵����ݣ�0 ��һ���ֽ�   1 �ڶ����ֽ�
*			uint8_t byte
*			uint8_t mask
* ��    ��: ��
* ע������: ����ǰ��Ҫ�ȵ���CANFD_Close()�ر�CANģ��
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
* ��������:	CANFD_INTEn()
* ����˵��:	ʹ��ָ���ж�
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t it			interrupt type����Чֵ���� CANFD_IT_RXOK��CANFD_IT_TXOK��CANFD_IT_ERR��CANFD_IT_RXOV��CANFD_IT_WKUP ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void CANFD_INTEn(CANFD_TypeDef * CANx, uint32_t it)
{
	CANx->CR |= (it << 8);
}


/****************************************************************************************************************************************** 
* ��������:	CANFD_INTDis()
* ����˵��:	�ر�ָ���ж�
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t it			interrupt type����Чֵ���� CANFD_IT_RXOK��CANFD_IT_TXOK��CANFD_IT_ERR��CANFD_IT_RXOV��CANFD_IT_WKUP ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void CANFD_INTDis(CANFD_TypeDef * CANx, uint32_t it)
{
	CANx->CR &= ~(it << 8);
}


/****************************************************************************************************************************************** 
* ��������:	CANFD_INTClr()
* ����˵��:	����жϱ�־
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t it			interrupt type����Чֵ���� CANFD_IT_RX0OK��CANFD_IT_RX1OK��CANFD_IT_TX0OK��CANFD_IT_TX1OK��CANFD_IT_TX2OK��
*									CANFD_IT_ERR��CANFD_IT_RXOV��CANFD_IT_WKUP ���䡰��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void CANFD_INTClr(CANFD_TypeDef * CANx, uint32_t it)
{
	CANx->IF = it;
}


/****************************************************************************************************************************************** 
* ��������:	CANFD_INTStat()
* ����˵��:	��ѯ�ж�״̬
* ��    ��: CANFD_TypeDef * CANx	ָ��Ҫ�����õ�CAN�ӿڣ���Чֵ����CAN0
*			uint32_t it			interrupt type����Чֵ���� CANFD_IT_RX0OK��CANFD_IT_RX1OK��CANFD_IT_TX0OK��CANFD_IT_TX1OK��CANFD_IT_TX2OK��
*									CANFD_IT_ERR��CANFD_IT_RXOV��CANFD_IT_WKUP ���䡰��
* ��    ��: uint32_t			1 ָ���ж��ѷ���    0 ָ���ж�δ����
* ע������: ��
******************************************************************************************************************************************/
uint32_t CANFD_INTStat(CANFD_TypeDef * CANx, uint32_t it)
{
	return (CANx->IF & it);
}

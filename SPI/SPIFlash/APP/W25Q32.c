/****************************************************************************************************************************************** 
* �ļ�����:	W25Q32.c
* ����˵��:	W25Q32 SPI Flash ��������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����: V1.0.0		2017��10��26��
* ������¼: 
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
#include "W25Q32.h"


/****************************************************************************************************************************************** 
* ��������:	W25Q32_Init()
* ����˵��:	��ʼ��W25Q32ʹ�õ�SPI�ӿڣ�����ȡоƬID���ж����Ƿ���W25Q32оƬ
* ��    ��: ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void W25Q32_Init(void)
{
	uint16_t chip_id;
	uint8_t manufacture_id, device_id;
	SPI_InitStructure SPI_initStruct;
	
	GPIO_Init(GPIOD, PIN6, 1, 0, 0, 0);
#define W25Q32_Assert()		GPIO_ClrBit(GPIOD, PIN6)
#define W25Q32_Deassert()	GPIO_SetBit(GPIOD, PIN6)
	W25Q32_Deassert();
	
	PORT_Init(PORTD, PIN5, PORTD_PIN5_SPI0_SCLK, 0);
	PORT_Init(PORTD, PIN8, PORTD_PIN8_SPI0_MOSI, 1);
	PORT_Init(PORTD, PIN7, PORTD_PIN7_SPI0_MISO, 1);
	PORT_Init(PORTD, PIN3, PORTD_PIN3_SPI0_DAT2, 1);
	PORT_Init(PORTD, PIN4, PORTD_PIN4_SPI0_DAT3, 1);
	
	SPI_initStruct.clkDiv = SPI_CLKDIV_8;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_FIRST_EDGE;
	SPI_initStruct.IdleLevel = SPI_LOW_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 1;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn  = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	
	SPI_Open(SPI0);
	
	chip_id = W25Q32_ReadChipID();
	manufacture_id = chip_id >> 8;
	device_id = chip_id & 0xFF;
	
	if((manufacture_id == W25Q32_MANUFACTOR_ID) && (device_id == W25Q32_DEVICE_ID))
	{
		printf("W25Q32 found\r\n");
		
		W25Q32_QuadSwitch(1);
		while(W25Q32_IsBusy()) __NOP();	// �ȴ�д��������
		
		W25Q32_ProtectSet(0);
		while(W25Q32_IsBusy()) __NOP();	// �ȴ�д��������
	}
	else
	{
		printf("Manufactor ID: %02X, Device ID: %02X, fail to identify\r\n", manufacture_id, device_id);
	}
}

/****************************************************************************************************************************************** 
* ��������:	W25Q32_ReadChipID()
* ����˵��:	��ȡоƬID
* ��    ��: ��
* ��    ��: uint16_t			оƬID����16λ����8λΪ����ID����8λΪDevice ID
* ע������: ��
******************************************************************************************************************************************/
uint16_t W25Q32_ReadChipID(void)
{
	uint8_t manufacture_id, device_id;
	
	W25Q32_Assert();
	
	SPI_ReadWrite(SPI0, W25Q32_CMD_READ_CHIP_ID);
	SPI_ReadWrite(SPI0, 0x00);
	SPI_ReadWrite(SPI0, 0x00);
	SPI_ReadWrite(SPI0, 0x00);
	
	manufacture_id = SPI_ReadWrite(SPI0, 0xFF);
	device_id      = SPI_ReadWrite(SPI0, 0xFF);
	
	W25Q32_Deassert();
	
	return (manufacture_id << 8) | device_id;
}

/****************************************************************************************************************************************** 
* ��������:	W25Q32_ReadStatus()
* ����˵��:	��ȡоƬ״̬�Ĵ���
* ��    ��: ��
* ��    ��: uint16_t			оƬ״̬
* ע������: ��
******************************************************************************************************************************************/
uint16_t W25Q32_ReadStatus(void)
{
	uint16_t status;
	
	W25Q32_Assert();
	
	SPI_ReadWrite(SPI0, W25Q32_CMD_READ_STATUS_L);
	status = SPI_ReadWrite(SPI0, 0xFF);
	
	W25Q32_Deassert();
	
	W25Q32_Assert();
	
	SPI_ReadWrite(SPI0, W25Q32_CMD_READ_STATUS_H);
	status |= SPI_ReadWrite(SPI0, 0xFF) << 8;
	
	W25Q32_Deassert();
	
	return status;
}

/****************************************************************************************************************************************** 
* ��������:	W25Q32_IsBusy()
* ����˵��:	оƬæ��⣬ִ�в�����д���������Ҫִ��æ������ж�оƬ�Ƿ��Ѿ���ɲ���
* ��    ��: ��
* ��    ��: uint32_t			1 оƬ����ִ���ڲ�������д�����    0 оƬδ��ִ���ڲ�����
* ע������: ��
******************************************************************************************************************************************/
uint32_t W25Q32_IsBusy(void)
{
	return (W25Q32_ReadStatus() & W25Q32_STATUS_WIP_Msk) ? 1 : 0;
}

/****************************************************************************************************************************************** 
* ��������:	W25Q32_QuadSwitch()
* ����˵��:	4�߶�����
* ��    ��: uint32_t			1 ����4�߶�д   0 �ر�4�߶�д
* ��    ��: uint16_t			оƬ״̬
* ע������: ��
******************************************************************************************************************************************/
void W25Q32_QuadSwitch(uint32_t on)
{
	uint16_t status = W25Q32_ReadStatus();
	
	if(on)
		status |=  (1 << W25Q32_STATUS_QE_Pos);
	else
		status &= ~(1 << W25Q32_STATUS_QE_Pos);
	
	W25Q32_Assert();
	
	SPI_ReadWrite(SPI0, W25Q32_CMD_WRITE_ENABLE);
	
	W25Q32_Deassert();
	
	W25Q32_Assert();
	
	SPI_ReadWrite(SPI0, W25Q32_CMD_WRITE_STATUS);
	SPI_ReadWrite(SPI0, (status >>  0) & 0xFF);
	SPI_ReadWrite(SPI0, (status >>  8) & 0xFF);
	
	W25Q32_Deassert();
}

/****************************************************************************************************************************************** 
* ��������:	W25Q32_ProtectSet()
* ����˵��:	���д��������
* ��    ��: uint32_t			��5λ����3λΪ������Χ�����λ�Ǳ�����λ����/���������м�λ�ǰ���λ�ã�TOP/BOTTOM��������ֵ0�ر����д����
* ��    ��: uint16_t			оƬ״̬
* ע������: ��
******************************************************************************************************************************************/
void W25Q32_ProtectSet(uint32_t set)
{
	uint16_t status = W25Q32_ReadStatus();
	
	status &= ~(W25Q32_STATUS_SBP_Msk | W25Q32_STATUS_SRP_Msk | W25Q32_STATIS_CMP_Msk);
	status |= (set << W25Q32_STATUS_SBP_Pos);
	
	W25Q32_Assert();
	
	SPI_ReadWrite(SPI0, W25Q32_CMD_WRITE_ENABLE);
	
	W25Q32_Deassert();
	
	W25Q32_Assert();
	
	SPI_ReadWrite(SPI0, W25Q32_CMD_WRITE_STATUS);
	SPI_ReadWrite(SPI0, (status >>  0) & 0xFF);
	SPI_ReadWrite(SPI0, (status >>  8) & 0xFF);
	
	W25Q32_Deassert();
}

/****************************************************************************************************************************************** 
* ��������:	W25Q32_EraseSector()
* ����˵��:	��������
* ��    ��: uint32_t addr		Ҫ�����������ĵ�ַ��ÿ����4K�ֽ�
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void W25Q32_EraseSector(uint32_t addr)
{
	W25Q32_Assert();
	
	SPI_ReadWrite(SPI0, W25Q32_CMD_WRITE_ENABLE);
	
	W25Q32_Deassert();
	
	W25Q32_Assert();
		
	SPI_ReadWrite(SPI0, W25Q32_CMD_ERASE_SECTOR);
	SPI_ReadWrite(SPI0, (addr >> 16) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  8) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  0) & 0xFF);
	
	W25Q32_Deassert();
}

/****************************************************************************************************************************************** 
* ��������:	W25Q32_ReadData()
* ����˵��:	��W25Q32ָ����ַ��ȡָ�����ֽ�����
* ��    ��: uint32_t addr		Ҫ��ȡ��������W25Q32�ϵĵ�ַ
*			uint8_t buff[]		��ȡ�������ݴ洢��buff��
*			uint32_t cnt		Ҫ��ȡ���ݵ��ֽڸ���
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void W25Q32_ReadData(uint32_t addr, uint8_t buff[], uint32_t cnt)
{
	uint32_t i;
	
	W25Q32_Assert();
	
	SPI_ReadWrite(SPI0, W25Q32_CMD_READ_DATA);
	SPI_ReadWrite(SPI0, (addr >> 16) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  8) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  0) & 0xFF);
	
	for(i = 0; i < cnt; i++)
	{
		buff[i] = SPI_ReadWrite(SPI0, 0xFF);
	}
	
	W25Q32_Deassert();
}

/****************************************************************************************************************************************** 
* ��������:	W25Q32_ReadData_4bit()
* ����˵��:	��W25Q32ָ����ַ��ȡָ�����ֽ����ݣ���ȡʱʹ��4�߶�ȡ
* ��    ��: uint32_t addr		Ҫ��ȡ��������W25Q32�ϵĵ�ַ
*			uint8_t buff[]		��ȡ�������ݴ洢��buff��
*			uint32_t cnt		Ҫ��ȡ���ݵ��ֽڸ���
* ��    ��: ��
* ע������: ÿ������256���ֽ�
******************************************************************************************************************************************/
void W25Q32_ReadData_4bit(uint32_t addr, uint8_t buff[], uint32_t cnt)
{
	uint32_t i;
	
	SPI_Close(SPI0);
	SPI0->CTRL &= ~SPI_CTRL_FFS_Msk;
	SPI0->CTRL |= (3 << SPI_CTRL_FFS_Pos);
	SPI0->SPIMCR = (1 << SPI_SPIMCR_RDEN_Pos) |
				   ((cnt - 1) << SPI_SPIMCR_DLEN_Pos) |
				   ((8 - 1)   << SPI_SPIMCR_DUMMY_Pos);
	SPI_Open(SPI0);
	
	W25Q32_Assert();
	
	SPI_Write(SPI0, (W25Q32_CMD_READ_DATA_4bit << 24) | (addr & 0xFFFFFF));
	while(SPI_IsRXEmpty(SPI0)) __NOP();
	buff[0] = SPI_Read(SPI0);
	
	for(i = 0; i < cnt; i++)
	{
		while(SPI_IsRXEmpty(SPI0)) __NOP();
		
		buff[i] = SPI_Read(SPI0);
	}
	
	W25Q32_Deassert();
	
	SPI_Close(SPI0);
	SPI0->SPIMCR = 0;	// ���������SPIMCR.EN���ٸı�CTRL.FFS
	SPI0->CTRL &= ~SPI_CTRL_FFS_Msk;
	SPI0->CTRL |= (0 << SPI_CTRL_FFS_Pos);
	SPI_Open(SPI0);
}

/****************************************************************************************************************************************** 
* ��������:	W25Q32_ReadData_4bit_DMA()
* ����˵��: ͬ W25Q32_ReadData_4bit����ʹ�� DMA ��ȡSPI���ݼĴ������ɱ��������ȡ����ʱ���µ����ݶ�ʧ
* ��    ��: ͬ W25Q32_ReadData_4bit
* ��    ��: ��
* ע������: ÿ������256���ֽ�
*			buff�Ĵ�С������cnt+1����buff[0]������Ч���ݣ���Ч������buff[1..cnt]
******************************************************************************************************************************************/
void W25Q32_ReadData_4bit_DMA(uint32_t addr, uint8_t buff[], uint32_t cnt)
{
	DMA_InitStructure DMA_initStruct;
	
	// SPI0 RX DMA
    DMA_initStruct.Mode = DMA_MODE_SINGLE;
    DMA_initStruct.Unit = DMA_UNIT_BYTE;
    DMA_initStruct.Count = cnt;
    DMA_initStruct.SrcAddr = (uint32_t)&SPI0->DATA;
    DMA_initStruct.SrcAddrInc = 0;
    DMA_initStruct.DstAddr = (uint32_t)buff;
    DMA_initStruct.DstAddrInc = 1;
    DMA_initStruct.Handshake = DMA_CH2_SPI0RX;
    DMA_initStruct.Priority = DMA_PRI_LOW;
    DMA_initStruct.INTEn = 0;
    DMA_CH_Init(DMA_CH2, &DMA_initStruct);
    DMA_CH_Open(DMA_CH2);
	
	
	SPI_Close(SPI0);
	SPI0->CTRL &= ~SPI_CTRL_FFS_Msk;
	SPI0->CTRL |= (3 << SPI_CTRL_FFS_Pos);
	SPI0->SPIMCR = (1 << SPI_SPIMCR_RDEN_Pos) |
				   ((cnt - 1) << SPI_SPIMCR_DLEN_Pos) |
				   ((8 - 1)   << SPI_SPIMCR_DUMMY_Pos);
	SPI_Open(SPI0);
	
	W25Q32_Assert();
	
	__disable_irq();
	
	SPI_Write(SPI0, (W25Q32_CMD_READ_DATA_4bit << 24) | (addr & 0xFFFFFF));
	while(SPI_IsRXEmpty(SPI0)) __NOP();
	buff[0] = SPI_Read(SPI0);
	
	SPI0->CTRL |= (1 << SPI_CTRL_DMARXEN_Pos);
	
	__enable_irq();
	
	while(DMA_CH_INTStat(DMA_CH2, DMA_IT_DONE) == 0) __NOP();
	DMA_CH_INTClr(DMA_CH2, DMA_IT_DONE);
	
	W25Q32_Deassert();
	
	SPI0->CTRL &= ~(1 << SPI_CTRL_DMARXEN_Pos);
	
	SPI_Close(SPI0);
	SPI0->SPIMCR = 0;
	SPI0->CTRL &= ~SPI_CTRL_FFS_Msk;
	SPI0->CTRL |= (0 << SPI_CTRL_FFS_Pos);
	SPI_Open(SPI0);
}

/****************************************************************************************************************************************** 
* ��������:	W25Q32_WritePage()
* ����˵��:	��W25Q32ָ����ַ��д��ָ�����ֽ�����
* ��    ��: uint32_t addr		����Ҫд�뵽W25Q32��λ��
*			uint8_t data[]		Ҫд�������
*			uint16_t cnt		Ҫд������ݸ�����ÿҳ256�ֽڣ�ÿ��д������ݱ�����ͬһҳ�ĵ�ַ��Χ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void W25Q32_WritePage(uint32_t addr, uint8_t data[], uint16_t cnt)
{
	uint32_t i;
	
	W25Q32_Assert();
	
	SPI_ReadWrite(SPI0, W25Q32_CMD_WRITE_ENABLE);
	
	W25Q32_Deassert();
	
	W25Q32_Assert();
	
	SPI_ReadWrite(SPI0, W25Q32_CMD_WRITE_PAGE);
	SPI_ReadWrite(SPI0, (addr >> 16) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  8) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  0) & 0xFF);
	
	for(i = 0; i < cnt; i++)
	{
		SPI_ReadWrite(SPI0, data[i]);
	}
	
	W25Q32_Deassert();
}

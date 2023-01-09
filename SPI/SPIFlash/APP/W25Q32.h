#ifndef __W25Q32_H__
#define __W25Q32_H__


void W25Q32_Init(void);
uint16_t W25Q32_ReadChipID(void);
uint16_t W25Q32_ReadStatus(void);

uint32_t W25Q32_IsBusy(void);

void W25Q32_EraseSector(uint32_t addr);
void W25Q32_ReadData(uint32_t addr, uint8_t buff[], uint32_t cnt);
void W25Q32_WritePage(uint32_t addr, uint8_t data[], uint16_t cnt);

void W25Q32_QuadSwitch(uint32_t on);
void W25Q32_ProtectSet(uint32_t set);
void W25Q32_ReadData_4bit(uint32_t addr, uint8_t buff[], uint32_t cnt);
void W25Q32_ReadData_4bit_DMA(uint32_t addr, uint8_t buff[], uint32_t cnt);


#define W25Q32_STATUS_WIP_Pos		0		// Write In Progress
#define W25Q32_STATUS_WIP_Msk		(0x01 << W25Q32_STATUS_WIP_Pos)
#define W25Q32_STATUS_WEL_Pos		1		// Write Enable Latch
#define W25Q32_STATUS_WEL_Msk		(0x01 << W25Q32_STATUS_WEL_Pos)
#define W25Q32_STATUS_SBP_Pos		2		// Software Block Protect
#define W25Q32_STATUS_SBP_Msk		(0x1F << W25Q32_STATUS_SBP_Pos)
#define W25Q32_STATUS_SRP_Pos		7		// Status Register Protect
#define W25Q32_STATUS_SRP_Msk		(0x03 << W25Q32_STATUS_SRP_Pos)
#define W25Q32_STATUS_QE_Pos		9		// Quad Enable
#define W25Q32_STATUS_QE_Msk		(0x01 << W25Q32_STATUS_QE_Pos)
#define W25Q32_STATUS_CMP_Pos		14		// for W25Q32BV: COMPLEMENT PROTECT
#define W25Q32_STATIS_CMP_Msk		(0x01 << W25Q32_STATUS_CMP_Pos)
#define W25Q32_STATUS_SUS_Pos		15		// for W25Q32BV: SUSPEND STATUS
#define W25Q32_STATIS_SUS_Msk		(0x01 << W25Q32_STATUS_SUS_Pos)


#define W25Q32_MANUFACTOR_ID		0xEF
#define W25Q32_DEVICE_ID			0x15


#define W25Q32_CMD_READ_CHIP_ID		0x90
#define W25Q32_CMD_READ_STATUS_L	0x05
#define W25Q32_CMD_READ_STATUS_H	0x35
#define W25Q32_CMD_WRITE_STATUS		0x01
#define W25Q32_CMD_READ_DATA		0x03
#define W25Q32_CMD_READ_DATA_4bit	0x6B
#define W25Q32_CMD_WRITE_PAGE		0x02
#define W25Q32_CMD_ERASE_SECTOR		0x20
#define W25Q32_CMD_WRITE_ENABLE		0x06
#define W25Q32_CMD_WRITE_DISABLE	0x04



#endif //__W25Q32_H__

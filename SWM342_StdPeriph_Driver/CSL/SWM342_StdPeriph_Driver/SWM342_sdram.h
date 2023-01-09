#ifndef __SWM342_SDRAM_H__
#define __SWM342_SDRAM_H__

typedef struct {
	uint8_t ClkDiv;				// SDRAM 时钟分频，SDRAM_CLKDIV_1
	uint8_t NbrBank;			// Bank 个数
	uint8_t NbrRowAddr;			// Row Address Line 个数
	uint8_t NbrColAddr;			// Column Address Line 个数
	uint8_t CASLatency;			// 列地址到有效数据输出间隔，SDRAM_CASLATENCY_2、SDRAM_CASLATENCY_3
	
	uint8_t TimeTRC;			// Active to Active (same bank) delay
	uint8_t TimeTRRD;			// Active to Active (diff bank) delay
	uint8_t TimeTRCD;			// Active to Read/Write delay
	uint8_t TimeTRAS;			// Active to Precharge delay
	uint8_t TimeTRP;			// Precharge to Active delay
} SDRAM_InitStructure;


#define SDRAM_CLKDIV_1		0

#define SDRAM_BANK_1		0
#define SDRAM_BANK_2		1
#define SDRAM_BANK_4		2

#define SDRAM_ROW_11		11
#define SDRAM_ROW_12		12
#define SDRAM_ROW_13		13

#define SDRAM_COLUMN_8		0
#define SDRAM_COLUMN_9		1
#define SDRAM_COLUMN_10		2
#define SDRAM_COLUMN_11		3

#define SDRAM_CASLATENCY_2	2
#define SDRAM_CASLATENCY_3	3

#define SDRAM_TRC_1			0
#define SDRAM_TRC_2			1
#define SDRAM_TRC_3			2
#define SDRAM_TRC_4			3
#define SDRAM_TRC_5			4
#define SDRAM_TRC_6			5
#define SDRAM_TRC_7			6
#define SDRAM_TRC_8			7
#define SDRAM_TRC_9			8
#define SDRAM_TRC_10		9
#define SDRAM_TRC_11		10
#define SDRAM_TRC_12		11
#define SDRAM_TRC_13		12
#define SDRAM_TRC_14		13
#define SDRAM_TRC_15		14
#define SDRAM_TRC_16		15

#define SDRAM_TRRD_1		0
#define SDRAM_TRRD_2		1
#define SDRAM_TRRD_3		2
#define SDRAM_TRRD_4		3
#define SDRAM_TRRD_5		4
#define SDRAM_TRRD_6		5
#define SDRAM_TRRD_7		6
#define SDRAM_TRRD_8		7

#define SDRAM_TRCD_1		0
#define SDRAM_TRCD_2		1
#define SDRAM_TRCD_3		2
#define SDRAM_TRCD_4		3
#define SDRAM_TRCD_5		4
#define SDRAM_TRCD_6		5
#define SDRAM_TRCD_7		6
#define SDRAM_TRCD_8		7

#define SDRAM_TRAS_1		0
#define SDRAM_TRAS_2		1
#define SDRAM_TRAS_3		2
#define SDRAM_TRAS_4		3
#define SDRAM_TRAS_5		4
#define SDRAM_TRAS_6		5
#define SDRAM_TRAS_7		6
#define SDRAM_TRAS_8		7
#define SDRAM_TRAS_9		8
#define SDRAM_TRAS_10		9
#define SDRAM_TRAS_11		10
#define SDRAM_TRAS_12		11
#define SDRAM_TRAS_13		12
#define SDRAM_TRAS_14		13
#define SDRAM_TRAS_15		14
#define SDRAM_TRAS_16		15

#define SDRAM_TRP_1			0
#define SDRAM_TRP_2			1
#define SDRAM_TRP_3			2
#define SDRAM_TRP_4			3
#define SDRAM_TRP_5			4
#define SDRAM_TRP_6			5
#define SDRAM_TRP_7			6
#define SDRAM_TRP_8			7

#define SDRAM_TWR_1			0
#define SDRAM_TWR_2			1
#define SDRAM_TWR_3			2
#define SDRAM_TWR_4			3
#define SDRAM_TWR_5			4
#define SDRAM_TWR_6			5
#define SDRAM_TWR_7			6
#define SDRAM_TWR_8			7

#define SDRAM_TXSR_1		0
#define SDRAM_TXSR_2		1
#define SDRAM_TXSR_3		2
#define SDRAM_TXSR_4		3
#define SDRAM_TXSR_5		4
#define SDRAM_TXSR_6		5
#define SDRAM_TXSR_7		6
#define SDRAM_TXSR_8		7
#define SDRAM_TXSR_9		8
#define SDRAM_TXSR_10		9
#define SDRAM_TXSR_11		10
#define SDRAM_TXSR_12		11
#define SDRAM_TXSR_13		12
#define SDRAM_TXSR_14		13
#define SDRAM_TXSR_15		14
#define SDRAM_TXSR_16		15
#define SDRAM_TXSR_17		16
#define SDRAM_TXSR_18		17
#define SDRAM_TXSR_19		18
#define SDRAM_TXSR_20		19
#define SDRAM_TXSR_21		20
#define SDRAM_TXSR_22		21
#define SDRAM_TXSR_23		22
#define SDRAM_TXSR_24		23
#define SDRAM_TXSR_25		24
#define SDRAM_TXSR_26		25
#define SDRAM_TXSR_27		26
#define SDRAM_TXSR_28		27
#define SDRAM_TXSR_29		28
#define SDRAM_TXSR_30		29
#define SDRAM_TXSR_31		30
#define SDRAM_TXSR_32		31

#define SDRAM_TRFC_1		0
#define SDRAM_TRFC_2		1
#define SDRAM_TRFC_3		2
#define SDRAM_TRFC_4		3
#define SDRAM_TRFC_5		4
#define SDRAM_TRFC_6		5
#define SDRAM_TRFC_7		6
#define SDRAM_TRFC_8		7
#define SDRAM_TRFC_9		8
#define SDRAM_TRFC_10		9
#define SDRAM_TRFC_11		10
#define SDRAM_TRFC_12		11
#define SDRAM_TRFC_13		12
#define SDRAM_TRFC_14		13
#define SDRAM_TRFC_15		14
#define SDRAM_TRFC_16		15


void SDRAM_Init(SDRAM_InitStructure * initStruct);

void SDRAM_Enable(void);
void SDRAM_Disable(void);

#endif //__SWM342_SDRAM_H__

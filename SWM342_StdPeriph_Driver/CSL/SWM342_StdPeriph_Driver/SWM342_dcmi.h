#ifndef __SWM342_DCMI_H__
#define __SWM342_DCMI_H__


typedef struct {
	uint16_t Mode;				//DCMI_MODE_SnapShot、DCMI_MODE_Continue_All、DCMI_MODE_Continue_1of2、DCMI_MODE_Continue_1of4
	uint8_t  Format;			//DCMI_FMT_Uncompressed、DCMI_FMT_JPEG
	uint8_t  BusWidth;			//DCMI_BusWidth_8b、DCMI_BusWidth_10b、DCMI_BusWidth_12b、DCMI_BusWidth_14b
	uint8_t  PCKPolarity;		//DCMI_PCKPolarity_Falling、DCMI_PCKPolarity_Rising
	uint8_t  VSPolarity;		//DCMI_VSPolarity_Low、DCMI_VSPolarity_High
	uint8_t  HSPolarity;		//DCMI_HSPolarity_Low、DCMI_HSPolarity_High
	uint8_t  IntEOCEn;			//End of Frame Capture Interrupt Enable
} DCMI_InitStructure;


#define DCMI_MODE_SnapShot			(1 | (0 << 7))
#define DCMI_MODE_Continue_All		(0 | (0 << 7))
#define DCMI_MODE_Continue_1of2		(0 | (1 << 7))
#define DCMI_MODE_Continue_1of4		(0 | (2 << 7))

#define DCMI_FMT_Uncompressed		0
#define DCMI_FMT_JPEG				1

#define DCMI_BusWidth_8b			0
#define DCMI_BusWidth_10b			1
#define DCMI_BusWidth_12b			2
#define DCMI_BusWidth_14b			3

#define DCMI_PCKPolarity_Falling	0			// DCMI_PCK falling edge active
#define DCMI_PCKPolarity_Rising		1

#define DCMI_VSPolarity_Low			0			// DCMI_VSYNC active low
#define DCMI_VSPolarity_High		1

#define DCMI_HSPolarity_Low			0
#define DCMI_HSPolarity_High		1


typedef enum {
	DCMI_Line_All         = (0 | (0 << 1)),
	DCMI_Line_1of2_First  = (1 | (0 << 1)),		// capture first line, second line being dropped
	DCMI_Line_1of2_Second = (1 | (1 << 1))		// capture second line, first line being dropped
} DCMI_DownSampling_Line;

typedef enum {
	DCMI_Byte_All         = (0 | (0 << 2)),
	DCMI_Byte_1of2_First  = (1 | (0 << 2)),
	DCMI_Byte_1of2_Second = (1 | (1 << 2)),
	DCMI_Byte_1of4_First  = (2 | (0 << 2)),
	DCMI_Byte_1of4_Second = (2 | (1 << 2)),
	DCMI_Byte_2of4_First  = (3 | (0 << 2)),		// capture first two-byte
	DCMI_Byte_2of4_Second = (3 | (1 << 2))		// capture second two-byte
} DCMI_DownSampling_Byte;


/* interrupt type */
#define DCMI_IT_FRAME		0x01	// Frame Capture complete
#define DCMI_IT_FIFOOV		0x02	// FIFO Overrun
#define DCMI_IT_ESCERR		0x04	// Embedded Synchronization Code Error
#define DCMI_IT_VSYNC		0x08	// VSYNC change from inactive to active
#define DCMI_IT_HSYNC		0x10	// HSYNC change from inactive to active



void DCMI_Init(DCMI_TypeDef * DCMIx, DCMI_InitStructure * initStruct);
void DCMI_Open(DCMI_TypeDef * DCMIx);
void DCMI_Close(DCMI_TypeDef * DCMIx);
void DCMI_Start(DCMI_TypeDef * DCMIx);
void DCMI_Stop(DCMI_TypeDef * DCMIx);

bool DCMI_DataAvaliable(DCMI_TypeDef * DCMIx);

void DCMI_SetCropWindow(DCMI_TypeDef * DCMIx, uint16_t start_x, uint16_t start_y, uint16_t count_x, uint16_t count_y);
void DCMI_CropSwitch(DCMI_TypeDef * DCMIx, bool on);

void DCMI_DownSampling(DCMI_TypeDef * DCMIx, DCMI_DownSampling_Line line_sel, DCMI_DownSampling_Byte byte_sel);

void DCMI_EmbeddedSync(DCMI_TypeDef * DCMIx, uint8_t frame_start, uint8_t line_start, uint8_t line_end, uint8_t frame_end);

void DCMI_INTEn(DCMI_TypeDef * DCMIx, uint32_t it);
void DCMI_INTDis(DCMI_TypeDef * DCMIx, uint32_t it);
void DCMI_INTClr(DCMI_TypeDef * DCMIx, uint32_t it);
bool DCMI_INTStat(DCMI_TypeDef * DCMIx, uint32_t it);


#endif //__SWM342_DCMI_H__

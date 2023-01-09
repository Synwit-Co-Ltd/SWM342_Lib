#ifndef __SWM342_SRA_H__
#define __SWM342_SRA_H__


typedef struct {
	uint8_t  IntEOTEn;		// End of Transform（转换完成）中断使能
} SRA_InitStructure;


#define SRA_FMT_MONO8b		(0 | (0 << 4))	// Monochrome 8-bit
#define SRA_FMT_RGB565		(1 | (0 << 4))
#define SRA_FMT_BGR565		(1 | (1 << 4))
#define SRA_FMT_RGB888		(2 | (0 << 4))
#define SRA_FMT_BGR888		(2 | (1 << 4))
#define SRA_FMT_RGB888X		(3 | (0 << 4))
#define SRA_FMT_BGR888X		(3 | (1 << 4))


typedef struct {
	uint16_t Format;			// SRA_FMT_MONO8b、SRA_FMT_RGB565、SRA_FMT_BGR565、...
	uint32_t BGColor;			// Background Color
	uint32_t ImageAddr;
	uint16_t ImageWidth;
	uint16_t ImageHeight;
	uint16_t ScaleWidth;		// 缩放后图片宽度，必须满足：0.5 ≤ ScaleWidth/ImageWidth   ≤ 4
	uint16_t ScaleHeight;		// 缩放后图片高度，必须满足：0.5 ≤ ScaleHeight/ImageHeight ≤ 4
	uint16_t RotateAngle;		// 图片旋转角度，取值 0-359、SRA_MIRROR_X、SRA_MIRROR_Y、SRA_MIRROR_XY
	uint32_t OutputAddr;
} SRA_TransformParam;


#define SRA_MIRROR_X		361	// 水平镜像
#define SRA_MIRROR_Y		362	// 垂直镜像
#define SRA_MIRROR_XY		363	// 水平和垂直镜像


#define SRA_IT_FRAME		(1 << 0)
#define SRA_IT_8LINE		(1 << 1)
#define SRA_IT_ROTERR		(1 << 2)		// Rotate Error
#define SRA_IT_BUSERR		(1 << 3)



void SRA_Init(SRA_InitStructure * initStruct);
bool SRA_Transform(SRA_TransformParam * transParam, uint16_t *dstWidth, uint16_t *dstHeight);

void SRA_Start(void);
void SRA_Abort(void);
uint32_t SRA_Busy(void);

void SRA_INTEn(uint32_t it);
void SRA_INTDis(uint32_t it);
void SRA_INTClr(uint32_t it);
uint32_t SRA_INTStat(uint32_t it);


#endif //__SWM342_SRA_H__

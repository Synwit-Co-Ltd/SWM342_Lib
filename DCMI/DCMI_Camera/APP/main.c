#include <string.h>
#include "SWM342.h"
#include "OV2640.h"
#include "jfif_parser.h"


#define LCD_HDOT	480		// ˮƽ����
#define LCD_VDOT	272		// ��ֱ����
#define LCD_DIRH	1		// ˮƽ��ʾ��

#define CAP_FMT		OV_FMT_RGB565
#define CAP_HDOT	320
#define CAP_VDOT	240


uint16_t *LCD_Buffer = (uint16_t *)(SDRAMM_BASE);
uint16_t *CAP_Buffer = (uint16_t *)(SDRAMM_BASE + 0x100000);
uint16_t *RGB_Buffer = (uint16_t *)(SDRAMM_BASE + 0x200000);


void SerialInit(void);
void MemoryInit(void);
void RGBLCDInit(void);

int main(void)
{
	DMA_InitStructure DMA_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	MemoryInit();
	
	RGBLCDInit();
	
	LCD_Start(LCD);
	
	OV2640_Init(CAP_FMT, 5, CAP_HDOT, CAP_VDOT);
	
	DMA_initStruct.Mode = DMA_MODE_CIRCLE;
	DMA_initStruct.Unit = DMA_UNIT_WORD;
	DMA_initStruct.Count = CAP_HDOT * CAP_VDOT * 2 / 4;
	DMA_initStruct.SrcAddr = (uint32_t)&DCMI->DR;
	DMA_initStruct.SrcAddrInc = 0;
	DMA_initStruct.DstAddr = (uint32_t)CAP_Buffer;
	DMA_initStruct.DstAddrInc = 1;
	DMA_initStruct.Handshake = DMA_CH4_DCMI;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = 0;
	DMA_CH_Init(DMA_CH4, &DMA_initStruct);
	DMA_CH_Open(DMA_CH4);
	
#if CAP_FMT == OV_FMT_JPEG
	jfif_info_t jfif_info;
	jpeg_outset_t jpeg_outset;
	JPEG_InitStructure JPEG_initStruct;
	
	JPEG_initStruct.DoneIEn = 0;
	JPEG_initStruct.ErrorIEn = 0;
	JPEG_Init(JPEG, &JPEG_initStruct);
#endif

	while(1==1)
	{
#if CAP_FMT == OV_FMT_RGB565
		DCMI_Start(DCMI);
		while(DMA_CH_INTStat(DMA_CH4, DMA_IT_DONE) == 0) __NOP();
		
		for(int i = 0; i < CAP_VDOT; i++)
			for(int j = 0; j < CAP_HDOT; j++)
				LCD_Buffer[LCD_HDOT * i + j] = CAP_Buffer[CAP_HDOT * i + j];
#else
		DCMI_Start(DCMI);
		while(DCMI_INTStat(DCMI, DCMI_IT_FRAME) == 0) __NOP();
		DCMI_INTClr(DCMI, DCMI_IT_FRAME);
		
		DMA_CH_Abort(DMA_CH4);	// ѹ����ͼƬ����������ȻС�� CAP_HDOT * CAP_VDOT * 2 / 4 ���������Ҫ��ֹ����
		DMA_CH_Open(DMA_CH4);	// Ϊ��һ�β����� DMA ͨ��
		
		uint32_t xfer_len = CAP_HDOT * CAP_VDOT * 2 / 4 - DMA_CH_GetRemaining(DMA_CH4);
		
		jfif_parse((uint8_t  *)CAP_Buffer, xfer_len * 4, &jfif_info);
		
		jpeg_outset.format = JPEG_OUT_RGB565;
		jpeg_outset.dither = 0;
		jpeg_outset.RGBAddr = (uint32_t)RGB_Buffer;
		JPEG_Decode(JPEG, &jfif_info, &jpeg_outset);
		while(JPEG_DecodeBusy(JPEG)) __NOP();
		
		for(int i = 0; i < CAP_VDOT; i++)
			for(int j = 0; j < CAP_HDOT; j++)
				LCD_Buffer[LCD_HDOT * i + j] = RGB_Buffer[CAP_HDOT * i + j];
#endif
	}
}


void RGBLCDInit(void)
{
	uint32_t i;
	LCD_InitStructure LCD_initStruct;
	
	GPIO_Init(GPIOA, PIN6, 1, 0, 0, 0);		//��Ļ����
	GPIO_SetBit(GPIOA, PIN6);
	GPIO_Init(GPIOC, PIN6, 1, 0, 0, 0);		//��Ļ��λ
	GPIO_ClrBit(GPIOC, PIN6);
	for(i = 0; i < 1000000; i++) __NOP();
	GPIO_SetBit(GPIOC, PIN6);
	
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_LCD_B0,  0);
	PORT_Init(PORTB, PIN11, PORTB_PIN11_LCD_B1, 0);
	PORT_Init(PORTB, PIN13, PORTB_PIN13_LCD_B2, 0);
	PORT_Init(PORTB, PIN15, PORTB_PIN15_LCD_B3, 0);
	PORT_Init(PORTA, PIN2,  PORTA_PIN2_LCD_B4,  0);
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_LCD_B5,  0);
	PORT_Init(PORTA, PIN10, PORTA_PIN10_LCD_B6, 0);
	PORT_Init(PORTA, PIN11, PORTA_PIN11_LCD_B7, 0);
	PORT_Init(PORTA, PIN12, PORTA_PIN12_LCD_G0, 0);
	PORT_Init(PORTA, PIN13, PORTA_PIN13_LCD_G1, 0);
	PORT_Init(PORTA, PIN14, PORTA_PIN14_LCD_G2, 0);
	PORT_Init(PORTA, PIN15, PORTA_PIN15_LCD_G3, 0);
	PORT_Init(PORTC, PIN0,  PORTC_PIN0_LCD_G4,  0);
	PORT_Init(PORTC, PIN1,  PORTC_PIN1_LCD_G5,  0);
	PORT_Init(PORTC, PIN2,  PORTC_PIN2_LCD_G6,  0);
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_LCD_G7,  0);
	PORT_Init(PORTC, PIN4,  PORTC_PIN4_LCD_R0,  0);
	PORT_Init(PORTC, PIN5,  PORTC_PIN5_LCD_R1,  0);
	PORT_Init(PORTC, PIN8,  PORTC_PIN8_LCD_R2,  0);
	PORT_Init(PORTC, PIN9,  PORTC_PIN9_LCD_R3,  0);
	PORT_Init(PORTC, PIN10, PORTC_PIN10_LCD_R4, 0);
	PORT_Init(PORTC, PIN11, PORTC_PIN11_LCD_R5, 0);
	PORT_Init(PORTC, PIN12, PORTC_PIN12_LCD_R6, 0);
	PORT_Init(PORTC, PIN13, PORTC_PIN13_LCD_R7, 0);
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_LCD_VSYNC, 0);
	PORT_Init(PORTB, PIN3,  PORTB_PIN3_LCD_HSYNC, 0);
	PORT_Init(PORTB, PIN4,  PORTB_PIN4_LCD_DEN,   0);
	PORT_Init(PORTB, PIN5,  PORTB_PIN5_LCD_DCLK,  0);
	
	LCD_initStruct.ClkDiv = 6;
	LCD_initStruct.Format = LCD_FMT_RGB565;
	LCD_initStruct.HnPixel = LCD_HDOT;
	LCD_initStruct.VnPixel = LCD_VDOT;
	LCD_initStruct.Hfp = 5;
	LCD_initStruct.Hbp = 40;
	LCD_initStruct.Vfp = 8;
	LCD_initStruct.Vbp = 8;
	LCD_initStruct.HsyncWidth = 5;
	LCD_initStruct.VsyncWidth = 5;
	LCD_initStruct.DataSource = (uint32_t)LCD_Buffer;
	LCD_initStruct.Background = 0xFFFF;
	LCD_initStruct.SampleEdge = LCD_SAMPLE_FALL;	// ATK-4342 RGBLCD �½��ز���
	LCD_initStruct.IntEOTEn = 0;
	LCD_Init(LCD, &LCD_initStruct);
}


void MemoryInit(void)
{
	SDRAM_InitStructure SDRAM_InitStruct;
	
	PORT_Init(PORTM, PIN13, PORTM_PIN13_SDR_CLK, 0);
	PORT_Init(PORTM, PIN14, PORTM_PIN14_SDR_CKE, 0);
	PORT_Init(PORTB, PIN7,  PORTB_PIN7_SDR_WE,   0);
	PORT_Init(PORTB, PIN8,  PORTB_PIN8_SDR_CAS,  0);
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_SDR_RAS,  0);
	PORT_Init(PORTB, PIN10, PORTB_PIN10_SDR_CS,  0);
	PORT_Init(PORTE, PIN15, PORTE_PIN15_SDR_BA0, 0);
	PORT_Init(PORTE, PIN14, PORTE_PIN14_SDR_BA1, 0);
	PORT_Init(PORTN, PIN14, PORTN_PIN14_SDR_A0,  0);
	PORT_Init(PORTN, PIN13, PORTN_PIN13_SDR_A1,  0);
	PORT_Init(PORTN, PIN12, PORTN_PIN12_SDR_A2,  0);
	PORT_Init(PORTN, PIN11, PORTN_PIN11_SDR_A3,  0);
	PORT_Init(PORTN, PIN10, PORTN_PIN10_SDR_A4,  0);
	PORT_Init(PORTN, PIN9,  PORTN_PIN9_SDR_A5,   0);
	PORT_Init(PORTN, PIN8,  PORTN_PIN8_SDR_A6,   0);
	PORT_Init(PORTN, PIN7,  PORTN_PIN7_SDR_A7,   0);
	PORT_Init(PORTN, PIN6,  PORTN_PIN6_SDR_A8,   0);
	PORT_Init(PORTN, PIN3,  PORTN_PIN3_SDR_A9,   0);
	PORT_Init(PORTN, PIN15, PORTN_PIN15_SDR_A10, 0);
	PORT_Init(PORTN, PIN2,  PORTN_PIN2_SDR_A11,  0);
	PORT_Init(PORTM, PIN15, PORTM_PIN15_SDR_A12, 0);
	PORT_Init(PORTE, PIN7,  PORTE_PIN7_SDR_D0,   1);
	PORT_Init(PORTE, PIN6,  PORTE_PIN6_SDR_D1,   1);
	PORT_Init(PORTE, PIN5,  PORTE_PIN5_SDR_D2,   1);
	PORT_Init(PORTE, PIN4,  PORTE_PIN4_SDR_D3,   1);
	PORT_Init(PORTE, PIN3,  PORTE_PIN3_SDR_D4,   1);
	PORT_Init(PORTE, PIN2,  PORTE_PIN2_SDR_D5,   1);
	PORT_Init(PORTE, PIN1,  PORTE_PIN1_SDR_D6,   1);
	PORT_Init(PORTE, PIN0,  PORTE_PIN0_SDR_D7,   1);
	PORT_Init(PORTE, PIN8,  PORTE_PIN8_SDR_D8,   1);
	PORT_Init(PORTE, PIN9,  PORTE_PIN9_SDR_D9,   1);
	PORT_Init(PORTE, PIN10, PORTE_PIN10_SDR_D10, 1);
	PORT_Init(PORTE, PIN11, PORTE_PIN11_SDR_D11, 1);
	PORT_Init(PORTE, PIN12, PORTE_PIN12_SDR_D12, 1);
	PORT_Init(PORTE, PIN13, PORTE_PIN13_SDR_D13, 1);
	PORT_Init(PORTC, PIN14, PORTC_PIN14_SDR_D14, 1);
	PORT_Init(PORTC, PIN15, PORTC_PIN15_SDR_D15, 1);
	PORT_Init(PORTB, PIN6,  PORTB_PIN6_SDR_LDQM, 0);
	PORT_Init(PORTM, PIN12, PORTM_PIN12_SDR_UDQM,0);
	
	SDRAM_InitStruct.ClkDiv = SDRAM_CLKDIV_1;
	SDRAM_InitStruct.NbrBank = SDRAM_BANK_4;
	SDRAM_InitStruct.NbrRowAddr = SDRAM_ROW_12;
	SDRAM_InitStruct.NbrColAddr = SDRAM_COLUMN_8;
	SDRAM_InitStruct.CASLatency = SDRAM_CASLATENCY_3;
	SDRAM_InitStruct.TimeTRC  = SDRAM_TRC_14;
	SDRAM_InitStruct.TimeTRRD = SDRAM_TRRD_4;
	SDRAM_InitStruct.TimeTRCD = SDRAM_TRCD_4;
	SDRAM_InitStruct.TimeTRAS = SDRAM_TRAS_10;
	SDRAM_InitStruct.TimeTRP  = SDRAM_TRP_4;
	SDRAM_Init(&SDRAM_InitStruct);
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);		//GPIOM.0����ΪUART0��������
 	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);		//GPIOM.1����ΪUART0�������
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

/****************************************************************************************************************************************** 
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}

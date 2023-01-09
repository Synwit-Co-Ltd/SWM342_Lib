#include <string.h>
#include "SWM342.h"


uint32_t WrBuff[4096 / 4];
uint32_t RdBuff[4096 / 4];


void SerialInit(void);
void SFC_Config(uint8_t width);

int main(void)
{
	SystemInit();
	
	SerialInit();

	SFC_Config(4);
	
	CACHE_Open();
	
	uint32_t id = SFC_ReadJEDEC();
	
	printf("\r\nFlash JEDEC: %06X\r\n", id);
	
	for(int i = 0; i < sizeof(WrBuff) / 4; i++)
		WrBuff[i] = (i << 16) | i;
	
	for(int addr = 0; addr < 1024 * 64; addr += 4096)
	{
		SFC_Erase(addr, 1);
		
		for(int page = 0; page < 4096; page += 256)
		{
			SFC_Write(addr + page, &WrBuff[page / 4], 64);
		}
		printf("Sector @ 0x%06X Write Done.\r\n", addr);
	}
	
	for(int addr = 0; addr < 1024 * 64; addr += 4096)
	{
		SFC_Read(addr, RdBuff, 4096 / 4);
		if(memcmp(RdBuff, WrBuff, 4096) == 0)
			printf("Sector @ 0x%06X Direct Read Pass!\r\n", addr);
		else
			printf("Sector @ 0x%06X Direct Read Fail!\r\n", addr);
		
		memcpy(RdBuff, (void *)(0x20014000 + addr), 4096);
		if(memcmp(RdBuff, WrBuff, 4096) == 0)
			printf("Sector @ 0x%06X Cache Read Pass!\r\n", addr);
		else
			printf("Sector @ 0x%06X Cache Read Fail!\r\n", addr);
	}
	
	while(1==1)
	{
	}
}


void SFC_Config(uint8_t width)
{
	SFC_InitStructure SFC_initStruct;
	
	/* SFC使用专用的FSPI（Flash SPI）接口连接SPI Flash */
	PORT_Init(PORTD, PIN5, PORTD_PIN5_FSPI_SCLK, 0);
	PORT_Init(PORTD, PIN6, PORTD_PIN6_FSPI_SSEL, 0);
	PORT_Init(PORTD, PIN8, PORTD_PIN8_FSPI_MOSI, 1);
	PORT_Init(PORTD, PIN7, PORTD_PIN7_FSPI_MISO, 1);
	PORT_Init(PORTD, PIN3, PORTD_PIN3_FSPI_DAT2, 1);
	PORT_Init(PORTD, PIN4, PORTD_PIN4_FSPI_DAT3, 1);
	
	switch(width)
	{
	case 1:
	default:
		SFC_initStruct.ClkDiv = SFC_CLKDIV_4;
		SFC_initStruct.Cmd_Read = 0x03;
		SFC_initStruct.Width_Read = SFC_RDWIDTH_1;
		SFC_initStruct.Cmd_PageProgram = 0x02;
		SFC_initStruct.Width_PageProgram = SFC_PPWIDTH_1;
		break;
	
	case 2:
		SFC_initStruct.ClkDiv = SFC_CLKDIV_4;
		SFC_initStruct.Cmd_Read = 0xBB;
		SFC_initStruct.Width_Read = SFC_RDWIDTH_2;
		SFC_initStruct.Cmd_PageProgram = 0x02;
		SFC_initStruct.Width_PageProgram = SFC_PPWIDTH_1;
		break;
	
	case 4:
		SFC_initStruct.ClkDiv = SFC_CLKDIV_4;
		SFC_initStruct.Cmd_Read = 0xEB;
		SFC_initStruct.Width_Read = SFC_RDWIDTH_4;
		SFC_initStruct.Cmd_PageProgram = 0x32;
		SFC_initStruct.Width_PageProgram = SFC_PPWIDTH_4;
		break;
	}
	
	SFC_Init(&SFC_initStruct);
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);	//GPIOM.0配置为UART0输入引脚
 	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);	//GPIOM.1配置为UART0输出引脚
 	
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
* 函数名称: fputc()
* 功能说明: printf()使用此函数完成实际的串口打印动作
* 输    入: int ch		要打印的字符
*			FILE *f		文件句柄
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}

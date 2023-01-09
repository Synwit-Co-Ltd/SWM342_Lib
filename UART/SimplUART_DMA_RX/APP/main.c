#include "SWM342.h"
#include "CircleBuffer.h"

CircleBuffer_t CirBuf;

#define RX_LEN	1024
uint8_t RX_Buffer[RX_LEN] = {0};


void SerialInit(void);
void TestSingal(void);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	TestSingal();
   	
	while(1==1)
	{
		uint8_t chr;
		if(CirBuf_Read(&CirBuf, &chr, 1))
			printf("%c", chr);
	}
}


void TestSingal(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN2, PORTM_PIN2_UART1_RX, 1);
	PORT_Init(PORTM, PIN3, PORTM_PIN3_UART1_TX, 0);
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART1, &UART_initStruct);
	UART_Open(UART1);
	
	TIMR_Init(BTIMR0, TIMR_MODE_TIMER, CyclesPerUs, 1000, 1);
	TIMR_Start(BTIMR0);
}


void BTIMR0_Handler(void)
{
	static int n = 0;
	static char tx_buff[8][32] = {
		"TestString\r\n",
		"TestString1\r\n",
		"TestString12\r\n",
		"TestString123\r\n",
		"TestString1234\r\n",
		"TestString12345\r\n",
		"TestString123456\r\n",
		"TestString1234567\r\n",
	};
	
	TIMR_INTClr(BTIMR0);
	
	int i = n / 1000;
	int j = n % 1000;
	
	if(j < strlen(tx_buff[i]))
		UART_WriteByte(UART1, tx_buff[i][j]);
	
	n++;
	if(n == 8000) n = 0;
}


void UART0_Handler(void)
{
	if(UART_INTStat(UART0, UART_IT_RX_TOUT))
	{
		UART_INTClr(UART0, UART_IT_RX_TOUT);
		
		int xfered = RX_LEN - (DMA->CH[DMA_CH1].SRCSR & DMA_SRCSR_LEN_Msk);		// ARORT 之后 DMA->SRCSR.LEN 会被清零，因此必须在 ARORT 之前读传输个数
																				// 这里 DMA->DSTSR.LEN 无法反映实际剩余个数，因此必须用 DMA->SRCSR.LEN
		DMA->CH[DMA_CH1].CR |= DMA_CR_ABORT_Msk;	// 将 DMA FIFO 的数据 Flush 到 RAM
		
		CirBuf_Write(&CirBuf, RX_Buffer, xfered);
		
		DMA_CH_Open(DMA_CH1);	// 接收下一帧数据
	}
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
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 1;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
	
	
	DMA_InitStructure DMA_initStruct;
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.Count = RX_LEN;
	DMA_initStruct.SrcAddr = (uint32_t)&UART0->DATA;
	DMA_initStruct.SrcAddrInc = 0;
	DMA_initStruct.DstAddr = (uint32_t)RX_Buffer;
	DMA_initStruct.DstAddrInc = 1;
	DMA_initStruct.Handshake = DMA_CH1_UART0RX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = 0;
	DMA_CH_Init(DMA_CH1, &DMA_initStruct);
	DMA_CH_Open(DMA_CH1);
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

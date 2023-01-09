#include "SWM342.h"


/* 测试说明
 *     CANFD_MODE_NORMAL 模式：CAN_TX/RX 引脚上可以测到波形（PB4/PB5 短接，或经收发器连接 CAN 协议分析仪），但 CAN0 无法接收到自己发出的数据包
 *	   CANFD_MODE_LOOPBACK 模式：CAN_TX/RX 引脚上测不到波形，但 CAN0 能接收到自己发出的数据包并打印出来
 ********************************************************************************************************************************************/


CANFD_Message CAN_TXMsg, CAN_RXMsg;
CANFD_IDFilter CAN_IDFilter0, CAN_IDFilter1;


void SerialInit(void);

int main(void)
{
	CANFD_InitStructure CAN_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTB, PIN5, PORTB_PIN5_CAN0_RX, 1);	//GPIOB.5配置为CAN0输入引脚
	PORT_Init(PORTB, PIN4, PORTB_PIN4_CAN0_TX, 0);	//GPIOB.4配置为CAN0输出引脚
	
	CAN_initStruct.Mode = CANFD_MODE_LOOPBACK;
	CAN_initStruct.CAN_bs1 = 12;
	CAN_initStruct.CAN_bs2 = 7;
	CAN_initStruct.CAN_sjw = 4;
	CAN_initStruct.Baudrate = 100000;
	CAN_initStruct.RXSuccessIEn = 0;
	CAN_initStruct.RXOverrunIEn = 0;
	CAN_initStruct.TXSuccessIEn = 0;
	CAN_initStruct.ErrorIEn = 0;
	CANFD_Init(CAN0, &CAN_initStruct);
	
	CANFD_SetBaudrate(CAN0, 1, 500000, 6, 3, 2);	//CANFD帧中，数据段传输波特率
	
	CAN_IDFilter0.canfd = CANFD_FILT_FD_and_nonFD;
	CAN_IDFilter0.extid = CANFD_FILT_EXT_and_STD;
	CAN_IDFilter0.remote = CANFD_FILT_REMOTE_and_DATA;
	CAN_IDFilter0.mask = CANFD_MKID(0x00F, 0x00FFF);	//对于标准帧： ID低4位无所谓， ID高7位必须匹配
														//对于扩展帧：BID低4位无所谓，BID高7位必须匹配；EID低12位无所谓，EID高6位必须匹配
	CAN_IDFilter0.id[0] = CANFD_MKID(0x122, 0x00000);	//对于标准帧：接收ID为0x12X的帧；对于扩展帧，接收BID为0x12X、EID为0x00XXX的帧
	CAN_IDFilter0.id[1] = CANFD_MKID(0x145, 0x01333);	//对于标准帧：接收ID为0x14X的帧；对于扩展帧，接收BID为0x14X、EID为0x01XXX的帧
	CANFD_SetIDFilter(CAN0, CANFD_RXBUF_0, &CAN_IDFilter0);
	
	CANFD_Open(CAN0);
	
	CAN_TXMsg.id = 0x122;
	CAN_TXMsg.format = CANFD_FRAME_STD;
	CAN_TXMsg.remote = 0;
	CAN_TXMsg.size = 8;
	for(int i = 0; i < CAN_TXMsg.size; i++)
		CAN_TXMsg.buff[i] = i;
	
	CANFD_Transmit(CAN0, CANFD_TXBUF_0, &CAN_TXMsg);
	while(CANFD_TXStatus(CAN0, CANFD_TXBUF_0) == CANFD_TXST_BUSY) __NOP();
	
	CAN_TXMsg.id = CANFD_MKID(0x145, 0x1333);
	CAN_TXMsg.format = CANFD_FRAME_EXT;
	CANFD_Transmit(CAN0, CANFD_TXBUF_0, &CAN_TXMsg);
	while(CANFD_TXStatus(CAN0, CANFD_TXBUF_0) == CANFD_TXST_BUSY) __NOP();
	
	while(CANFD_RXDataAvailable(CAN0, CANFD_RXBUF_0))
	{
		CANFD_Receive(CAN0, CANFD_RXBUF_0, &CAN_RXMsg);
		
		char *fmt;
		switch(CAN_RXMsg.format)
		{
		case CANFD_FRAME_STD:      fmt = "STD";      break;
		case CANFD_FRAME_EXT:      fmt = "EXT";      break;
		case CANFD_FRAME_FDSTD:    fmt = "FDSTD";    break;
		case CANFD_FRAME_FDEXT:    fmt = "FDEXT";    break;
		case CANFD_FRAME_FDSTD_SW: fmt = "FDSTD_SW"; break;
		case CANFD_FRAME_FDEXT_SW: fmt = "FDEXT_SW"; break;
		}
		
		printf("\r\nReceive %s %08X: ", fmt, CAN_RXMsg.id);
		for(int i = 0; i < CAN_RXMsg.size; i++) printf("%02X, ", CAN_RXMsg.buff[i]);
	}
	
	while(1==1)
	{
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);		//GPIOM.0配置为UART0输入引脚
 	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);		//GPIOM.1配置为UART0输出引脚
 	
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

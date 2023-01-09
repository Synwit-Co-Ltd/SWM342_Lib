#include <string.h>
#include "SWM342.h"

#include "CircleBuffer.h"

CircleBuffer_t CirBuf;


CANFD_IDFilter CAN_IDFilter0, CAN_IDFilter1;


void SerialInit(void);

int main(void)
{
	CANFD_InitStructure CAN_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTB, PIN5, PORTB_PIN5_CAN0_RX, 1);	//GPIOB.5����ΪCAN0��������
	PORT_Init(PORTB, PIN4, PORTB_PIN4_CAN0_TX, 0);	//GPIOB.4����ΪCAN0�������
	
	CAN_initStruct.Mode = CANFD_MODE_NORMAL;
	CAN_initStruct.CAN_bs1 = 12;
	CAN_initStruct.CAN_bs2 = 7;
	CAN_initStruct.CAN_sjw = 4;
	CAN_initStruct.Baudrate = 100000;
	CAN_initStruct.RXSuccessIEn = 1;
	CAN_initStruct.RXOverrunIEn = 0;
	CAN_initStruct.TXSuccessIEn = 0;
	CAN_initStruct.ErrorIEn = 0;
	CANFD_Init(CAN0, &CAN_initStruct);
	
	CANFD_SetBaudrate(CAN0, 1, 500000, 6, 3, 2);	//CANFD֡�У����ݶδ��䲨����
	
	CAN_IDFilter0.canfd = CANFD_FILT_FD_and_nonFD;
	CAN_IDFilter0.extid = CANFD_FILT_EXT_and_STD;
	CAN_IDFilter0.remote = CANFD_FILT_REMOTE_and_DATA;
	CAN_IDFilter0.mask = CANFD_MKID(0x00F, 0x00FFF);	//���ڱ�׼֡�� ID��4λ����ν�� ID��7λ����ƥ��
														//������չ֡��BID��4λ����ν��BID��7λ����ƥ�䣻EID��12λ����ν��EID��6λ����ƥ��
	CAN_IDFilter0.id[0] = CANFD_MKID(0x122, 0x00000);	//���ڱ�׼֡������IDΪ0x12X��֡��������չ֡������BIDΪ0x12X��EIDΪ0x00XXX��֡
	CAN_IDFilter0.id[1] = CANFD_MKID(0x145, 0x01333);	//���ڱ�׼֡������IDΪ0x14X��֡��������չ֡������BIDΪ0x14X��EIDΪ0x01XXX��֡
	CAN_IDFilter0.id[2] = CAN_IDFilter0.id[1];			//������ȷ��ֵ
	CAN_IDFilter0.id[3] = CAN_IDFilter0.id[1];
	CANFD_SetIDFilter(CAN0, CANFD_RXBUF_0, &CAN_IDFilter0);
	
	CANFD_Open(CAN0);
	
	while(1==1)
	{
		CANFD_Message CAN_rxmsg;
		
		if(CirBuf_Read(&CirBuf, &CAN_rxmsg, 1))
		{
			char *fmt;
			switch(CAN_rxmsg.format)
			{
			case CANFD_FRAME_STD:      fmt = "STD";      break;
			case CANFD_FRAME_EXT:      fmt = "EXT";      break;
			case CANFD_FRAME_FDSTD:    fmt = "FDSTD";    break;
			case CANFD_FRAME_FDEXT:    fmt = "FDEXT";    break;
			case CANFD_FRAME_FDSTD_SW: fmt = "FDSTD_SW"; break;
			case CANFD_FRAME_FDEXT_SW: fmt = "FDEXT_SW"; break;
			}
			
			if(CAN_rxmsg.remote)
			{
				printf("\r\nReceive %s Remote Request", fmt);
			}
			else
			{
				printf("\r\nReceive %s %08X: ", fmt, CAN_rxmsg.id);
				for(int i = 0; i < CAN_rxmsg.size; i++) printf("%02X, ", CAN_rxmsg.buff[i]);
			}
		}
	}
}


void CAN0_Handler(void)
{
	CANFD_Message CAN_rxmsg;
	
	if(CANFD_INTStat(CAN0, CANFD_IT_RX0OK))
	{
		CANFD_INTClr(CAN0, CANFD_IT_RX0OK);
		
		CANFD_Receive(CAN0, CANFD_RXBUF_0, &CAN_rxmsg);
		CirBuf_Write(&CirBuf, &CAN_rxmsg, 1);
	}
	else if(CANFD_INTStat(CAN0, CANFD_IT_RX1OK))
	{
		CANFD_INTClr(CAN0, CANFD_IT_RX1OK);
		
		CANFD_Receive(CAN0, CANFD_RXBUF_1, &CAN_rxmsg);
		CirBuf_Write(&CirBuf, &CAN_rxmsg, 1);
	}
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

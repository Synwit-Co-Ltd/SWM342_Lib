#include <string.h>
#include "SWM342.h"
#include "W25Q32.h"

#define EEPROM_ADDR	  0x008000

uint8_t WrBuff[20] = {0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
				      0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
uint8_t RdBuff[21] = {0};

void SerialInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	W25Q32_Init();
	
	W25Q32_EraseSector(EEPROM_ADDR);
 	while(W25Q32_IsBusy()) __NOP();	// �ȴ������������
	
	W25Q32_ReadData(EEPROM_ADDR, RdBuff, 20);
	
	printf("\r\nAfter Erase: \r\n");
	for(i = 0; i < 20; i++) printf("0x%02X, ", RdBuff[i]);
	
	W25Q32_WritePage(EEPROM_ADDR, WrBuff, 20);
 	while(W25Q32_IsBusy()) __NOP();	// �ȴ�д��������
	
	W25Q32_ReadData(EEPROM_ADDR, RdBuff, 20);
	
	printf("\r\nAfter Write: \r\n");
	for(i = 0; i < 20; i++) printf("0x%02X, ", RdBuff[i]);
	
	/* 4�߶� */
	memset(RdBuff, 0x00, 20);
	W25Q32_ReadData_4bit(EEPROM_ADDR, RdBuff, 20);
	
	printf("\r\nQuad Read: \r\n");
	for(i = 0; i < 20; i++) printf("0x%02X, ", RdBuff[i]);

	/* 4�߶���DMA�� */
	memset(RdBuff, 0x00, 20);
	W25Q32_ReadData_4bit_DMA(EEPROM_ADDR, RdBuff, 20);
	
	printf("\r\nQuad DMA Read: \r\n");
	for(i = 0; i < 20; i++) printf("0x%02X, ", RdBuff[i]);
	
	while(1==1)
	{
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);	//GPIOM.0����ΪUART0��������
 	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);	//GPIOM.1����ΪUART0�������
 	
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

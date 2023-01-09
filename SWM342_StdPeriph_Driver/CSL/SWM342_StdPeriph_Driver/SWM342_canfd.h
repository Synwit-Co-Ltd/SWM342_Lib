#ifndef __SWM342_CANFD_H__
#define __SWM342_CANFD_H__


typedef struct {
	uint8_t  Mode;				//CANFD_MODE_NORMAL、CANFD_MODE_LISTEN、CANFD_MODE_LOOPBACK
	uint8_t  CAN_bs1;			//取值 2--96，单位 TQ
	uint8_t  CAN_bs2;			//取值 1--32，单位 TQ
	uint8_t  CAN_sjw;			//取值 1--32，单位 TQ
	uint32_t Baudrate;			//波特率，即位传输速率，取值1--1000000
	uint8_t  RXSuccessIEn;
	uint8_t  RXOverrunIEn;
	uint8_t  TXSuccessIEn;
	uint8_t  ErrorIEn;
} CANFD_InitStructure;

#define CANFD_MODE_CONFIG		0	//配置模式
#define CANFD_MODE_NORMAL		1	//工作模式
#define CANFD_MODE_LISTEN		3	//监听模式
#define CANFD_MODE_LOOPBACK		4	//自测模式

#define CANFD_RETRY_Always		0	//Message will always be retransmitted if the previous transmission is failed.
#define CANFD_RETRY_Never		1	//Message will not be retransmitted if one message transmission is failed.
#define CANFD_RETRY_3Times		2	//Message will be retransmitted at most 3 times if one message transmission is failed.
#define CANFD_RETRY_8Times		3	//Message will be retransmitted at most 8 times if one message transmission is failed.


typedef struct {
	uint32_t id;		//消息ID
	uint8_t  format;	//帧格式：CANFD_FRAME_STD、CANFD_FRAME_EXT、...
	uint8_t  remote;	//消息是否为远程帧
	uint8_t  passive;	//1 frame is from error passive node
	uint8_t  size;
	uint8_t  buff[64];
} CANFD_Message;

#define CANFD_MKID(BID, EID)	(((BID) << 18) | (EID))

#define CANFD_FRAME_STD			0
#define CANFD_FRAME_EXT			1
#define CANFD_FRAME_FDSTD		2
#define CANFD_FRAME_FDEXT		3
#define CANFD_FRAME_FDSTD_SW	6
#define CANFD_FRAME_FDEXT_SW	7


typedef struct {
	uint8_t  canfd;		//CANFD_FILT_FD_only、CANFD_FILT_nonFD_only、CANFD_FILT_FD_and_nonFD
	uint8_t  extid;		//CANFD_FILT_EXT_only、CANFD_FILT_STD_only、CANFD_FILT_EXT_and_STD
	uint8_t  remote;	//CANFD_FILT_REMOTE_only、CANFD_FILT_DATA_only、CANFD_FILT_REMOTE_and_DATA
	uint32_t mask;		//filter pass：id | mask == ID | mask
	uint32_t id[4];		//RX Buffer0 有 4 个 filter ID    RX Buffer1 有 2 个 filter ID
} CANFD_IDFilter;

#define CANFD_FILT_FD_only			2
#define CANFD_FILT_nonFD_only		1
#define CANFD_FILT_FD_and_nonFD		0

#define CANFD_FILT_EXT_only			2
#define CANFD_FILT_STD_only			1
#define CANFD_FILT_EXT_and_STD		0

#define CANFD_FILT_REMOTE_only		2
#define CANFD_FILT_DATA_only		1
#define CANFD_FILT_REMOTE_and_DATA	0


#define CANFD_TXBUF_0		0
#define CANFD_TXBUF_1		1
#define CANFD_TXBUF_2		2

#define CANFD_RXBUF_0		0
#define CANFD_RXBUF_1		1


#define CANFD_TXST_BUSY		0
#define CANFD_TXST_SUCC		1
#define CANFD_TXST_ARBL		2
#define CANFD_TXST_ERR		3


/* Interrupt Type */
#define CANFD_IT_RX0OK		(1 << 1)	// RX buffer 0 received a frame successfully interrupt flag
#define CANFD_IT_RX1OK		(1 << 0)
#define CANFD_IT_TX0OK		(1 << 4)	// TX buffer 0 transmitted a frame successfully interrupt flag
#define CANFD_IT_TX1OK		(1 << 3)
#define CANFD_IT_TX2OK		(1 << 2)
#define CANFD_IT_RXOK		(1 << 3)	// RX buffer received a frame successfully interrupt enable
#define CANFD_IT_TXOK		(1 << 4)	// TX buffer transmitted a frame successfully interrupt enable
#define CANFD_IT_ERR		(1 << 5)
#define CANFD_IT_RXOV		(1 << 6)
#define CANFD_IT_WKUP		(1 << 7)




void CANFD_Init(CANFD_TypeDef * CANx, CANFD_InitStructure * initStruct);
void CANFD_Open(CANFD_TypeDef * CANx);
void CANFD_Close(CANFD_TypeDef * CANx);

void CANFD_Transmit(CANFD_TypeDef * CANx, uint32_t txbuf, CANFD_Message * msg);
void CANFD_Receive(CANFD_TypeDef * CANx, uint32_t rxbuf, CANFD_Message * msg);

void CANFD_CANFD_TXAbort(CANFD_TypeDef * CANx, uint32_t txbuf);
uint32_t CANFD_TXStatus(CANFD_TypeDef * CANx, uint32_t txbuf);
uint32_t CANFD_RXDataAvailable(CANFD_TypeDef * CANx, uint32_t rxbuf);

void CANFD_SetBaudrate(CANFD_TypeDef * CANx, uint32_t CANFD_data, uint32_t baudrate, uint32_t CAN_bs1, uint32_t CAN_bs2, uint32_t CAN_sjw);

void CANFD_SetIDFilter(CANFD_TypeDef * CANx, uint32_t rxbuf, CANFD_IDFilter * idf);
void CANFD_SetDataFilter(CANFD_TypeDef * CANx, uint32_t rxbuf, uint32_t index, uint8_t byte, uint8_t mask);

void CANFD_INTEn(CANFD_TypeDef * CANx, uint32_t it);
void CANFD_INTDis(CANFD_TypeDef * CANx, uint32_t it);
void CANFD_INTClr(CANFD_TypeDef * CANx, uint32_t it);
uint32_t CANFD_INTStat(CANFD_TypeDef * CANx, uint32_t it);


#endif //__SWM342_CANFD_H__

#ifndef __SWM342_PORT_H__
#define __SWM342_PORT_H__

void PORT_Init(PORT_TypeDef * PORTx, uint32_t n, uint32_t func, uint32_t digit_in_en);	//端口引脚功能选择，其可取值如下：



#define PORTA_PIN0_GPIO         0
#define PORTA_PIN0_I2C0_SDA		1
#define PORTA_PIN0_SPI1_SCLK	2
#define PORTA_PIN0_UART1_TX     3
#define PORTA_PIN0_HALL0_IN0    4
#define PORTA_PIN0_LCD_B7		5

#define PORTA_PIN1_GPIO         0
#define PORTA_PIN1_I2C0_SCL		1
#define PORTA_PIN1_SPI1_MISO	2
#define PORTA_PIN1_UART2_RTS	3
#define PORTA_PIN1_HALL0_IN1    4
#define PORTA_PIN1_LCD_B3		5

#define PORTA_PIN2_GPIO         0
#define PORTA_PIN2_SPI1_MOSI	1
#define PORTA_PIN2_UART1_RX     2
#define PORTA_PIN2_PWM_BRK0     3
#define PORTA_PIN2_HALL0_IN2    4
#define PORTA_PIN2_LCD_B4       5

#define PORTA_PIN3_GPIO         0
#define PORTA_PIN3_I2C0_SDA		1
#define PORTA_PIN3_LCD_B6		2
#define PORTA_PIN3_XTAL_IN    	15

#define PORTA_PIN4_GPIO         0
#define PORTA_PIN4_I2C0_SCL		1
#define PORTA_PIN4_LCD_B5		2
#define PORTA_PIN4_XTAL_OUT   	15

#define PORTA_PIN5_GPIO         0
#define PORTA_PIN5_PWM_BRK1     1
#define PORTA_PIN5_PWM_CLK1     2
#define PORTA_PIN5_SDR_LDQM		3

#define PORTA_PIN6_GPIO         0
#define PORTA_PIN6_I2C1_SDA		1
#define PORTA_PIN6_PWM_BRK0     2
#define PORTA_PIN6_PWM_CLK0     3

#define PORTA_PIN7_GPIO         0
#define PORTA_PIN7_I2C1_SCL   	1
#define PORTA_PIN7_TIMR3_IN		2
#define PORTA_PIN7_TIMR3_OUT	3
#define PORTA_PIN7_DCMI_PCLK	4

#define PORTA_PIN8_GPIO         0

#define PORTA_PIN9_GPIO         0
#define PORTA_PIN9_SPI1_DAT2	1
#define PORTA_PIN9_CAN0_RX		2
#define PORTA_PIN9_LCD_B5       3

#define PORTA_PIN10_GPIO        0
#define PORTA_PIN10_SPI1_DAT3	1
#define PORTA_PIN10_UART0_CTS   2
#define PORTA_PIN10_PWM_CLK1    3
#define PORTA_PIN10_BTIMR3_OUT	4
#define PORTA_PIN10_CAN0_TX		5
#define PORTA_PIN10_LCD_B6      6
#define PORTA_PIN10_ADC0_CH11	15

#define PORTA_PIN11_GPIO        0
#define PORTA_PIN11_UART0_RTS   1
#define PORTA_PIN11_PWM_CLK0    2
#define PORTA_PIN11_BTIMR2_OUT	3
#define PORTA_PIN11_LCD_B7      4
#define PORTA_PIN11_ADC1_TRIG1  5

#define PORTA_PIN12_GPIO        0
#define PORTA_PIN12_UART0_RX    1
#define PORTA_PIN12_BTIMR1_OUT	2
#define PORTA_PIN12_LCD_G0      3
#define PORTA_PIN12_ADC0_TRIG1  4
#define PORTA_PIN12_ADC0_CH10	15

#define PORTA_PIN13_GPIO        0
#define PORTA_PIN13_UART0_TX    1
#define PORTA_PIN13_BTIMR0_OUT	2
#define PORTA_PIN13_LCD_G1      3
#define PORTA_PIN13_ADC0_CH9	15

#define PORTA_PIN14_GPIO        0
#define PORTA_PIN14_SPI1_SSEL   1
#define PORTA_PIN14_LCD_G2     	2
#define PORTA_PIN14_ADC0_CH8	15

#define PORTA_PIN15_GPIO        0
#define PORTA_PIN15_SPI1_SCLK	1
#define PORTA_PIN15_LCD_G3     	2
#define PORTA_PIN15_ADC0_CH7	15

#define PORTB_PIN0_GPIO         0
#define PORTB_PIN0_UART1_TX     1
#define PORTB_PIN0_DMA_TRIG1	2
#define PORTB_PIN0_TIMR1_IN     3
#define PORTB_PIN0_TIMR1_OUT    4
#define PORTB_PIN0_SD_D6		5

#define PORTB_PIN1_GPIO         0
#define PORTB_PIN1_TIMR0_IN     1
#define PORTB_PIN1_TIMR0_OUT    2
#define PORTB_PIN1_LCD_B0		3
#define PORTB_PIN1_SD_D7		4

#define PORTB_PIN2_GPIO         0
#define PORTB_PIN2_SPI1_SCLK	1
#define PORTB_PIN2_UART3_RTS    2
#define PORTB_PIN2_BTIMR3_OUT	3
#define PORTB_PIN2_CAN1_TX		4
#define PORTB_PIN2_LCD_CS		5
#define PORTB_PIN2_LCD_VSYNC	5

#define PORTB_PIN3_GPIO         0
#define PORTB_PIN3_SPI1_MISO    1
#define PORTB_PIN3_UART3_TX     2
#define PORTB_PIN3_HALL0_IN0    3
#define PORTB_PIN3_BTIMR2_OUT	4
#define PORTB_PIN3_CAN1_RX      5
#define PORTB_PIN3_LCD_WR		6
#define PORTB_PIN3_LCD_HSYNC	6

#define PORTB_PIN4_GPIO         0
#define PORTB_PIN4_SPI1_MOSI	1
#define PORTB_PIN4_UART3_RX     2
#define PORTB_PIN4_HALL0_IN1    3
#define PORTB_PIN4_BTIMR1_OUT	4
#define PORTB_PIN4_CAN0_TX      5
#define PORTB_PIN4_LCD_RS      	6
#define PORTB_PIN4_LCD_DEN		6

#define PORTB_PIN5_GPIO         0
#define PORTB_PIN5_SPI1_SSEL	1
#define PORTB_PIN5_UART3_CTS    2
#define PORTB_PIN5_HALL0_IN2    3
#define PORTB_PIN5_BTIMR0_OUT	4
#define PORTB_PIN5_CAN0_RX		5
#define PORTB_PIN5_LCD_RD      	6
#define PORTB_PIN5_LCD_DCLK		6

#define PORTB_PIN6_GPIO         0
#define PORTB_PIN6_SDR_LDQM		1

#define PORTB_PIN7_GPIO         0
#define PORTB_PIN7_UART4_RTS	1
#define PORTB_PIN7_SDR_WE		2

#define PORTB_PIN8_GPIO         0
#define PORTB_PIN8_SPI0_SCLK    1
#define PORTB_PIN8_UART4_TX		2
#define PORTB_PIN8_TIMR3_IN     3
#define PORTB_PIN8_TIMR3_OUT    4
#define PORTB_PIN8_SDR_CAS		5

#define PORTB_PIN9_GPIO         0
#define PORTB_PIN9_SPI0_MISO    1
#define PORTB_PIN9_UART4_CTS	2
#define PORTB_PIN9_TIMR2_IN     3
#define PORTB_PIN9_TIMR2_OUT    4
#define PORTB_PIN9_SDR_RAS		5

#define PORTB_PIN10_GPIO        0
#define PORTB_PIN10_SPI0_MOSI   1
#define PORTB_PIN10_UART4_RX	2
#define PORTB_PIN10_TIMR1_IN    3
#define PORTB_PIN10_TIMR1_OUT   4
#define PORTB_PIN10_SDR_CS		5

#define PORTB_PIN11_GPIO        0
#define PORTB_PIN11_JTAG_TDO    1
#define PORTB_PIN11_UART2_TX    2
#define PORTB_PIN11_PWM_BRK2	3
#define PORTB_PIN11_TIMR0_IN    4
#define PORTB_PIN11_TIMR0_OUT   5
#define PORTB_PIN11_LCD_B1		6

#define PORTB_PIN12_GPIO        0
#define PORTB_PIN12_SWDCK		1
#define PORTB_PIN12_JTAG_TCK  	1
#define PORTB_PIN12_UART0_RX	2
#define PORTB_PIN12_TIMR3_IN    3
#define PORTB_PIN12_TIMR3_OUT   4

#define PORTB_PIN13_GPIO        0
#define PORTB_PIN13_JTAG_TDI    1
#define PORTB_PIN13_I2C1_SDA	2
#define PORTB_PIN13_UART2_RX    3
#define PORTB_PIN13_TIMR2_IN    4
#define PORTB_PIN13_TIMR2_OUT   5
#define PORTB_PIN13_LCD_B2      6

#define PORTB_PIN14_GPIO        0
#define PORTB_PIN14_SWDIO		1
#define PORTB_PIN14_JTAG_TMS 	1
#define PORTB_PIN14_SPI0_SSEL   2
#define PORTB_PIN14_UART0_TX   	3
#define PORTB_PIN14_TIMR1_IN    4
#define PORTB_PIN14_TIMR1_OUT   5

#define PORTB_PIN15_GPIO        0
#define PORTB_PIN15_JTAG_TRST   1
#define PORTB_PIN15_I2C1_SCL	2
#define PORTB_PIN15_UART2_CTS   3
#define PORTB_PIN15_TIMR0_IN    4
#define PORTB_PIN15_TIMR0_OUT   5
#define PORTB_PIN15_LCD_B3      6

#define PORTC_PIN0_GPIO         0
#define PORTC_PIN0_SPI1_MISO	1
#define PORTC_PIN0_UART2_TX     2
#define PORTC_PIN0_TIMR1_IN		3
#define PORTC_PIN0_TIMR1_OUT	4
#define PORTC_PIN0_LCD_G4		5
#define PORTC_PIN0_ADC0_CH6		15

#define PORTC_PIN1_GPIO         0
#define PORTC_PIN1_I2C0_SDA		1
#define PORTC_PIN1_SPI1_MOSI	2
#define PORTC_PIN1_UART2_RX		2
#define PORTC_PIN1_TIMR0_IN		4
#define PORTC_PIN1_TIMR0_OUT	5
#define PORTC_PIN1_LCD_G5		6
#define PORTC_PIN1_ADC0_CH5     15

#define PORTC_PIN2_GPIO         0
#define PORTC_PIN2_I2C0_SCL		1
#define PORTC_PIN2_UART3_RX		2
#define PORTC_PIN2_TIMR2_IN		3
#define PORTC_PIN2_TIMR2_OUT	4
#define PORTC_PIN2_LCD_G6		5
#define PORTC_PIN2_ADC0_CH4     15

#define PORTC_PIN3_GPIO         0
#define PORTC_PIN3_UART3_TX		1
#define PORTC_PIN3_BTIMR11_OUT	2
#define PORTC_PIN3_TIMR3_IN		3
#define PORTC_PIN3_TIMR3_OUT	4
#define PORTC_PIN3_LCD_G7		5
#define PORTC_PIN3_ADC0_CH3     15

#define PORTC_PIN4_GPIO         0
#define PORTC_PIN4_I2C1_SDA     1
#define PORTC_PIN4_SPI2_SSEL    2
#define PORTC_PIN4_BTIMR10_OUT	3
#define PORTC_PIN4_TIMR4_IN		4
#define PORTC_PIN4_TIMR4_OUT	5
#define PORTC_PIN4_LCD_R0      	6
#define PORTC_PIN4_ADC0_CH2     15

#define PORTC_PIN5_GPIO         0
#define PORTC_PIN5_I2C1_SCL     1
#define PORTC_PIN5_SPI2_MISO    2
#define PORTC_PIN5_BTIMR9_OUT	3
#define PORTC_PIN5_LCD_R1      	4
#define PORTC_PIN5_ADC0_CH1     15

#define PORTC_PIN6_GPIO         0
#define PORTC_PIN6_SPI2_MOSI    1
#define PORTC_PIN6_UART5_RX		2
#define PORTC_PIN6_BTIMR8_OUT	3
#define PORTC_PIN6_ADC0_CH0     15

#define PORTC_PIN7_GPIO         0
#define PORTC_PIN7_SPI2_SCLK    1
#define PORTC_PIN7_UART5_TX		2

#define PORTC_PIN8_GPIO         0
#define PORTC_PIN8_LCD_R2		1

#define PORTC_PIN9_GPIO         0
#define PORTC_PIN9_BTIMR7_OUT	1
#define PORTC_PIN9_TIMR2_IN		2
#define PORTC_PIN9_TIMR2_OUT	3
#define PORTC_PIN9_LCD_R3       4
#define PORTC_PIN9_ADC1_CH6     15

#define PORTC_PIN10_GPIO        0
#define PORTC_PIN10_BTIMR6_OUT	1
#define PORTC_PIN10_TIMR1_IN	2
#define PORTC_PIN10_TIMR1_OUT	3
#define PORTC_PIN10_LCD_R4      4
#define PORTC_PIN10_ADC1_CH5    15

#define PORTC_PIN11_GPIO        0
#define PORTC_PIN11_BTIMR5_OUT	1
#define PORTC_PIN11_TIMR0_IN	2
#define PORTC_PIN11_TIMR0_OUT	3
#define PORTC_PIN11_LCD_R5      4
#define PORTC_PIN11_ADC1_CH4    15

#define PORTC_PIN12_GPIO        0
#define PORTC_PIN12_UART0_RX	1
#define PORTC_PIN12_BTIMR4_OUT	2
#define PORTC_PIN12_LCD_R6      3
#define PORTC_PIN12_ADC1_CH3    15

#define PORTC_PIN13_GPIO        0
#define PORTC_PIN13_I2C0_SDA	1
#define PORTC_PIN13_UART0_TX	2
#define PORTC_PIN13_BTIMR3_OUT	3
#define PORTC_PIN13_TIMR4_IN	4
#define PORTC_PIN13_TIMR4_OUT	5
#define PORTC_PIN13_LCD_R7		6
#define PORTC_PIN13_ADC1_CH2    15

#define PORTC_PIN14_GPIO        0
#define PORTC_PIN14_UART5_RX	1
#define PORTC_PIN14_SDR_D14		2

#define PORTC_PIN15_GPIO        0
#define PORTC_PIN15_SDR_D15		1

#define PORTD_PIN0_GPIO         0
#define PORTD_PIN0_I2C0_SCL		1
#define PORTD_PIN0_TIMR3_IN		2
#define PORTD_PIN0_TIMR3_OUT	3
#define PORTD_PIN0_LCD_R4		4
#define PORTD_PIN0_ADC1_CH1     15

#define PORTD_PIN1_GPIO         0
#define PORTD_PIN1_LCD_R5		1
#define PORTD_PIN1_ADC1_CH0     15

#define PORTD_PIN2_GPIO         0
#define PORTD_PIN2_QEI_Z		1
#define PORTD_PIN2_DAC_OUT		15

#define PORTD_PIN3_GPIO         0
#define PORTD_PIN3_QEI_B		1
#define PORTD_PIN3_SPI0_DAT2	2
#define PORTD_PIN3_FSPI_DAT2   	3
#define PORTD_PIN3_UART1_TX     4
#define PORTD_PIN3_CAN1_TX		5
#define PORTD_PIN3_ADC1_TRIG1   6

#define PORTD_PIN4_GPIO         0
#define PORTD_PIN4_QEI_A		1
#define PORTD_PIN4_SPI0_DAT3	2
#define PORTD_PIN4_FSPI_DAT3   	3
#define PORTD_PIN4_UART1_RX     4
#define PORTD_PIN4_BTIMR11_OUT	5
#define PORTD_PIN4_CAN1_RX		6
#define PORTD_PIN4_ADC0_TRIG1   7

#define PORTD_PIN5_GPIO         0
#define PORTD_PIN5_SPI0_SCLK	1
#define PORTD_PIN5_FSPI_SCLK    2
#define PORTD_PIN5_UART3_TX     3
#define PORTD_PIN5_PWM_CLK1		4
#define PORTD_PIN5_BTIMR10_OUT	5
#define PORTD_PIN5_CAN0_TX      6
#define PORTD_PIN5_WAKEUP		15

#define PORTD_PIN6_GPIO         0
#define PORTD_PIN6_SPI0_SSEL	1
#define PORTD_PIN6_FSPI_SSEL    2
#define PORTD_PIN6_UART3_RX     3
#define PORTD_PIN6_PWM_CLK0		4
#define PORTD_PIN6_BTIMR9_OUT	5
#define PORTD_PIN6_CAN0_RX		6

#define PORTD_PIN7_GPIO         0
#define PORTD_PIN7_QEI_DIR		1
#define PORTD_PIN7_I2C1_SDA		2
#define PORTD_PIN7_SPI0_MISO	3
#define PORTD_PIN7_FSPI_MISO    4
#define PORTD_PIN7_BTIMR8_OUT	5

#define PORTD_PIN8_GPIO         0
#define PORTD_PIN8_I2C1_SCL		1
#define PORTD_PIN8_SPI0_MOSI	2
#define PORTD_PIN8_FSPI_MOSI	3
#define PORTD_PIN8_PWM1BN       4
#define PORTD_PIN8_TIMR4_IN     5
#define PORTD_PIN8_TIMR4_OUT    6

#define PORTD_PIN9_GPIO         0
#define PORTD_PIN9_PWM1B        1
#define PORTD_PIN9_TIMR3_IN     2
#define PORTD_PIN9_TIMR3_OUT    3

#define PORTD_PIN10_GPIO        0
#define PORTD_PIN10_PWM_CLK1    1
#define PORTD_PIN10_SDR_UDQM	2

#define PORTD_PIN11_GPIO        0
#define PORTD_PIN11_PWM_CLK0    1

#define PORTD_PIN12_GPIO        0

#define PORTD_PIN13_GPIO        0

#define PORTD_PIN14_GPIO        0

#define PORTD_PIN15_GPIO        0
#define PORTD_PIN15_WAKEUP     	15

#define PORTE_PIN0_GPIO			0
#define PORTE_PIN0_SDR_D7		1

#define PORTE_PIN1_GPIO			0
#define PORTE_PIN1_UART4_RX		1
#define PORTE_PIN1_SDR_D6		2

#define PORTE_PIN2_GPIO			0
#define PORTE_PIN2_UART4_TX		1
#define PORTE_PIN2_SDR_D5		2

#define PORTE_PIN3_GPIO			0
#define PORTE_PIN3_SDR_D4		1

#define PORTE_PIN4_GPIO			0
#define PORTE_PIN4_SDR_D3		1

#define PORTE_PIN5_GPIO			0
#define PORTE_PIN5_SDR_D2		1

#define PORTE_PIN6_GPIO			0
#define PORTE_PIN6_SDR_D1		1

#define PORTE_PIN7_GPIO			0
#define PORTE_PIN7_SDR_D0		1

#define PORTE_PIN8_GPIO			0
#define PORTE_PIN8_SDR_D8		1

#define PORTE_PIN9_GPIO			0
#define PORTE_PIN9_SDR_D9		1

#define PORTE_PIN10_GPIO		0
#define PORTE_PIN10_UART4_RX	1
#define PORTE_PIN10_SDR_D10		2

#define PORTE_PIN11_GPIO		0
#define PORTE_PIN11_UART4_TX	1
#define PORTE_PIN11_SDR_D11		2

#define PORTE_PIN12_GPIO		0
#define PORTE_PIN12_SDR_D12		1

#define PORTE_PIN13_GPIO		0
#define PORTE_PIN13_UART5_TX	1
#define PORTE_PIN13_SDR_D13		2

#define PORTE_PIN14_GPIO		0
#define PORTE_PIN14_SDR_BA1		1

#define PORTE_PIN15_GPIO		0
#define PORTE_PIN15_SDR_BA0		1

#define PORTF_PIN0_GPIO			0
#define PORTF_PIN0_DCMI_D1		1

#define PORTF_PIN1_GPIO			0
#define PORTF_PIN1_DCMI_D0		1

#define PORTF_PIN2_GPIO			0
#define PORTF_PIN2_DCMI_VSYNC	1

#define PORTF_PIN3_GPIO			0
#define PORTF_PIN3_DCMI_HSYNC	1

#define PORTF_PIN4_GPIO			0
#define PORTF_PIN4_DCMI_D5		1

#define PORTF_PIN5_GPIO			0
#define PORTF_PIN5_DCMI_D4		1

#define PORTF_PIN6_GPIO			0
#define PORTF_PIN6_DCMI_D3		1

#define PORTF_PIN7_GPIO			0
#define PORTF_PIN7_DCMI_D2		1

#define PORTF_PIN8_GPIO			0
#define PORTF_PIN8_DCMI_D9		1

#define PORTF_PIN9_GPIO			0
#define PORTF_PIN9_DCMI_D8		1

#define PORTF_PIN10_GPIO		0
#define PORTF_PIN10_DCMI_D7		1

#define PORTF_PIN11_GPIO		0
#define PORTF_PIN11_DCMI_D6		1

#define PORTF_PIN12_GPIO		0
#define PORTF_PIN12_DCMI_D13	1

#define PORTF_PIN13_GPIO		0
#define PORTF_PIN13_DCMI_D12	1

#define PORTF_PIN14_GPIO		0
#define PORTF_PIN14_DCMI_D11	1

#define PORTF_PIN15_GPIO		0
#define PORTF_PIN15_DCMI_D10	1

#define PORTM_PIN0_GPIO         0
#define PORTM_PIN0_UART0_RX     1
#define PORTM_PIN0_PWM_BRK1     2
#define PORTM_PIN0_CAN1_TX      3

#define PORTM_PIN1_GPIO         0
#define PORTM_PIN1_UART0_TX     1
#define PORTM_PIN1_PWM0A        2
#define PORTM_PIN1_TIMR2_IN     3
#define PORTM_PIN1_TIMR2_OUT    4
#define PORTM_PIN1_CAN1_RX		5

#define PORTM_PIN2_GPIO         0
#define PORTM_PIN2_SPI0_SCLK	1
#define PORTM_PIN2_UART1_RX		2
#define PORTM_PIN2_PWM0B        3
#define PORTM_PIN2_BTIMR3_OUT	4
#define PORTM_PIN2_TIMR1_IN     5
#define PORTM_PIN2_TIMR1_OUT    6
#define PORTM_PIN2_SD_CLK		7

#define PORTM_PIN3_GPIO         0
#define PORTM_PIN3_SPI0_SSEL	1
#define PORTM_PIN3_UART1_TX		2
#define PORTM_PIN3_PWM1A		3
#define PORTM_PIN3_BTIMR2_OUT	4
#define PORTM_PIN3_TIMR0_IN     5
#define PORTM_PIN3_TIMR0_OUT    6
#define PORTM_PIN3_SD_DET		7

#define PORTM_PIN4_GPIO         0
#define PORTM_PIN4_SPI0_MISO	1
#define PORTM_PIN4_UART3_TX		2
#define PORTM_PIN4_PWM0AN       3
#define PORTM_PIN4_BTIMR1_OUT	4
#define PORTM_PIN4_TIMR2_IN     5
#define PORTM_PIN4_TIMR2_OUT    6
#define PORTM_PIN4_LCD_CS		7
#define PORTM_PIN4_LCD_VSYNC	7
#define PORTM_PIN4_SD_CMD		8

#define PORTM_PIN5_GPIO         0
#define PORTM_PIN5_I2C1_SDA		1
#define PORTM_PIN5_SPI0_MOSI	2
#define PORTM_PIN5_UART3_RX     3
#define PORTM_PIN5_PWM0BN       4
#define PORTM_PIN5_BTIMR0_OUT	5
#define PORTM_PIN5_TIMR1_IN     6
#define PORTM_PIN5_TIMR1_OUT    7
#define PORTM_PIN5_SD_D0		8

#define PORTM_PIN6_GPIO         0
#define PORTM_PIN6_I2C1_SCL		1
#define PORTM_PIN6_UART2_RX     2
#define PORTM_PIN6_PWM1AN       3
#define PORTM_PIN6_TIMR0_IN     4
#define PORTM_PIN6_TIMR0_OUT    5
#define PORTM_PIN6_SD_D1		6

#define PORTM_PIN7_GPIO         0
#define PORTM_PIN7_UART2_TX     1

#define PORTM_PIN8_GPIO         0
#define PORTM_PIN8_CAN0_TX		1
#define PORTM_PIN8_LCD_WR		2
#define PORTM_PIN8_LCD_HSYNC	2

#define PORTM_PIN9_GPIO         0
#define PORTM_PIN9_CAN0_RX		1

#define PORTM_PIN10_GPIO        0

#define PORTM_PIN11_GPIO        0
#define PORTM_PIN11_LCD_RS      1
#define PORTM_PIN11_LCD_DEN		1

#define PORTM_PIN12_GPIO        0
#define PORTM_PIN12_SDR_UDQM	1

#define PORTM_PIN13_GPIO        0
#define PORTM_PIN13_PWM_BRK0    1
#define PORTM_PIN13_SDR_CLK		2

#define PORTM_PIN14_GPIO        0
#define PORTM_PIN14_TIMR3_IN    1
#define PORTM_PIN14_TIMR3_OUT   2
#define PORTM_PIN14_SDR_CKE		3

#define PORTM_PIN15_GPIO        0
#define PORTM_PIN15_TIMR2_IN    1
#define PORTM_PIN15_TIMR2_OUT   2
#define PORTM_PIN15_SDR_A12		3

#define PORTN_PIN0_GPIO         0
#define PORTN_PIN0_SPI2_SCLK	1
#define PORTN_PIN0_BTIMR7_OUT	2
#define PORTN_PIN0_TIMR1_IN     3
#define PORTN_PIN0_TIMR1_OUT    4
#define PORTN_PIN0_LCD_RD      	5
#define PORTN_PIN0_LCD_DCLK		5
#define PORTN_PIN0_SD_D2		6

#define PORTN_PIN1_GPIO         0
#define PORTN_PIN1_SPI2_SSEL	1
#define PORTN_PIN1_BTIMR6_OUT	2
#define PORTN_PIN1_TIMR0_IN     3
#define PORTN_PIN1_TIMR0_OUT    4
#define PORTN_PIN1_SD_D3		5

#define PORTN_PIN2_GPIO         0
#define PORTN_PIN2_SDR_A11		1

#define PORTN_PIN3_GPIO         0
#define PORTN_PIN3_UART1_CTS    1
#define PORTN_PIN3_SDR_A9		2

#define PORTN_PIN4_GPIO         0
#define PORTN_PIN4_I2C0_SDA     1
#define PORTN_PIN4_SPI2_MISO	2
#define PORTN_PIN4_UART1_RTS    3
#define PORTN_PIN4_BTIMR5_OUT	4
#define PORTN_PIN4_TIMR3_IN     5
#define PORTN_PIN4_TIMR3_OUT    6
#define PORTN_PIN4_CAN0_TX		7
#define PORTN_PIN4_SD_D4		8
#define PORTN_PIN4_ADC1_TRIG0	9

#define PORTN_PIN5_GPIO         0
#define PORTN_PIN5_I2C0_SCL     1
#define PORTN_PIN5_SPI2_MOSI	2
#define PORTN_PIN5_UART1_RX     3
#define PORTN_PIN5_DMA_TRIG0	4
#define PORTN_PIN5_BTIMR4_OUT	5
#define PORTN_PIN5_TIMR2_IN     6
#define PORTN_PIN5_TIMR2_OUT    7
#define PORTN_PIN5_CAN0_RX		8
#define PORTN_PIN5_LCD_R3		9
#define PORTN_PIN5_SD_D5		10
#define PORTN_PIN5_ADC0_TRIG0	11

#define PORTN_PIN6_GPIO			0
#define PORTN_PIN6_SDR_A8		1

#define PORTN_PIN7_GPIO			0
#define PORTN_PIN7_UART5_RTS	1
#define PORTN_PIN7_SDR_A7		2

#define PORTN_PIN8_GPIO			0
#define PORTN_PIN8_UART5_TX		1
#define PORTN_PIN8_SDR_A6		2

#define PORTN_PIN9_GPIO			0
#define PORTN_PIN9_UART5_CTS	1
#define PORTN_PIN9_SDR_A5		2

#define PORTN_PIN10_GPIO		0
#define PORTN_PIN10_UART5_RX	1
#define PORTN_PIN10_SDR_A4		2

#define PORTN_PIN11_GPIO		0
#define PORTN_PIN11_SDR_A3		1

#define PORTN_PIN12_GPIO		0
#define PORTN_PIN12_SDR_A2		1

#define PORTN_PIN13_GPIO		0
#define PORTN_PIN13_SDR_A1		1

#define PORTN_PIN14_GPIO		0
#define PORTN_PIN14_SDR_A0		1

#define PORTN_PIN15_GPIO		0
#define PORTN_PIN15_SDR_A10		1


#endif //__SWM342_PORT_H__

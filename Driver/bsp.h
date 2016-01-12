#ifndef _BSP_H
#define _BSP_H


#define	LED_NIGHT_FLIGHT_PORT_RCC_ENABLE 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)
#define	LED_NIGHT_FLIGHT_PORT				GPIOC
#define	LED_NIGHT_FLIGHT_PIN				GPIO_Pin_8
#define LED_NIGHT_FLIGHT_OFF  				LED_NIGHT_FLIGHT_PORT->BSRR = LED_NIGHT_FLIGHT_PIN	//BSRR设置使cpu的io口出高电平
#define LED_NIGHT_FLIGHT_ON					LED_NIGHT_FLIGHT_PORT->BRR  = LED_NIGHT_FLIGHT_PIN

/* Exported macro ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* 头字节包序号，末两字节CRC */
extern u8 RX_BUFF[];			 


/* Exported functions ------------------------------------------------------- */
void UART1_SendByte(u16 Data);
void UART1Write(u8* data,u16 len);
void INIT(void);



void RCC_Configuration(void);
void GPIO_Configuration(void);


#endif

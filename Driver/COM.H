#ifndef _COM_H
#define _COM_H

void USART1_Config(USART_TypeDef* USARTx,u32 baud);
void USART2_Config(USART_TypeDef* USARTx,u32 baud);
void USART3_Config(USART_TypeDef* USARTx,u32 baud);
void UART4_Config(USART_TypeDef* USARTx,u32 baud);
void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...);
char *itoa(long value, char *string, int radix);
void USART_Send_Buf(USART_TypeDef* USARTx, u8* buf, u16 len);

#endif




/*
File:	serial.h
Author: Simon Michelsen   -    simon6400()gmail.com
Date:	2017
*/


#ifndef SERIAL_H_
#define SERIAL_H_

/* Defines *******************************************************************/
/* Type Definitions **********************************************************/
/* Constants *****************************************************************/
/* Variables *****************************************************************/
extern char ser_rx_buf[];
extern volatile uint8_t msg_rx_complete;

/* Public Functions **********************************************************/
void Init_USART(void);
void USART_Transmit_Byte( unsigned char data);
char USART_Receive( void );
void SendText(char *text);
void HandleRxByte(char ch);

#endif /* SERIAL_H_ */
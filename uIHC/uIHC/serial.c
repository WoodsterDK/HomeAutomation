/*
File:	serial.c
Author: Simon Michelsen   -    simon6400()gmail.com
*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>       // memcpy
#include <stdio.h>
#include <avr/wdt.h>
#include <inttypes.h>

#include "main.h"
#include "timer.h"
#include "ihc.h"
#include "serial.h"

/* Defines *******************************************************************/
#define MAX_NUM_OF_RX_CHAR 20
#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define UBRRVAL ((FOSC/(BAUD*16UL))-1)

/* Type Definitions **********************************************************/
/* Constants *****************************************************************/
/* Variables *****************************************************************/
char ser_rx_buf[MAX_NUM_OF_RX_CHAR];
volatile uint8_t msg_rx_complete = 0;	// true if message complete, set false by consumer
/* Prototypes ****************************************************************/
/* Implementation ************************************************************/

/**************** USART ***************/
void Init_USART(void)
{
	//unsigned int ubrr = MYUBRR;
	
	// Set baud rate
	UBRR0H = (unsigned char)(UBRRVAL>>8);
	UBRR0L = (unsigned char)UBRRVAL;
	
	//Enable Transmitter and Receiver and Interrupt on receive complete
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
	
	// Set frame format: 8data, 1stop bit
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

// Tager < 2 us
ISR(USART_RX_vect)
{
	static char ch;
	static unsigned char buff_cnt = 0;
//DEBUG_OUT_CLR();
	
	ch = (char)UDR0;

	if(msg_rx_complete == 0)
	{
		if(ch == '\n' || ch == '\r' || buff_cnt > (MAX_NUM_OF_RX_CHAR-1))
		{
			ser_rx_buf[buff_cnt] = 0x00;
			buff_cnt = 0;
			msg_rx_complete = 1;
		}
		else
		{
			ser_rx_buf[buff_cnt++] = ch;
		}
	}

//DEBUG_OUT_SET();
	
}

void USART_Transmit_Byte( unsigned char data)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

void SendText(char *text)
{
	int i = 0;
	while(text[i] != 0x00)
	{
		USART_Transmit_Byte(text[i]);
		i++;
	}
}

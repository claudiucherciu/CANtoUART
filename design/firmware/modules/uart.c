#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"


void uart_init(const uint32_t sysclock, enum uart_baud_t uart_baud)
{ 
	uint16_t baud_setting = (sysclock / (uart_baud * 32.0)) - 1;
	LINCR = (1 << LSWRES);	/* reset the LIN/UART */
	LINBRRH = baud_setting >> 8;
	LINBRRL = baud_setting;
	/* enable LIN/UART, UART cmds, Rx, Tx */
	LINCR = (1 << LENA) | (1 << LCMD2) | (1 << LCMD1) | (1 << LCMD0); 
	UART_PORT |= (1 << UART_RX); /* pullup rx input pin */		
}


err_t uart_send(const uint8_t data) 
{
	static uint8_t busy = 0;
	volatile uint32_t timeout;
		
	/* Ensure that function is not already called */
	if (busy)
	{
		return err_FAIL;
	}
	/* Ensure that uart is free */
	if (LINSIR & (1 << LBUSY))
	{
		return err_FAIL;
	}
	busy = 1;
	timeout = 0;
	LINDAT = data; /* Send data */
	/* Wait for sending to complete or timeout */
	while ((LINSIR & (1 << LTXOK)) == 0)
	{
		timeout++;
		if (timeout > UART_TIMEOUT)
		{
			busy = 0;
			return err_FAIL;
		}
	}
	busy = 0;
	return err_OK;
}


err_t uart_read(uint8_t* data)
{
	static uint8_t busy = 0;
	volatile uint32_t timeout;
	
	if (busy) 
	{
		return err_FAIL;
	}	
	busy = 1;
	timeout = 0;	
	/*  Check if data is available */
	while ((LINSIR & (1 << LRXOK)) == 0)	
	{
		timeout++;
		if (timeout > UART_TIMEOUT)
		{
			busy = 0;
			return err_FAIL;
		}		
	}
	*data = LINDAT; /* return register value */	
	busy = 0;
	return err_OK;
}


void uart_read_array(uint8_t* array, const uint8_t length)
{
	uint8_t i = 0;

	while (i < length)
	{
		uart_read(&array[i]);
		i++;
	}
}


void uart_print_string(const char string[]) 
{
	uint8_t i = 0;
	
	uart_send('\r');
	uart_send('\n');
	while (string[i]) /* Strings are terminated with NULL character */
	{
		uart_send(string[i++]);
	}
}


void uart_read_string(char string[], const uint8_t max_length) 
{
	uint8_t response;
	uint8_t i = 0;
	
	while (i < (max_length - 1)) 
	{                   
		uart_read(&response);
		if (response == '\r')
		{                     
			break;
		}
		else 
		{
			string[i] = response; /* add in a letter */
			i++;
		}
	}
	string[i] = 0; /* terminal NULL character */
}
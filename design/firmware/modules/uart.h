/*! 
\file uart.h
\brief		Functions to initialize, send, and receive over UART.
\author		Artur Scholz
\date		2015-12-02
\copyright	GNU Public License.
*/ 
#ifndef UART_H
#define UART_H


/* Constants & Macros ********************************************************/


/*! Max number of retry cycles before UART send/receive timeout */
#define UART_TIMEOUT	300000UL	
#define UART_PORT		PORTD		/*!< UART port */
#define UART_RX			PD4			/*!< UART RX pin */
#define UART_TX			PD3			/*!< UART TX pin */


/* Type Definitions **********************************************************/


/*!
\enum		err_t
\brief		Error codes 
*/
#ifndef ERR_T
#define ERR_T
typedef enum 
{
   err_OK	= 0,
   err_FAIL	= -1
} err_t;
#endif


/*!
\enum		uart_baud_t
\brief		Baud rate of UART
*/
enum uart_baud_t 
{
   uart_baud_1k2 = 1200,
   uart_baud_4k8 = 4800,
   uart_baud_9k6 = 9600
};


/* Global Variables **********************************************************/


uint8_t uart_buffer; /*!< A generic buffer for UART data byte */


/* Functions *****************************************************************/


/*!
\brief		Initialize UART interface
\param		sysclock_Hz		System clock frequency
\param		uart_baud		Desired baud rate
\details	Calculates bit-clock multiplier and configures the hardware UART
*/
void uart_init(const uint32_t sysclock, const uint16_t uart_baud);


/*!
\brief		Send byte over UART
\param		data		Byte to send
\details	This function sends a data byte over UART and returns when byte
			has been sent.
*/
err_t uart_send(const uint8_t data);


/*!
\brief		Receive byte over UART
\param		data		Pointer to buffer for data read
\return		err_OK if success, err_FAIL if not
\details	This function polls the UART for a received data byte.
*/
err_t uart_read(uint8_t* data);


/*!
\brief		Receive byte array over UART
\param		array		Pointer to array for storing of received data
\param		length		Number of bytes to read
\details	This function simply calls #uart_read for #length times.
*/
void uart_read_array(uint8_t* array, const uint8_t length);


/*!
\brief		Utility function to transmit an entire string
\param		string		Pointer to string array
\details	This function simply calls #uart_send until end of string 
			(defined by NULL character) is reached. 
*/
void uart_print_string(const char string[]);
 
 
/*!
\brief		Utility function to receive an entire string
\param		string		Pointer to array for storing the string
\param		max_length	The maximum lenght of the string to store
\details	This function reads and stores the bytes received via
			UART until either the EOL character \c '\r' has been
			detected or #max_length is reached. A NULL character
			is then appened to the stored byte array to make it a
			proper string.
*/     	 
void uart_read_string(char string[], const uint8_t max_length);


#endif /* UART_H */

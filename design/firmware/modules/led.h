/*! 
\file led.h
\brief		Functions to blink a LED.
\author		Artur Scholz
\date		2016-02-10
\copyright	GNU Public License. 
*/ 
#ifndef LED_H_
#define LED_H_


/* Constants & Macros ********************************************************/


#define LED_PORT	PORTB		/*!< LED port */
#define LED_DDR		DDRB		/*!< LED data direction */
#define LED0		PB0			/*!< LED pin */


/* Functions *****************************************************************/


/*!
\brief		Initialize LED interface
\param		led		The pin to which the LED is connected
\details	This function sets the selected pin of the microntroller to
			output mode. 
*/
void led_init(uint8_t led);


/*!
\brief		Lightens the LED
\param		led		The pin to which the LED is connected
*/
void led_set(uint8_t led);


/*!
\brief		Clears the LED
\param		led		The pin to which the LED is connected
*/
void led_clr(uint8_t led);


/*!
\brief		Toggles the current state of the LED
\param		led		The pin to which the LED is connected 
*/
void led_toggle(uint8_t led);


#endif /* LED_H_ */
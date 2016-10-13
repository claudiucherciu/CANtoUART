/*! 
\file gps.h
\brief		Functions to interface the GPS receiver.
\author		Artur Scholz
\date		2016-03-08
\copyright	GNU Public License. 
*/ 
#ifndef GPS_H_
#define GPS_H_


/* Constants & Macros ********************************************************/


#define GPS_RCV_PORT		PORTD	/*!< GPS receiver port */
#define GPS_RCV_DDR			DDRD	/*!< GPS receiver data direction */
#define GPS_RCV_PIN_PWR		PD1		/*!< GPS receiver power pin */


/* Functions *****************************************************************/


/*!
\brief		Initialize GPS receiver
\details	Define microcontroller output pin and set GPS to off.
*/
void gps_init(void);


/*!
\brief		Power off GPS receiver
*/
void gps_off(void);


/*!
\brief		Power on GPS receiver
*/
void gps_on(void);


#endif /* GPS_H_ */
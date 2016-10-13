#include <avr/io.h>
#include "gps.h"


void gps_init(void)
{
	/* Define pins as output pins */
	GPS_RCV_DDR |= (1 << GPS_RCV_PIN_PWR);	
	/* Set pin levels such that gps is off */
	GPS_RCV_PORT &= ~(1 << GPS_RCV_PIN_PWR);
}


void gps_off(void)
{
	/* Set pin levels such that gps is off */
	GPS_RCV_PORT &= ~(1 << GPS_RCV_PIN_PWR);
}


void gps_on(void)
{
	/* Set pin levels such that gps is on */
	GPS_RCV_PORT |= (1 << GPS_RCV_PIN_PWR);
}

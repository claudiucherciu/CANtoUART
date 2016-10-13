#include <avr/io.h>
#include "led.h"


void led_init(uint8_t led) 
{
	LED_DDR |= (1 << led);
}


void led_clr(uint8_t led) 
{
	LED_PORT &= ~(1 << led);
}


void led_set(uint8_t led) 
{
	LED_PORT |= (1 << led);
}


void led_toggle(uint8_t led) 
{
	LED_PORT ^= (1 << led);
}
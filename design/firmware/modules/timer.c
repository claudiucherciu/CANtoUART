#include <avr/io.h>
#include "timer.h"


void timer0_init(void)
{
	timer0_ms	= 0;
	timer0_sec	= 0;
	TCCR0A |= (1 << WGM01); /* CTC mode */
	TCCR0B |= (1 << CS02) | (1 << CS00); /* Clock at CPU/1024 */ 
	OCR0A = (1000UL * TIMER0_TICK_MS) / 128; /* (1000/128) = clock ticks per ms */
}


void timer0_isr_start(void)
{
	TIMSK0 |= (1 << OCIE0A); /* Enable timer compare match */
}


void timer0_isr_stop(void)
{
	TIMSK0 &= ~(1 << OCIE0A); /* Disable timer compare match */
}


void timer1_init(void)
{
	TCCR1B |= (1 << WGM12); /* CTC mode */
	TCCR1B |= (1 << CS12) | (1 << CS10); /* Clock at CPU/1024 */ 
	OCR1A = (1000UL * TIMER1_TICK_MS) / 128; /* (1000/128) = clock ticks per ms */
}


void timer1_isr_start(void)
{
	TIMSK1 |= (1 << OCIE1A); /* Enable timer compare match */
}


void timer1_isr_stop(void)
{
	TIMSK1 &= ~(1 << OCIE1A); /* Disable timer compare match */
}

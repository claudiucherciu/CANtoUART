/*! 
\file timer.h
\brief		Functions to initialize, send, and receive over UART.
\author		Artur Scholz
\date		2015-12-02
\copyright	GNU Public License.
*/
#ifndef TIMER_H
#define TIMER_H


/* Constants & Macros ********************************************************/


#define	TIMER0_TICK_MS	10 /*!< Timer triggers every 0.01 second */
#define	TIMER0_TICK_US	(1000UL * TIMER0_TICK_MS)
#define	TIMER1_TICK_MS	100 /*!< Timer triggers every 0.1 second */
#define	TIMER1_TICK_US	(1000UL * TIMER1_TICK_MS)


/* Global Variables **********************************************************/


uint32_t timer0_ms;
uint32_t timer0_sec;


/* Functions *****************************************************************/


/*!
\brief		Initialize timer 0
\details	The clock source for timer 0 is configured to trigger 
			an interrupt each #TIMER0_TICK_MS milliseconds. The 
			timer must be started after it has been initialized.
*/
void timer0_init(void);


/*!
\brief		Enable timer 0 interrupt
\details	Timer is running and triggers an interrupt at overflow.
*/
void timer0_isr_start(void);


/*!
\brief		Disable timer 0 interrupt
\details	Timer is running but does not trigger an interrupt at overflow.
*/
void timer0_isr_stop(void);


/*!
\brief		Initialize timer 1
\details	The clock source for timer 1 is configured to trigger 
			an interrupt each #TIMER1_TICK_MS milliseconds. The 
			timer must be started after it has been initialized.
*/
void timer1_init(void);


/*!
\brief		Enable timer 1 interrupt
\details	Timer is running and triggers an interrupt at overflow.
*/
void timer1_isr_start(void);


/*!
\brief		Disable timer 1 interrupt
\details	Timer is running but does not trigger an interrupt at overflow.
*/
void timer1_isr_stop(void);


#endif /* TIMER_H */

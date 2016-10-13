/*! 
\file main.c
\brief		CANopen Slave
\author		Artur Scholz
\date		2016-03-18
\copyright	GNU Public License.
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <stdio.h>
#define F_CPU		8000000UL	/* 8 MHz */
#include "modules/timer.h"
#include "modules/uart.h"
#include "modules/led.h"
#include "modules/can.h"
#include "modules/gps.h"
#include "canopen/canopen.h"
#include "canopen/canopen_specific.h"
#include "applications/nmea.h"
#include "applications/queue.h"


/* Check if if debug messages shall be sent to uart */
#ifdef DEBUG
	#define debug_output(message)	uart_print_string(message)
#else
	#define debug_output(message)	{}	
#endif


/* Check if if compiler optimizations are enabled */
#ifdef __OPTIMIZE__
	#warning "Compiler optimizations enabled; code may not work as designed"
#endif


int main()
{	
	clock_prescale_set(clock_div_1); /* Set system clock to 8 MHz */
	uart_init(F_CPU, uart_baud_9k6);	
	led_init(LED0);
	led_set(LED0);	
	timer1_init();
	timer1_isr_start();	
	gps_init();	
	canopen_init(F_CPU, can_baud_1M);		
	sei(); /* Enable interrupts */
	debug_output("info: init complete");
	
	for (;;)
	{		
		/* Check if a change of opmode was requested via telecommand */
		if (node_gps_opmode_next != node_gps_opmode_current)
		{
			switch (node_gps_opmode_next)
			{
			case node_gps_opmode_STBY:
				gps_off();
				debug_output("info: now standby");
				node_gps_tm_stat.opmode = node_gps_opmode_STBY;	
				node_gps_opmode_current = node_gps_opmode_next;						
				break;
			case node_gps_opmode_ACTIVE:
				gps_on();
				debug_output("info: now active");
				node_gps_tm_stat.opmode = node_gps_opmode_ACTIVE;
				node_gps_opmode_current = node_gps_opmode_next;
				break;
			default:
				debug_output("warning: opmode not valid");
				node_gps_opmode_next = node_gps_opmode_current;
				break;
			}	
		}
		
		/* Check if SYNC was received */
		if (node_gps_sync_received)
		{
			debug_output("info: SYNC");
			node_gps_sync_received = 0;	
			canopen_send_tpdo(CANOPEN_TPDO_TM_STAT_INDEX);
			canopen_send_tpdo(CANOPEN_TPDO_TM_UTC_INDEX);
			canopen_send_tpdo(CANOPEN_TPDO_TM_POS_INDEX);		
			canopen_send_tpdo(CANOPEN_TPDO_TM_ALT_INDEX);	
		}
		
		/* Check if a TC request for telemetry is in the queue */		
		while (queue_is_emtpy() == FALSE)
		{			
			queue_remove(&queue_buffer);							
			switch (queue_buffer)
			{
			case NODE_GPS_TM_STAT_ID:
				debug_output("info: STAT");	
				canopen_send_tpdo(CANOPEN_TPDO_TM_STAT_INDEX);
				break;
			case NODE_GPS_TM_UTC_ID:
				debug_output("info: UTC");
				canopen_send_tpdo(CANOPEN_TPDO_TM_UTC_INDEX);
				break;								
			case NODE_GPS_TM_POS_ID:
				debug_output("info: POS");
				canopen_send_tpdo(CANOPEN_TPDO_TM_POS_INDEX);
				break;						
			case NODE_GPS_TM_ALT_ID:
				debug_output("info: ALT");
				canopen_send_tpdo(CANOPEN_TPDO_TM_ALT_INDEX);
				break;								
			}						
		}
		
		if (node_gps_opmode_current == node_gps_opmode_ACTIVE)
		{
			uart_buffer = 0;
			if (uart_read(&uart_buffer) == err_OK)
			{
				if (uart_buffer == NMEA_MSG_START) /* Check if start of sequence found */
				{
					/* Read the message identifier */
					uart_read_array((uint8_t*)(&nmea.msg_buffer[0]), NMEA_MSG_ID_LENGTH); 
					/* Check if this is a GGA message */
					if (nmea_check_msg(&nmea) == err_OK)
					{
						/* Read the rest of the message */
						uart_read_string(&nmea.msg_buffer[NMEA_MSG_ID_LENGTH], 
													NMEA_MSG_LENGTH - NMEA_MSG_ID_LENGTH);
						/* Parse the message content and check for its validity */							
						nmea_parse(&nmea); 
						node_gps_tm_stat.valid		= nmea.is_valid;
				
						/* the following outputs are for test purposes */		
						if (nmea.is_valid) 
						{						
							/* Update the canopen process data block variables */
							node_gps_tm_utc.hour		= nmea.hour;
							node_gps_tm_utc.min			= nmea.min;
							node_gps_tm_utc.sec			= nmea.sec;
							node_gps_tm_pos.lat			= nmea.lat;
							node_gps_tm_pos.lat_sub_1	= nmea.lat_sub >> 8;
							node_gps_tm_pos.lat_sub_0	= nmea.lat_sub & 0xFF;
							node_gps_tm_pos.lon_1		= nmea.lon >> 8;
							node_gps_tm_pos.lon_0		= nmea.lon & 0xFF;
							node_gps_tm_pos.lon_sub_1	= nmea.lon_sub >> 8;
							node_gps_tm_pos.lon_sub_0	= nmea.lon_sub & 0xFF;		
							node_gps_tm_alt.alt_1		= nmea.alt >> 8;
							node_gps_tm_alt.alt_0		= nmea.alt & 0xFF;
							node_gps_tm_alt.alt_sub		= nmea.alt_sub;	
						
							/* Debug output */
							char buff[80];		
							sprintf(buff, "Time %02d:%02d:%02d", nmea.hour, nmea.min, nmea.sec);			
							debug_output(buff);				
							sprintf(buff, "Lat %d.%d", nmea.lat, nmea.lat_sub);
							debug_output(buff);
							sprintf(buff, "Lon %d.%d", nmea.lon, nmea.lon_sub);
							debug_output(buff);	
							sprintf(buff, "Alt %d.%d", nmea.alt, nmea.alt_sub);			
							debug_output(buff);													
						}
						else
						{
							debug_output(".");		
						}					
					}
				}
			}
		}
	}
	return 0;
}


/*! Timer1 interrupt service routine - Handle redundancy management */
ISR(TIMER1_COMPA_vect) 
{ 
	static uint8_t* ntoggle = &canopen_redundancy_management.ntoggle;
	static uint8_t* ctoggle = &canopen_redundancy_management.ctoggle;
	
	canopen_consumer_heartbeat_counter_ms += TIMER1_TICK_MS;
	
	/* Check if Heartbeat is enabled */
	if (canopen_consumer_heartbeat_time > 0) 
	{
		/* Check if Heartbeat has expired */
		if (canopen_consumer_heartbeat_counter_ms 
				>= canopen_consumer_heartbeat_time) 
		{
			canopen_consumer_heartbeat_counter_ms = 0;
			canopen_consumer_heartbeat_ttoggle_counter++;
			if (canopen_consumer_heartbeat_ttoggle_counter 
					>= canopen_redundancy_management.ttoggle)
			{
				/* Switch the bus if Ntoogle limit has not yet been reached */
				if (*ctoggle < *ntoggle)
				{
					canopen_bus_switchover(); 
					*ctoggle = *ctoggle + 1;
					debug_output("info: bus switched");
					node_gps_tm_stat.bus_switch_cnt++;
				}
				else
				{
					debug_output("warning: switch limit");
				}
				canopen_consumer_heartbeat_ttoggle_counter = 0;
			}	
		}
	}
}


/*! CAN interrupt service routine */
ISR(CAN_INT_vect) 
{ 
	static uint8_t* ctoggle = &canopen_redundancy_management.ctoggle;
	static struct can_frame_t can_frame;
	uint8_t page_saved;	
	uint8_t i;

	/* RX interrupt, broadcast frames from master */
	if (CANSIT2 & (1 << CAN_MOB_RX_MASTER)) 	
	{
		page_saved = CANPAGE;
		CANPAGE = CAN_MOB_RX_MASTER << MOBNB0;;
		can_frame.id = (CANIDT1 << 3) | (CANIDT2 >> 5);		
		can_frame.dlc = CANCDMOB & 0x0F; 
		for (i = 0; i < can_frame.dlc; i++) /* Read data */
		{
			can_frame.data[i] = CANMSG;
		}
		CANSTMOB &= ~(1 << RXOK); /* Clear rx interrupt flag */	
		CANCDMOB = 1 << CONMOB1; /* Enable reception again */	
		CANPAGE = page_saved;
		
		switch (can_frame.id)
		{
		case canopen_cob_id_HEARTBEAT:
			canopen_consumer_heartbeat_counter_ms = 0;
			canopen_consumer_heartbeat_ttoggle_counter = 0;
			*ctoggle = 0;
			if (canopen_get_state() == canopen_state_PRE_OPERATIONAL) 
			{
				canopen_set_state(canopen_state_OPERATIONAL);
			}
			led_toggle(LED0);
			break;
		case canopen_cob_id_SYNC:
			node_gps_sync_received = 1;
			break;
		}	
	}
	
	/* RX interrupt, dedicated frames (telecommands) from master */		
	if (CANSIT2 & (1 << CAN_MOB_RX_NODE)) 
	{
		page_saved = CANPAGE;
		CANPAGE = CAN_MOB_RX_NODE << MOBNB0;;
		can_frame.id = (CANIDT1 << 3) | (CANIDT2 >> 5);		
		can_frame.dlc = CANCDMOB & 0x0F; 
		for (i = 0; i < can_frame.dlc; i++) /* read data */
		{
			can_frame.data[i] = CANMSG;
		}
		CANSTMOB &= ~(1 << RXOK); /* Clear rx interrupt flag */	
		CANCDMOB = 1 << CONMOB1; /* Enable reception again */	
		CANPAGE = page_saved;
		
		if (canopen_receive_rpdo(CANOPEN_RPDO_TC_OPMODE_INDEX, &can_frame) == err_OK)
		{
			node_gps_opmode_next = node_gps_tc_opmode.opmode; 
		}
		else if (canopen_receive_rpdo(CANOPEN_RPDO_TC_REQUEST_TM_INDEX, &can_frame) == err_OK)
		{
			/* Add the tm request in the queue */
			queue_add(node_gps_tc_request_tm.request_tm_id); 
		}		
	}
}

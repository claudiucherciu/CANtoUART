#include <avr/io.h>
#include <assert.h>
#include "can.h"


void can_init(const uint32_t sysclock_Hz, const enum can_baud_t can_baud) 
{
	uint8_t i; /* temporary MOb register set selection number */

	/* Initialize all transceiver interfaces */
	can_trx_init();
	/* Reset the CAN controller hardware and general registers */
	CANGCON = (1 << SWRES); 
	/* Setup the CAN baud timing for 8 MHz clock */
	assert (sysclock_Hz == 8000000UL);
	assert ((can_baud == can_baud_500K) || (can_baud == can_baud_1M));
	if (can_baud == can_baud_500K) /* 500 kbps */
	{
		CANBT1 = 0x02; 
		CANBT2 = 0x04; 
		CANBT3 = 0x13; 		
	}
	else if (can_baud == can_baud_1M) /* 1 Mbps */
	{
		CANBT1 = 0x00; 
		CANBT2 = 0x04; 
		CANBT3 = 0x12; 
	}
	for (i = 0; i < CAN_MOB_MAX; i++) 
	{
		CANPAGE = (i << MOBNB0); /* Set the MOb page number for each MOb */
		CANCDMOB = 0x00; /* Set each MOb to disabled */
		CANSTMOB = 0x00; /* Clear all MOb status flags */
	}
}


void can_set_mode(const enum can_mode_t can_mode)
{	
	switch (can_mode)
	{
	case can_mode_STOP:
		CANGCON &= ~(1 << ENASTB); /* Disable CAN */
		can_trx_set_mode(can_trx_mode_STBY); 
		break;
	case can_mode_START:
		can_trx_set_mode(can_trx_mode_ON);
		CANGCON |= (1 << ENASTB); /* Enable CAN */
		break;
	default:
		assert (0); /* Error: invalid mode */	
	}
}


void can_configure_receive(const uint8_t mob, 
							const struct can_frame_t* can_frame)
{
	uint8_t page_saved;
	
	assert (mob < CAN_MOB_MAX);
	/* Enable reception of CAN frames from the network master */		
	page_saved = CANPAGE;		
	CANPAGE = mob << MOBNB0; /* use MOb for Rx and auto-increment data index */
	CANSTMOB = 0x00; /* clear all MOb status flags */
	CANIDM4 = (1 << RTRMSK) | (1 << IDEMSK); /* set mask for rtr and ide */
	CANIDM3 = 0x00;
	CANIDM2 = (can_frame->id_mask & 0x07) << 5;
	CANIDM1 = can_frame->id_mask >> 3;	
	CANIDT4 = 0x00; /* clear rtr and ide bits */
	CANIDT3 = 0x00;
	CANIDT2 = (can_frame->id & 0x07) << 5;
	CANIDT1 = can_frame->id >> 3;
	CANCDMOB = (1 << CONMOB1) | (can_frame->dlc << DLC0); /* enable reception */	
	CANPAGE = page_saved;	
    CANIE1 = 0; /* compatibility with future chips */ 	
	CANIE2 |= (1 << mob); /* Enable receive MOb interrupts */	
    CANGIE = (1 << ENIT) | (1 << ENRX); /* Enable interrupts: global, receive */		
}


err_t can_send(const uint8_t mob, const struct can_frame_t* can_frame)
{	
	static uint8_t busy = 0;
	uint8_t page_saved;
	uint8_t i;

	assert (mob < CAN_MOB_MAX);
	/* Check if function has been already called */	
	if (busy) 
	{
		return err_FAIL;
	}
	else
	{
		busy = 1;
	}	
	page_saved = CANPAGE;		
	CANPAGE = mob << MOBNB0; /* use MOb for Tx and auto-increment data index */	
	CANSTMOB = 0x00; /* Clear MOb status flags */
	for (i = 0; i < can_frame->dlc; i++)
	{
		CANMSG = can_frame->data[i];	
	}
	CANIDT2 = (can_frame->id & 0x07) << 5;
	CANIDT1 = can_frame->id >> 3;
	CANCDMOB = (1 << CONMOB0) | (can_frame->dlc); /* Set Tx bit and dlc */
	while (CANSTMOB == 0); /* Wait for Tx result */
	if (CANSTMOB & (1 << TXOK))
	{
		CANCDMOB = 0x00; /* Disable transmit */
		CANSTMOB = 0x00; /* Clear MOb status flags */	
		CANPAGE = page_saved;
		busy = 0;
		return err_OK;
	}
	else
	{
		CANCDMOB = 0x00; /* Disable transmit */
		CANSTMOB = 0x00; /* Clear MOb status flags */	
		CANPAGE = page_saved;
		busy = 0;
		return err_FAIL;
	}
}


/*****************************************************************************/


void can_trx_init(void)
{
	/* Define the pins as output pins */
	CAN_TRX_DDR |= (1 << CAN_TRX1_PIN_STBY);
	CAN_TRX_DDR |= (1 << CAN_TRX2_PIN_STBY);
	CAN_TRX_DDR |= (1 << CAN_TRX_PIN_MUX);
	
	/* Set pin levels such that both trx are in stby */
	CAN_TRX_PORT |= (1 << CAN_TRX1_PIN_STBY);
	CAN_TRX_PORT |= (1 << CAN_TRX2_PIN_STBY);
}


void can_trx_select_active(const enum can_trx_t can_trx)
{
	can_trx_active = can_trx; 
}


void can_trx_set_mode(const enum can_trx_mode_t can_trx_mode)
{	
	switch (can_trx_mode)
	{
	case can_trx_mode_ON:
		/* Activate the selected trx, and put the other in stby. */
		/* Also ensure that mux selects the active trx. */
		if (can_trx_active == can_TRX1)
		{
			CAN_TRX_PORT |=  (1 << CAN_TRX2_PIN_STBY);
			CAN_TRX_PORT &= ~(1 << CAN_TRX1_PIN_STBY);
			CAN_TRX_PORT |=  (1 << CAN_TRX_PIN_MUX);
		}
		else
		{
			CAN_TRX_PORT |=  (1 << CAN_TRX1_PIN_STBY);
			CAN_TRX_PORT &= ~(1 << CAN_TRX2_PIN_STBY);
			CAN_TRX_PORT &= ~(1 << CAN_TRX_PIN_MUX);	
		}
		break;
	case can_trx_mode_STBY:
		/* Put both in standby */
		CAN_TRX_PORT |=  (1 << CAN_TRX1_PIN_STBY);
		CAN_TRX_PORT |=  (1 << CAN_TRX2_PIN_STBY);
		/* Ensure that mux selects the active trx */
		if (can_trx_active == can_TRX1)
		{
			CAN_TRX_PORT |=  (1 << CAN_TRX_PIN_MUX);
		}
		else
		{
			CAN_TRX_PORT &= ~(1 << CAN_TRX_PIN_MUX);	
		}
		break;
	default:
		assert (0); /* Error: mode not supported */	
	}	
}


void can_trx_toogle_active(void)
{
	can_trx_set_mode(can_trx_mode_STBY); /* Put current bus in standby */
	
	if (can_trx_active == can_TRX1)
	{
		can_trx_select_active(can_TRX2);
	}
	else
	{
		can_trx_select_active(can_TRX1);
	}
}
#include <avr/io.h>
#include <assert.h>
#include "../modules/can.h"
#include "canopen_specific.h"
#include "canopen.h"


void canopen_od_init(void)
{		
	uint16_t i;
	uint16_t j;
	
	/* Initialise all indexes with zero */
	for (i = 0; i < CANOPEN_NUMBER_OF_OD_OBJECTS; i++)
	{
		canopen_od[i].index = 0;
	}	

	i = canopen_find_free_index();
	canopen_od[i].index = CANOPEN_COB_ID_SYNCH_INDEX;
	canopen_od[i].object = &canopen_cob_id_synch;	
	canopen_cob_id_synch = CANOPEN_COB_ID_SYNCH;
	
	i = canopen_find_free_index(); 
	canopen_od[i].index = CANOPEN_COMMUNICATION_CYCLE_PERIOD_INDEX;
	canopen_od[i].object = &canopen_communication_cycle_period;	
	canopen_communication_cycle_period = CANOPEN_COMMUNICATION_CYCLE_PERIOD;	
	
	i = canopen_find_free_index(); 
	canopen_od[i].index = CANOPEN_CONSUMER_HEARTBEAT_TIME_INDEX;
	canopen_od[i].object = &canopen_consumer_heartbeat_time;	
	canopen_consumer_heartbeat_time = CANOPEN_CONSUMER_HEARTBEAT_TIME;		

	i = canopen_find_free_index(); 
	canopen_od[i].index = CANOPEN_PRODUCER_HEARTBEAT_TIME_INDEX;
	canopen_od[i].object = &canopen_producer_heartbeat_time;	
	canopen_producer_heartbeat_time = CANOPEN_PRODUCER_HEARTBEAT_TIME;

	i = canopen_find_free_index(); 
	canopen_od[i].index = CANOPEN_REDUNDANCY_MANAGEMENT_INDEX;
	canopen_od[i].object = &canopen_redundancy_management;	
	canopen_redundancy_management.bdefault = CANOPEN_REDUNDANCY_MANAGEMENT_BDEFAULT;
	canopen_redundancy_management.ttoggle = CANOPEN_REDUNDANCY_MANAGEMENT_TTOGGLE;	
	canopen_redundancy_management.ntoggle = CANOPEN_REDUNDANCY_MANAGEMENT_NTOGGLE;	
	canopen_redundancy_management.ctoggle = 0;	
	
	for (j = 0; j < CANOPEN_NUMBER_OF_RPDO; j++)
	{
		i = canopen_find_free_index();
		canopen_od[i].index = (CANOPEN_RPDO_INDEX + j);
		canopen_od[i].object = &canopen_rpdo[j];	
		i = canopen_find_free_index(); 
		canopen_od[i].index = (CANOPEN_RPDO_MAP_INDEX + j);
		canopen_od[i].object = &canopen_rpdo_map[j];	
	}
	
	for (j = 0; j < CANOPEN_NUMBER_OF_TPDO; j++)
	{
		i = canopen_find_free_index(); 
		canopen_od[i].index = (CANOPEN_TPDO_INDEX + j);
		canopen_od[i].object = &canopen_tpdo[j];	
		i = canopen_find_free_index(); 
		canopen_od[i].index = (CANOPEN_TPDO_MAP_INDEX + j);
		canopen_od[i].object = &canopen_tpdo_map[j];	
	}	
}


void canopen_init(const uint32_t sysclock_Hz, const uint16_t can_baud_kHz)
{	
	canopen_set_state(canopen_state_BOOTUP);
	/* Initialize the generic and specific od entries */
	canopen_od_init();
	canopen_od_init_specific(); 
	/* Select default bootup transceiver */
	can_trx_select_active(canopen_redundancy_management.bdefault); 
	can_init(sysclock_Hz, can_baud_kHz); /* Initialize the CAN bus interface */
	canopen_init_specific(); /* Initialize the specifics of CAN bus interface */
	can_set_mode(can_mode_START); /* Make transceiver operational */
	canopen_set_state(canopen_state_PRE_OPERATIONAL);
}


void canopen_set_state(const enum canopen_state_t canopen_state)
{
	switch (canopen_state)
	{
		case canopen_state_BOOTUP:
			canopen_current_state = canopen_state_BOOTUP;
			break;
		case canopen_state_PRE_OPERATIONAL:
			canopen_current_state = canopen_state_PRE_OPERATIONAL;
			break;
		case canopen_state_OPERATIONAL:
			canopen_current_state = canopen_state_OPERATIONAL;
			break;
		case canopen_state_STOPPED:
			canopen_current_state = canopen_state_STOPPED;
			break;
		default:
			assert (0); /* Error: state not supported */
	}
}


enum canopen_state_t canopen_get_state(void)
{
	return canopen_current_state;
}


uint16_t canopen_find_index(const uint16_t index)
{
	uint16_t i;

	for (i = 0; i < CANOPEN_NUMBER_OF_OD_OBJECTS; i++)
	{
		if (canopen_od[i].index == index)
		{
			return i;
		}
	}	
	assert (0); /* Error: requested index is not defined */	
}


uint16_t canopen_find_free_index(void)
{
	return canopen_find_index(0);	
}


void canopen_bus_switchover(void)
{
	can_set_mode(can_mode_STOP);
	can_trx_toogle_active();
	can_set_mode(can_mode_START);	
#if CANOPEN_IS_MASTER	
	canopen_current_state = canopen_state_OPERATIONAL;
#else
	canopen_current_state = canopen_state_PRE_OPERATIONAL;
#endif
}


err_t canopen_send_tpdo(const uint16_t tpdo_index)
{
	struct can_frame_t can_frame;	
	uint16_t tpdo_map;
	uint32_t map;
	uint16_t index;
	uint8_t subindex;		
	uint16_t i;
	uint16_t j;	

	/* Check that pdo_index is within valid range for TPDO*/
	assert ((tpdo_index >= CANOPEN_TPDO_INDEX) 
			&& (tpdo_index < CANOPEN_TPDO_MAP_INDEX));
	/* Check that current canopen state allows sending of tpdo */
	if (canopen_get_state() != canopen_state_OPERATIONAL)
	{
		return err_FAIL;
	}
		
	i = canopen_find_index(tpdo_index);
	/* Set COB-ID */
	can_frame.id = ((struct canopen_pdo_t*)canopen_od[i].object)->cob_id;
	/* Set DLC */
	tpdo_map = canopen_find_index(tpdo_index + 0x200);
	can_frame.dlc = ((struct canopen_pdo_map_t *)
						canopen_od[tpdo_map].object)->number_of_entries;
	
	for (i = 0; i < can_frame.dlc; i++)
	{
		map = ((struct canopen_pdo_map_t *)canopen_od[tpdo_map].object)->map[i];	
		index = (map >> 16);
		subindex = (map >> 8) & 0xFF;
		
		j = canopen_find_index(index);
		can_frame.data[i] = ((struct canopen_pdb_t *) 
								canopen_od[j].object)->var[subindex];
	}

	can_send(CAN_MOB_TX, &can_frame);	
	return err_OK;
}


err_t canopen_receive_rpdo(const uint16_t rpdo_index, const struct can_frame_t* can_frame)
{
	uint16_t rpdo_map;
	uint32_t map;
	uint16_t index;
	uint16_t i;
	uint16_t j;
	
	/* Check that pdo_index is within valid range for TPDO*/
	assert ((rpdo_index >= CANOPEN_RPDO_INDEX) 
			&& (rpdo_index < CANOPEN_RPDO_MAP_INDEX));
	/* Check that current canopen state allows sending of tpdo */	
	if (canopen_get_state() != canopen_state_OPERATIONAL)
	{
		return err_FAIL;
	}
	i = canopen_find_index(rpdo_index);
	/* Check COB-ID */
	if (can_frame->id != ((struct canopen_pdo_t*)canopen_od[i].object)->cob_id)
	{
		return err_FAIL;
	}
	/* Get the RPDO map object */
	rpdo_map = canopen_find_index(rpdo_index + 0x200);
	/* Get the map to the TC id value in the process data block */
	map = ((struct canopen_pdo_map_t *)canopen_od[rpdo_map].object)->map[0];
	index = (map >> 16);
	/* Find the process data block object */
	j = canopen_find_index(index);
	/* Check if TC id value matches */
	if (can_frame->data[0] != ((struct canopen_pdb_t *)
						canopen_od[j].object)->var[0])
	{
		return err_FAIL;
	}	
	/* If TC id matches, then update all entries in the process data block */
		
	for (i = 0; i < can_frame->dlc; i++)
	{
		((struct canopen_pdb_t *) canopen_od[j].object)->var[i] 
				= can_frame->data[i];		
	}	
	return err_OK;
}


void canopen_send_heartbeat(void)
{
	struct can_frame_t can_frame;
	can_frame.id = canopen_cob_id_HEARTBEAT + CANOPEN_OWN_NODE_ID;
	can_frame.dlc = 1;
	can_frame.data[0] = canopen_current_state;
	can_send(CAN_MOB_TX, &can_frame);
}


#if CANOPEN_IS_MASTER
err_t canopen_send_sync(void)
{
	if (canopen_get_state() == canopen_state_STOPPED)
	{
		return err_FAIL;
	}	
	struct can_frame_t can_frame;
	can_frame.id = canopen_cob_id_SYNC;
	can_frame.dlc = 0;
	can_send(CAN_MOB_TX, &can_frame);
	return err_OK;
}
#endif
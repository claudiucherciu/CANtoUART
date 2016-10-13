#include <avr/io.h>
#include <assert.h>
#include "../modules/can.h"
#include "canopen.h"
#include "canopen_specific.h"


uint32_t canopen_pdo_map_tc_opmode[] = CANOPEN_PDO_MAP_TC_OPMODE;
uint32_t canopen_pdo_map_tc_request_tm[] = CANOPEN_PDO_MAP_TC_REQUEST_TM;
uint32_t canopen_pdo_map_tm_stat[] = CANOPEN_PDO_MAP_TM_STAT;
uint32_t canopen_pdo_map_tm_utc[] = CANOPEN_PDO_MAP_TM_UTC;
uint32_t canopen_pdo_map_tm_pos[] = CANOPEN_PDO_MAP_TM_POS;
uint32_t canopen_pdo_map_tm_alt[] = CANOPEN_PDO_MAP_TM_ALT;


void canopen_od_init_specific(void)
{	
	uint16_t i;

	/* RPDOs */
	i = canopen_find_index(CANOPEN_RPDO_TC_OPMODE_INDEX);
	((struct canopen_pdo_t *)canopen_od[i].object)->cob_id 
		= CANOPEN_OWN_NODE_ID + canopen_cob_id_RPDO_1;
	((struct canopen_pdo_t *)canopen_od[i].object)->transmission_type
		= 254;

	i = canopen_find_index(CANOPEN_RPDO_TC_REQUEST_TM_INDEX);
	((struct canopen_pdo_t *)canopen_od[i].object)->cob_id 
		= CANOPEN_OWN_NODE_ID + canopen_cob_id_RPDO_1;
	((struct canopen_pdo_t *)canopen_od[i].object)->transmission_type
		= 254;

	/* TPDOs */
	i = canopen_find_index(CANOPEN_TPDO_TM_STAT_INDEX);
	((struct canopen_pdo_t *)canopen_od[i].object)->cob_id 
		= CANOPEN_OWN_NODE_ID + canopen_cob_id_TPDO_1;
	((struct canopen_pdo_t *)canopen_od[i].object)->transmission_type
		= 254;
		
	i = canopen_find_index(CANOPEN_TPDO_TM_UTC_INDEX);
	((struct canopen_pdo_t *)canopen_od[i].object)->cob_id 
		= CANOPEN_OWN_NODE_ID + canopen_cob_id_TPDO_1;
	((struct canopen_pdo_t *)canopen_od[i].object)->transmission_type
		= 254;	
		
	i = canopen_find_index(CANOPEN_TPDO_TM_POS_INDEX);
	((struct canopen_pdo_t *)canopen_od[i].object)->cob_id 
		= CANOPEN_OWN_NODE_ID + canopen_cob_id_TPDO_1;
	((struct canopen_pdo_t *)canopen_od[i].object)->transmission_type
		= 254;		
		
	i = canopen_find_index(CANOPEN_TPDO_TM_ALT_INDEX);
	((struct canopen_pdo_t *)canopen_od[i].object)->cob_id 
		= CANOPEN_OWN_NODE_ID + canopen_cob_id_TPDO_1;
	((struct canopen_pdo_t *)canopen_od[i].object)->transmission_type
		= 254;			

	/* RPDO Mappings */	
	i = canopen_find_index(CANOPEN_RPDO_TC_OPMODE_INDEX + 0x200);	
	((struct canopen_pdo_map_t *)canopen_od[i].object)->number_of_entries
		= sizeof(canopen_pdo_map_tc_opmode)/4; 
	((struct canopen_pdo_map_t *)canopen_od[i].object)->map 
		= canopen_pdo_map_tc_opmode;	
	
	i = canopen_find_index(CANOPEN_RPDO_TC_REQUEST_TM_INDEX + 0x200);	
	((struct canopen_pdo_map_t *)canopen_od[i].object)->number_of_entries
		= sizeof(canopen_pdo_map_tc_request_tm)/4; 
	((struct canopen_pdo_map_t *)canopen_od[i].object)->map 
		= canopen_pdo_map_tc_request_tm;

	/* TPDO Mappings */	
	i = canopen_find_index(CANOPEN_TPDO_TM_STAT_INDEX + 0x200);	
	((struct canopen_pdo_map_t *)canopen_od[i].object)->number_of_entries
		= sizeof(canopen_pdo_map_tm_stat)/4; 
	((struct canopen_pdo_map_t *)canopen_od[i].object)->map 
		= canopen_pdo_map_tm_stat;	
	
	i = canopen_find_index(CANOPEN_TPDO_TM_UTC_INDEX + 0x200);	
	((struct canopen_pdo_map_t *)canopen_od[i].object)->number_of_entries
		= sizeof(canopen_pdo_map_tm_utc)/4; 
	((struct canopen_pdo_map_t *)canopen_od[i].object)->map 
		= canopen_pdo_map_tm_utc;	
	
	i = canopen_find_index(CANOPEN_TPDO_TM_POS_INDEX + 0x200);	
	((struct canopen_pdo_map_t *)canopen_od[i].object)->number_of_entries
		= sizeof(canopen_pdo_map_tm_pos)/4; 
	((struct canopen_pdo_map_t *)canopen_od[i].object)->map 
		= canopen_pdo_map_tm_pos;	
	
	i = canopen_find_index(CANOPEN_TPDO_TM_ALT_INDEX + 0x200);	
	((struct canopen_pdo_map_t *)canopen_od[i].object)->number_of_entries
		= sizeof(canopen_pdo_map_tm_alt)/4; 
	((struct canopen_pdo_map_t *)canopen_od[i].object)->map 
		= canopen_pdo_map_tm_alt;	
		
	/* Create od entry for each process data block and link to */
	i = canopen_find_free_index(); 
	canopen_od[i].index					= CANOPEN_PDB_TC_OPMODE_INDEX;
	canopen_od[i].object				= &canopen_pdb_tc_opmode;	
	canopen_pdb_tc_opmode.number_of_entries	= sizeof(node_gps_tc_opmode);
	canopen_pdb_tc_opmode.var				= (uint8_t *) &node_gps_tc_opmode;	

	i = canopen_find_free_index(); 
	canopen_od[i].index					= CANOPEN_PDB_TC_REQUEST_TM_INDEX;
	canopen_od[i].object				= &canopen_pdb_tc_request_tm;	
	canopen_pdb_tc_request_tm.number_of_entries	= sizeof(node_gps_tc_request_tm);
	canopen_pdb_tc_request_tm.var				= (uint8_t *) &node_gps_tc_request_tm;		
			
	i = canopen_find_free_index(); 
	canopen_od[i].index					= CANOPEN_PDB_TM_STAT_INDEX;
	canopen_od[i].object				= &canopen_pdb_tm_stat;	
	canopen_pdb_tm_stat.number_of_entries	= sizeof(node_gps_tm_stat);
	canopen_pdb_tm_stat.var					= (uint8_t *) &node_gps_tm_stat;	

	i = canopen_find_free_index(); 
	canopen_od[i].index					= CANOPEN_PDB_TM_UTC_INDEX;
	canopen_od[i].object				= &canopen_pdb_tm_utc;	
	canopen_pdb_tm_utc.number_of_entries	= sizeof(node_gps_tm_utc);
	canopen_pdb_tm_utc.var					= (uint8_t *) &node_gps_tm_utc;	

	i = canopen_find_free_index(); 
	canopen_od[i].index					= CANOPEN_PDB_TM_POS_INDEX;
	canopen_od[i].object				= &canopen_pdb_tm_pos;	
	canopen_pdb_tm_pos.number_of_entries	= sizeof(node_gps_tm_pos);
	canopen_pdb_tm_pos.var					= (uint8_t *) &node_gps_tm_pos;	

	i = canopen_find_free_index(); 
	canopen_od[i].index					= CANOPEN_PDB_TM_ALT_INDEX;
	canopen_od[i].object				= &canopen_pdb_tm_alt;	
	canopen_pdb_tm_alt.number_of_entries	= sizeof(node_gps_tm_alt);
	canopen_pdb_tm_alt.var					= (uint8_t *) &node_gps_tm_alt;	
}


void canopen_init_specific(void)
{
	struct can_frame_t can_frame;
	
	can_frame.dlc		= 8;
	can_frame.id		= CAN_MOB_RX_MASTER_ID;
	can_frame.id_mask	= CAN_MOB_RX_MASTER_ID_MASK;	
	can_configure_receive(CAN_MOB_RX_MASTER, &can_frame);	
	can_frame.dlc		= 8;
	can_frame.id		= CAN_MOB_RX_NODE_ID;
	can_frame.id_mask	= CAN_MOB_RX_NODE_ID_MASK;	
	can_configure_receive(CAN_MOB_RX_NODE, &can_frame);	

	canopen_consumer_heartbeat_counter_ms = 0;
	canopen_consumer_heartbeat_ttoggle_counter = 0;	
	node_gps_opmode_current			= node_gps_opmode_STBY;
	node_gps_opmode_next			= node_gps_opmode_STBY;
	node_gps_tm_stat.opmode			= node_gps_opmode_STBY;
	node_gps_tm_stat.valid			= 0;
	node_gps_tm_stat.bus_switch_cnt = 0;
	node_gps_sync_received			= 0;

	node_gps_tc_opmode.tc_id		= NODE_GPS_TC_OPMODE_ID;
	node_gps_tc_request_tm.tc_id	= NODE_GPS_TC_REQUEST_TM_ID;
	node_gps_tm_stat.tm_id	= NODE_GPS_TM_STAT_ID;
	node_gps_tm_utc.tm_id	= NODE_GPS_TM_UTC_ID;
	node_gps_tm_pos.tm_id	= NODE_GPS_TM_POS_ID;
	node_gps_tm_alt.tm_id	= NODE_GPS_TM_ALT_ID;			
}

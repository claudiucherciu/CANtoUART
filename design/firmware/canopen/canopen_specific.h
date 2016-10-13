
/*! 
\file canopen_specific.h
\brief		Functions related to the specific implementation of CANopen stack.
\author		Artur Scholz
\date		2016-03-14
\copyright	GNU Public License. 
*/ 
#ifndef CANOPEN_SPECIFIC_H
#define CANOPEN_SPECIFIC_H


/* Constants & Macros ********************************************************/


#define CANOPEN_IS_MASTER					0	/*!< Master or slave node */
#define CANOPEN_OWN_NODE_ID					0x001
#define CANOPEN_NUMBER_OF_OD_OBJECTS \
	(CANOPEN_NUMBER_OF_DEFAULT_OBJECTS +\
	(2 * CANOPEN_NUMBER_OF_RPDO) +\
	(2 * CANOPEN_NUMBER_OF_TPDO) +\
	CANOPEN_NUMBER_OF_PDB)	
	
#define CAN_MOB_TX				0	/*!< MOb for tx CAN messages */
#define CAN_MOB_RX_MASTER		1	/*!< MOb for rx master CAN messages */
#define CAN_MOB_RX_NODE			2	/*!< MOb for rx CAN messages to the node */
#define CAN_MOB_RX_MASTER_ID		0x000	
#define CAN_MOB_RX_MASTER_ID_MASK	0x07F	/*!< Filter for master message */
#define CAN_MOB_RX_NODE_ID			CANOPEN_OWN_NODE_ID	
#define CAN_MOB_RX_NODE_ID_MASK		0x07F	/*!< Filter for node message */

#define CANOPEN_COMMUNICATION_CYCLE_PERIOD	5000000	/*	SYNC, in microsec.  */
#define CANOPEN_CONSUMER_HEARTBEAT_TIME		250		/* Heartbeat, in ms */
#define CANOPEN_PRODUCER_HEARTBEAT_TIME		250		/* Heartbeat, in ms */
#define CANOPEN_REDUNDANCY_MANAGEMENT_BDEFAULT	can_TRX1	/* Bdefault */
#define CANOPEN_REDUNDANCY_MANAGEMENT_TTOGGLE	5			/* Ttoggle */
#define CANOPEN_REDUNDANCY_MANAGEMENT_NTOGGLE	10			/* Ntoggle */


/* RPDOs */
#define CANOPEN_RPDO_TC_OPMODE_INDEX		0x1400UL
#define CANOPEN_RPDO_TC_REQUEST_TM_INDEX	0x1401UL
#define CANOPEN_NUMBER_OF_RPDO				2 

/* TPDOs */
#define CANOPEN_TPDO_TM_STAT_INDEX			0x1800UL
#define CANOPEN_TPDO_TM_UTC_INDEX			0x1801UL
#define CANOPEN_TPDO_TM_POS_INDEX			0x1802UL
#define CANOPEN_TPDO_TM_ALT_INDEX			0x1803UL
#define CANOPEN_NUMBER_OF_TPDO				4


/* Process Data Blocks */
#define CANOPEN_PDB_TC_OPMODE_INDEX			0x6000UL
#define CANOPEN_PDB_TC_REQUEST_TM_INDEX		0x6001UL
#define CANOPEN_PDB_TM_STAT_INDEX			0x6100UL
#define CANOPEN_PDB_TM_UTC_INDEX			0x6101UL
#define CANOPEN_PDB_TM_POS_INDEX			0x6102UL
#define CANOPEN_PDB_TM_ALT_INDEX			0x6103UL
#define CANOPEN_NUMBER_OF_PDB				6 


/* TC PDO Mappings */
#define CANOPEN_PDO_MAP_TC_OPMODE \
{	(CANOPEN_PDB_TC_OPMODE_INDEX << 16) | (0 << 8) | 8,\
	(CANOPEN_PDB_TC_OPMODE_INDEX << 16) | (1 << 8) | 8 \
}
#define CANOPEN_PDO_MAP_TC_REQUEST_TM \
{	(CANOPEN_PDB_TC_REQUEST_TM_INDEX << 16) | (0 << 8) | 8,\
	(CANOPEN_PDB_TC_REQUEST_TM_INDEX << 16) | (1 << 8) | 8 \
}

/* TM PDO Mappings */
#define CANOPEN_PDO_MAP_TM_STAT \
{	(CANOPEN_PDB_TM_STAT_INDEX << 16) | (0 << 8) | 8,\
	(CANOPEN_PDB_TM_STAT_INDEX << 16) | (1 << 8) | 8,\
	(CANOPEN_PDB_TM_STAT_INDEX << 16) | (2 << 8) | 8,\
	(CANOPEN_PDB_TM_STAT_INDEX << 16) | (3 << 8) | 8 \
}
#define CANOPEN_PDO_MAP_TM_UTC \
{	(CANOPEN_PDB_TM_UTC_INDEX << 16) | (0 << 8) | 8,\
	(CANOPEN_PDB_TM_UTC_INDEX << 16) | (1 << 8) | 8,\
	(CANOPEN_PDB_TM_UTC_INDEX << 16) | (2 << 8) | 8,\
	(CANOPEN_PDB_TM_UTC_INDEX << 16) | (3 << 8) | 8 \
}
#define CANOPEN_PDO_MAP_TM_POS \
{	(CANOPEN_PDB_TM_POS_INDEX << 16) | (0 << 8) | 8,\
	(CANOPEN_PDB_TM_POS_INDEX << 16) | (1 << 8) | 8,\
	(CANOPEN_PDB_TM_POS_INDEX << 16) | (2 << 8) | 8,\
	(CANOPEN_PDB_TM_POS_INDEX << 16) | (3 << 8) | 8,\
	(CANOPEN_PDB_TM_POS_INDEX << 16) | (4 << 8) | 8,\
	(CANOPEN_PDB_TM_POS_INDEX << 16) | (5 << 8) | 8,\
	(CANOPEN_PDB_TM_POS_INDEX << 16) | (6 << 8) | 8,\
	(CANOPEN_PDB_TM_POS_INDEX << 16) | (7 << 8) | 8 \
}
#define CANOPEN_PDO_MAP_TM_ALT \
{	(CANOPEN_PDB_TM_ALT_INDEX << 16) | (0 << 8) | 8,\
	(CANOPEN_PDB_TM_ALT_INDEX << 16) | (1 << 8) | 8,\
	(CANOPEN_PDB_TM_ALT_INDEX << 16) | (2 << 8) | 8,\
	(CANOPEN_PDB_TM_ALT_INDEX << 16) | (3 << 8) | 8 \
}


/* TC Ids */
#define NODE_GPS_TC_OPMODE_ID		1
#define NODE_GPS_TC_REQUEST_TM_ID	2

/* TM Ids */
#define NODE_GPS_TM_STAT_ID		1
#define NODE_GPS_TM_UTC_ID		2
#define NODE_GPS_TM_POS_ID		3
#define NODE_GPS_TM_ALT_ID		4


/* Type Definitions **********************************************************/


/*!
\enum		err_t
\brief		Error codes 
*/
#ifndef ERR_T
#define ERR_T
typedef enum 
{
   err_OK	= 0,
   err_FAIL	= -1
} err_t;
#endif


enum node_gps_opmode_t
{
	node_gps_opmode_UNKNOWN	= 0,
	node_gps_opmode_STBY	= 1,
	node_gps_opmode_ACTIVE	= 2
};


/* Structs *******************************************************************/


struct node_gps_tc_opmode_t
{
	uint8_t tc_id	: 8;	
	uint8_t opmode	: 8;	
};


struct node_gps_tc_request_tm_t
{
	uint8_t tc_id			: 8;	
	uint8_t request_tm_id	: 8;	
};


struct node_gps_tm_stat_t
{
	uint8_t tm_id			: 8;
	uint8_t opmode			: 8;
	uint8_t valid			: 8;
	uint8_t bus_switch_cnt	: 8;		
};


struct node_gps_tm_utc_t
{
	uint8_t tm_id	: 8;
	uint8_t hour	: 8;
	uint8_t min		: 8;
	uint8_t sec		: 8;	
};


struct node_gps_tm_pos_t
{
	uint8_t tm_id		: 8;
	uint8_t lat			: 8;
	uint8_t lat_sub_1	: 8;
	uint8_t lat_sub_0	: 8;
	uint8_t lon_1		: 8;
	uint8_t lon_0		: 8;
	uint8_t lon_sub_1	: 8;
	uint8_t lon_sub_0	: 8;	
};


struct node_gps_tm_alt_t
{
	uint8_t tm_id	: 8;	
	uint8_t alt_1	: 8;
	uint8_t alt_0	: 8;
	uint8_t alt_sub	: 8;	
};


/* Global Variables **********************************************************/


uint8_t node_gps_opmode_current;
uint8_t node_gps_opmode_next;
uint8_t node_gps_sync_received;

struct node_gps_tc_opmode_t node_gps_tc_opmode;
struct node_gps_tc_request_tm_t node_gps_tc_request_tm;
struct node_gps_tm_stat_t node_gps_tm_stat;
struct node_gps_tm_utc_t node_gps_tm_utc;
struct node_gps_tm_pos_t node_gps_tm_pos;
struct node_gps_tm_alt_t node_gps_tm_alt;

struct canopen_pdb_t canopen_pdb_tc_opmode;
struct canopen_pdb_t canopen_pdb_tc_request_tm;
struct canopen_pdb_t canopen_pdb_tm_stat;
struct canopen_pdb_t canopen_pdb_tm_utc;
struct canopen_pdb_t canopen_pdb_tm_pos;
struct canopen_pdb_t canopen_pdb_tm_alt;

extern uint32_t canopen_pdo_map_tc_opmode[];
extern uint32_t canopen_pdo_map_tc_request_tm[];
extern uint32_t canopen_pdo_map_tm_stat[];
extern uint32_t canopen_pdo_map_tm_utc[];
extern uint32_t canopen_pdo_map_tm_pos[];
extern uint32_t canopen_pdo_map_tm_alt[];


/* Functions *****************************************************************/


/*!
\brief		Initialize the specific elements of the CANopen object dictionary
*/																									
void canopen_od_init_specific(void);


/*!
\brief		Initialize the specific elements of the CANopen protocol stack
*/
void canopen_init_specific(void);


#endif /* CANOPEN_SPECIFIC_H */

/*! 
\file canopen.h
\brief		Functions to initialize, send, and receive through CANopen stack.
\author		Artur Scholz
\date		2016-03-11
\copyright	GNU Public License. 
*/ 
#ifndef CANOPEN_H
#define CANOPEN_H


#include "canopen_specific.h"


/* Constants & Macros ********************************************************/


#define CANOPEN_COB_ID_SYNCH_INDEX					0x1005UL
#define CANOPEN_COMMUNICATION_CYCLE_PERIOD_INDEX	0x1006UL
#define CANOPEN_CONSUMER_HEARTBEAT_TIME_INDEX		0x1016UL
#define CANOPEN_PRODUCER_HEARTBEAT_TIME_INDEX		0x1017UL
#define CANOPEN_REDUNDANCY_MANAGEMENT_INDEX			0x2000UL
#define CANOPEN_RPDO_INDEX							0x1400UL
#define CANOPEN_RPDO_MAP_INDEX						0x1600UL
#define CANOPEN_TPDO_INDEX							0x1800UL
#define CANOPEN_TPDO_MAP_INDEX						0x1A00UL

#if CANOPEN_IS_MASTER
	#define CANOPEN_COB_ID_SYNCH				0x80000080UL
#else
	#define CANOPEN_COB_ID_SYNCH				0x00000080UL
#endif

#define CANOPEN_NUMBER_OF_DEFAULT_OBJECTS			5 


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


/*!
\enum		canopen_state_t
\brief		CANopen NMT states codes 
*/
enum canopen_state_t
{
	canopen_state_BOOTUP			= 0x00, 
	canopen_state_PRE_OPERATIONAL	= 0x7F, /* SDO, SYNC, TIME, EMCY, NMT */   
	canopen_state_OPERATIONAL		= 0x05, /* as above, plus PDO */  
	canopen_state_STOPPED			= 0x04  /* Heartbeat only */
};


/*!
\enum		canopen_cob_id_t
\brief		CANopen default communication object identifiers 
*/
enum canopen_cob_id_t
{
     canopen_cob_id_NMT_SERVICE	= 0x000,   /**< 0x000, Network management */
     canopen_cob_id_SYNC        = 0x080,   /**< 0x080, Synchronous message */
     canopen_cob_id_EMERGENCY   = 0x080,   /**< 0x080, Emergency messages (+nodeID) */
     canopen_cob_id_TPDO_1      = 0x180,   /**< 0x180, Default TPDO1 (+nodeID) */
     canopen_cob_id_RPDO_1      = 0x200,   /**< 0x200, Default RPDO1 (+nodeID) */
     canopen_cob_id_TPDO_2      = 0x280,   /**< 0x280, Default TPDO2 (+nodeID) */
     canopen_cob_id_RPDO_2      = 0x300,   /**< 0x300, Default RPDO2 (+nodeID) */
     canopen_cob_id_TPDO_3      = 0x380,   /**< 0x380, Default TPDO3 (+nodeID) */
     canopen_cob_id_RPDO_3      = 0x400,   /**< 0x400, Default RPDO3 (+nodeID) */
     canopen_cob_id_TPDO_4      = 0x480,   /**< 0x480, Default TPDO4 (+nodeID) */
     canopen_cob_id_RPDO_4      = 0x500,   /**< 0x500, Default RPDO4 (+nodeID) */
     canopen_cob_id_TSDO        = 0x580,   /**< 0x580, SDO response from server (+nodeID) */
     canopen_cob_id_RSDO        = 0x600,   /**< 0x600, SDO request from client (+nodeID) */
     canopen_cob_id_HEARTBEAT   = 0x700    /**< 0x700, Heartbeat message */
};


/* Structs *******************************************************************/


struct canopen_od_t
{
	uint16_t	index;
	void		*object;
};	


struct canopen_pdo_t
{
	uint8_t		number_of_entries;	
    uint32_t	cob_id;
	uint8_t		transmission_type;		
};


struct canopen_pdo_map_t
{
	uint8_t		number_of_entries;
    uint32_t	*map; /* Mapped parameter [Index, Subindex, Length] */		
};		
	

struct canopen_redundancy_management_t
{
    uint8_t		bdefault;
    uint8_t		ttoggle;
    uint8_t		ntoggle;
    uint8_t		ctoggle;	
};


struct canopen_pdb_t
{
	uint8_t		number_of_entries;
	uint8_t		*var;
};


/* Global Variables **********************************************************/


uint8_t	canopen_current_state;		
uint32_t canopen_cob_id_synch;	
uint32_t canopen_communication_cycle_period;
uint16_t canopen_consumer_heartbeat_time;
uint16_t canopen_producer_heartbeat_time;
uint16_t canopen_consumer_heartbeat_counter_ms;
uint8_t canopen_consumer_heartbeat_ttoggle_counter;
struct canopen_redundancy_management_t canopen_redundancy_management;
struct canopen_od_t canopen_od[CANOPEN_NUMBER_OF_OD_OBJECTS];
struct canopen_pdo_t canopen_rpdo[CANOPEN_NUMBER_OF_RPDO];
struct canopen_pdo_t canopen_tpdo[CANOPEN_NUMBER_OF_TPDO];
struct canopen_pdo_map_t canopen_rpdo_map[CANOPEN_NUMBER_OF_RPDO];
struct canopen_pdo_map_t canopen_tpdo_map[CANOPEN_NUMBER_OF_TPDO];


/* Functions *****************************************************************/
					
	
/*!
\brief		Initialize the generic CANopen object dictionary
\details	This function does the static configuration of the object
			dictionary. It defines the default communication objects, 
			and the PDO	objects and mappings.
*/																									
void canopen_od_init(void);


/*!
\brief		Initialize the generic CANopen protocol stack
\param		sysclock_Hz		System clock frequency
\param		can_baud		Desired baud rate
\details	Initialize the CANopen protocol stack and its services.
*/
void canopen_init(const uint32_t sysclock_Hz, const uint16_t can_baud_kHz);


/*!
\brief		Set the CANopen state of the node
\param		canopen_state	See #canopen_state_t
*/		
void canopen_set_state(const enum canopen_state_t canopen_state);


/*!
\brief		Returns the CANopen state of the node
\return		See #canopen_state_t
*/		
enum canopen_state_t canopen_get_state(void);


/*!
\brief		Obtain the position of an object in the od array via its index.
\param		index		Index of the block
\return		Array position of the index in the od.
*/
uint16_t canopen_find_index(const uint16_t index);


/*!
\brief		Find a free position in the od array.
\return		Array position in the od.
*/
uint16_t canopen_find_free_index(void);


/*!
\brief		Switch over to the alternate bus.
\details	This function switches to the currently inactive CAN bus and makes
			it the active bus for sending messages.
*/
void canopen_bus_switchover(void);


/*!
\brief		Send a TPDO
\param		tpdo_index		The od index of the TPDO to send
*/
err_t canopen_send_tpdo(const uint16_t tpdo_index);


/*!
\brief		Recieve a RPDO
\param		rpdo_index		The od index of the RPDO to receive
\param		can_frame		Pointer to the received CAN frame
*/
err_t canopen_receive_rpdo(const uint16_t rpdo_index, const struct can_frame_t* can_frame);


/*!
\brief		Send the heartbeat message.
*/
void canopen_send_heartbeat(void);


#if CANOPEN_IS_MASTER
/*!
\brief		Send the sync message.
*/
err_t canopen_send_sync(void);
#endif


#endif /* CANOPEN_H */
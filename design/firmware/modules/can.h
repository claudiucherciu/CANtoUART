/*! 
\file can.h
\brief		Functions to initialize, send, and receive over CAN bus.
\author		Artur Scholz
\date		2016-02-08
\copyright	GNU Public License.
*/ 
#ifndef CAN_H
#define CAN_H


/* Constants & Macros ********************************************************/


#define CAN_MOB_MAX			6		/*!< Max number of CAN message objects */
#define CAN_TRX_PORT		PORTB	/*!< CAN transceiver interface port */
#define CAN_TRX_DDR			DDRB	/*!< CAN trx data direction bit */
#define CAN_TRX1_PIN_STBY	PB1		/*!< STBY (standby) pin (9) */
#define CAN_TRX_PIN_MUX		PB2		/*!< MUX pin (16) */
#define CAN_TRX2_PIN_STBY	PB3		/*!< STBY (standby) pin (23) */


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
\enum		can_trx
\brief		An enum for the selectable CAN transceivers 
*/
enum can_trx_t
{
	can_TRX1,
	can_TRX2
};


/*!
\enum		can_baud_t
\brief		Baud rate of CAN controller
*/
enum can_baud_t 
{
   can_baud_125K	= 125,
   can_baud_250K	= 250,
   can_baud_500K	= 500,
   can_baud_1M		= 1000
};


/*!
\enum		can_mode_t
\brief		Mode of CAN controller
*/
enum can_mode_t 
{
   can_mode_STOP,		/*!< Stop the controller */
   can_mode_START		/*!< Set controller into normal operation */
};


/*!
\enum		can_trx_mode_t
\brief		Mode of CAN transceiver
*/
enum can_trx_mode_t 
{
   can_trx_mode_ON,		/*!< Set transceiver into normal operation */
   can_trx_mode_STBY	/*!< Set transceiver into standby mode */
};


/* Structs *******************************************************************/


/*!
\struct		can_frame_t		
\brief		CAN frame structure 
*/
struct can_frame_t
{
	/*! 11-bit standard frame identifier */
	uint16_t id;
	/*! Id mask, must be set properly to receive frames */		
	uint16_t id_mask;
	/* 4-bit data length code */	
	uint8_t dlc;	
	/* Frame data stored as array of 8 bytes */ 	
	uint8_t data[8];	
};


/* Global Variables **********************************************************/


uint8_t can_trx_active; /*!< The currently active CAN transceiver */	


/* Functions *****************************************************************/


/*!
\brief		Initialize CAN interface
\param		sysclock_Hz		System clock frequency
\param		can_baud		Desired baud rate
\details	Initialize the CAN interface for reception and transmission
			of CAN frames at the supported baud rates.
*/
void can_init(const uint32_t sysclock_Hz, const enum can_baud_t can_baud);


/*!
\brief		Set mode of CAN controller
\param		can_mode		Desired mode from #can_mode enum.
*/
void can_set_mode(const enum can_mode_t can_mode);


/*!
\brief		Configure a MOb for receiving of CAN frames
\details	This function defines for the selected MOb the mask and id of
			the CAN messages to filter for and enables the interrupt of 
			the selected MOb.
*/
void can_configure_receive(const uint8_t mob, const struct can_frame_t* can_frame);


/*!
\brief		Transmit a CAN frame
\param		mob				MOb to be used
\param		can_frame		Pointer to buffer of CAN frame
\return		If the frame cannot be sent because the controller is busy,
			the function will return \c err_FAIL. It will also return a
			\c err_FAIL if the sending did not succeed, for example due
			to a frame acceptance error (i.e. no other node listening).
			If sending was successful it will return \c err_OK.
*/
err_t can_send(const uint8_t mob, const struct can_frame_t* can_frame);


/*!
\brief		Initialize CAN transceiver interface
\param		sysclock_Hz		System clock frequency
\param		can_baud		Desired baud rate
\details	This function sets the pins of the microntroller for mux and
			standby to output mode and sets both transceiver to standby.
*/
void can_trx_init(void);


/*!
\brief		Select the active CAN transceiver
\param		can_trx		Transceiver to be active, see #can_trx_t enum.
\details	This function sets the global variable that indicates the
			active transceiver. Only one transceiver is	active at a time.
			With the selected active transceiver, any further control
			commands will affect the now selected active transceiver.
*/
void can_trx_select_active(const enum can_trx_t can_trx);


/*!
\brief		Set mode of CAN transceiver
\param		can_trx_mode	Desired mode from #can_trx_mode enum.
\details	The mode will be set for the active transceiver.
*/
void can_trx_set_mode(const enum can_trx_mode_t can_trx_mode);


/*!
\brief		Switch over to the other CAN transceiver
\details	This function checks which is the active CAN transceiver and
			then calls #can_trx_set_mode to activate the other one.
*/
void can_trx_toogle_active(void);


#endif /* CAN_H */

/*! 
\file gps.h
\brief		Functions to parse NMEA messages provided by GPS receiver.
\author		Artur Scholz
\date		2016-03-08
\copyright	GNU Public License. 
*/
#ifndef NMEA_H
#define NMEA_H


/* Constants & Macros ********************************************************/


#define NMEA_MSG_LENGTH			80
#define NMEA_MSG_START			'$'
#define NMEA_MSG_ID_LENGTH		5
#define NMEA_MSG_ID				"GPGGA" /* the message type to filter for */
#define NMEA_MSG_ENTRIES		15		/* number of entries in the message */
#define NMEA_MSG_ENTRIES_MIN	7	/* number of entries in a minimum message */
#define NMEA_MSG_DELIMITER		","		/* message entries are comma separated */
#define NMEA_MSG_ENTRY_LENGHT_MAX	11	/* maximum length of a message entry + NULL character */


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


/* Structs *******************************************************************/


struct nmea_t 
{
	char msg_buffer[NMEA_MSG_LENGTH];
	uint8_t is_valid;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t lat;
	uint16_t lat_sub;
	uint16_t lon;
	uint16_t lon_sub;
	uint16_t alt;	
	uint8_t alt_sub;
};


/* Global Variables **********************************************************/


struct nmea_t nmea;


/* Functions *****************************************************************/



/* Compares the first part of the received message to check if it is of the desired message type */
err_t nmea_check_msg(const struct nmea_t* nmea);


/* Parses the message in the message buffer.  The comma separated entries in the message are
   temporarily stored in separate buffers. A valid message contains more than the minimum number
   of entries as specified in NMEA_MSG_ENTRIES_MIN. These separate buffers are then converted
   from text to numbers. Only if all conversions succeed the content is flagged as valid. */
void nmea_parse(struct nmea_t* nmea);


#endif /* NMEA_H */

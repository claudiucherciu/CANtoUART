#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "nmea.h"


err_t nmea_check_msg(const struct nmea_t* nmea)
{
	if (strncmp(&nmea->msg_buffer[0], NMEA_MSG_ID, NMEA_MSG_ID_LENGTH) == 0)
	{
		return err_OK;
	}
	return err_FAIL;
}


void nmea_parse(struct nmea_t* nmea)
{
	char *token;
	uint8_t msg_entries[NMEA_MSG_ENTRIES][NMEA_MSG_ENTRY_LENGHT_MAX];
	uint8_t i;
	i = 0;	
	token = strtok(nmea->msg_buffer, NMEA_MSG_DELIMITER); /* get first token */
	while (token != NULL) /* walk through other tokens */
	{
		strcpy((char *)msg_entries[i++], token);
		token = strtok(NULL, NMEA_MSG_DELIMITER);
	}
	/* message is considered invalid unless checks are passed */	
	nmea->is_valid = 0; 
	if (i > NMEA_MSG_ENTRIES_MIN)
	{	
		nmea->hour = (uint8_t) (((msg_entries[1][0] - '0') * 10) 
								+ (msg_entries[1][1] - '0'));
		nmea->min = (uint8_t) (((msg_entries[1][2] - '0') * 10) 
								+ (msg_entries[1][3] - '0'));
		nmea->sec = (uint8_t) (((msg_entries[1][4] - '0') * 10) 
								+ (msg_entries[1][5] - '0'));
		nmea->lat = (uint8_t) (((msg_entries[2][0] - '0') * 10) 
								+ (msg_entries[2][1] - '0'));
		if (msg_entries[3][0] == 'S') /* account for southern latitude */
		{
			nmea->lat = ~nmea->lat + 1; /* create 2's complement */
		}
		nmea->lat_sub = (uint16_t) (((msg_entries[2][2] - '0') * 1000) 
									+ ((msg_entries[2][3] - '0') * 100) 
									+ ((msg_entries[2][5] - '0') * 10) 
									+ (msg_entries[2][6] - '0'));
		nmea->lon = (uint16_t) (((msg_entries[4][0] - '0') * 100) 
									+ ((msg_entries[4][1] - '0') * 10) 
									+ (msg_entries[4][2] - '0'));
		if (msg_entries[5][0] == 'E') /* account for eastern longitude */
		{
			nmea->lon = ~nmea->lon + 1; /* create 2's complement */
		}
		nmea->lon_sub = (uint16_t) (((msg_entries[4][3] - '0') * 1000) 
									+ ((msg_entries[4][4] - '0') * 100) 
									+ ((msg_entries[4][6] - '0') * 10) 
									+ (msg_entries[4][7] - '0'));
	
		token = strtok(msg_entries[9], "."); /* get the first token */
		nmea->alt = (uint16_t) atoi(token);
		token = strtok(NULL, NMEA_MSG_DELIMITER);
		nmea->alt_sub = (uint8_t) atoi(token);
		
		nmea->is_valid = 1;
	}		
}


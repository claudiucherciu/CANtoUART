/*! 
\file queue.h
\brief		Functions to use a queue.
\author		Artur Scholz
\date		2016-03-21
\copyright	GNU Public License. 
*/
#ifndef QUEUE_H
#define QUEUE_H


/* Constants & Macros ********************************************************/


#define QUEUE_DATATYPE	uint8_t
#define QUEUE_SIZE		10


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
\enum		bool
\brief		Boolean 
*/
#ifndef BOOL
#define BOOL
typedef enum 
{
   FALSE	= 0,
   TRUE		= 1
} bool;
#endif


/* Structs *******************************************************************/





/* Global Variables **********************************************************/


QUEUE_DATATYPE queue[QUEUE_SIZE];
QUEUE_DATATYPE queue_buffer;
uint16_t queue_front;
uint16_t queue_rear;


/* Functions *****************************************************************/


err_t queue_add(const QUEUE_DATATYPE value);
err_t queue_remove(QUEUE_DATATYPE* value);
bool queue_is_emtpy(void);


#endif /* QUEUE_H */

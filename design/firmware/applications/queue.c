#include <avr/io.h>
#include "queue.h"


QUEUE_DATATYPE queue_buffer = 0;
uint16_t queue_front = 0;
uint16_t queue_rear = 0;


err_t queue_add(const QUEUE_DATATYPE value)
{
	queue[queue_front] = value;
	queue_front++;
	if (queue_front >= QUEUE_SIZE)
	{
		queue_front = 0;
	}
	return err_OK;
}


err_t queue_remove(QUEUE_DATATYPE* value)
{
	if (queue_front == queue_rear)
	{
		return err_FAIL;
	}
	*value = queue[queue_rear];
	queue_rear++;
	if (queue_rear >= QUEUE_SIZE)
	{
		queue_rear = 0;
	}	
	return err_OK;
}


bool queue_is_emtpy(void)
{
	if (queue_front == queue_rear)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}
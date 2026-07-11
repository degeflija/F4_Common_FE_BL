#include  "System_Configuration.h"
#include  "FreeRTOS_wrapper.h"

extern "C" TaskHandle_t create_restricted_task( const TaskParameters_t *param)
{
	RestrictedTask task( *param);
	return task.get_handle();
}

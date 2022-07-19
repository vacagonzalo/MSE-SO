#ifndef MYOS_H_
#define MYOS_H_

#define NO_ARGUMENTS 0
#define NO_ERROR_HANDLER 0

typedef enum
{
	myOS_FAILED,
	myOS_SUCCEEDED
} myOS_report_t;

myOS_report_t myOS_create_task(void * task);

myOS_report_t myOS_init_scheduler();

#endif /* MYOS_H_ */

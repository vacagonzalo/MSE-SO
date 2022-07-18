#include "myOS.h"
#include <stdint.h>

#define TASKS_NUMB   4
#define STACK_POOL   256
#define STACK_SIZE   (STACK_POOL / TASKS_NUMB)

#define STACK_XPSR   (STACK_SIZE - 1)
#define STACK_PC     (STACK_SIZE - 2)
#define STACK_LR     (STACK_SIZE - 3)
#define STACK_R12    (STACK_SIZE - 4)
#define STACK_R3     (STACK_SIZE - 5)
#define STACK_R2     (STACK_SIZE - 6)
#define STACK_R1     (STACK_SIZE - 7)
#define STACK_R0     (STACK_SIZE - 8)
#define STACK_LR_IRQ (STACK_SIZE - 9)

#define XPSR_T_1     (1 << 24)
#define LR_IRQ       0xfffffff9

#define STACK_AUTO_P STACK_R0

static uint32_t __myOS__task_created__ = 0;

static uint32_t __myOS__stack__[TASKS_NUMB][STACK_SIZE];

myOS_task_report_t myOS_create_task(void * task,
									void * errorHandler,
									void * argument)
{
	if(TASKS_NUMB == __myOS__task_created__)
	{
		return myOS_FAILED;
	}

	__myOS__stack__[__myOS__task_created__][STACK_XPSR]   = (uint32_t)XPSR_T_1;
	__myOS__stack__[__myOS__task_created__][STACK_PC]     = (uint32_t)task;
	__myOS__stack__[__myOS__task_created__][STACK_LR]     = (uint32_t)errorHandler;
	__myOS__stack__[__myOS__task_created__][STACK_R0]     = (uint32_t)argument;
	__myOS__stack__[__myOS__task_created__][STACK_LR_IRQ] = (uint32_t)LR_IRQ;

	++__myOS__task_created__;
	return myOS_SUCCEEDED;
}

static uint32_t __myOS__stack_pointer__[TASKS_NUMB];

myOS_report_t myOS_init_stack()
{
	for(uint32_t index = 0; index < (__myOS__task_created__ + 1); ++index)
	{
		__myOS__stack_pointer__[index] = (uint32_t)&__myOS__stack__[index][STACK_AUTO_P];
	}
	return myOS_SUCCEEDED;
}

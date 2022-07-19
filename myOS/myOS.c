#include "myOS.h"
#include <stdint.h>
#include "stm32f429xx.h"

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

#define SYSTICK_RESET 0U
#define SYSTICK_FREQ  4000000
#define SYSTICK_MILL (SYSTICK_FREQ/1000)
#define SYSTICK_MILLISECONDS(t) (t*SYSTICK_MILL-1)
#define SYSTICK_PRIORITY 15

#define CTRL_ENABLE     (1U<<0)
#define CTRL_TICK_INT   (1U<<1)
#define CTRL_CLCK_SRC   (1U<<2)
#define CTRL_COUNT_FLAG	(1U<<16)

#define INVALID_TASK (TASKS_NUMB+1)
#define LAST_TASK    (TASKS_NUMB-1)
#define FIRST_TASK   0

static uint32_t __myOS__task_created__ = 0;

static uint32_t __myOS__stack__[TASKS_NUMB][STACK_SIZE];

myOS_report_t myOS_create_task(void * task)
{
	if(TASKS_NUMB == __myOS__task_created__)
	{
		return myOS_FAILED;
	}

	__myOS__stack__[__myOS__task_created__][STACK_XPSR]   = (uint32_t)XPSR_T_1;
	__myOS__stack__[__myOS__task_created__][STACK_PC]     = (uint32_t)task;
	__myOS__stack__[__myOS__task_created__][STACK_LR]     = (uint32_t)NO_ERROR_HANDLER;
	__myOS__stack__[__myOS__task_created__][STACK_R0]     = (uint32_t)NO_ARGUMENTS;
	__myOS__stack__[__myOS__task_created__][STACK_LR_IRQ] = (uint32_t)LR_IRQ;

	++__myOS__task_created__;
	return myOS_SUCCEEDED;
}

static uint32_t __myOS__stack_pointer__[TASKS_NUMB];

static uint32_t __myOS__task__index__ = INVALID_TASK;

myOS_report_t myOS_init_stack()
{
	uint32_t created_copy = __myOS__task_created__;
	for(uint32_t index = 0; index < (__myOS__task_created__ + 1); ++index)
	{
		if(TASKS_NUMB >= __myOS__task_created__)
		{
				return myOS_FAILED;
		}
		if(created_copy != __myOS__task_created__)
		{
				return myOS_FAILED;
		}
		__myOS__stack_pointer__[index] = (uint32_t)&__myOS__stack__[index][STACK_AUTO_P];
	}
	return myOS_SUCCEEDED;
}

static void schedule()
{
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
	__ISB();
	__DSB();
}

myOS_report_t myOS_init_scheduler()
{
	myOS_init_stack();
	SysTick->CTRL = SYSTICK_RESET;
	SysTick->VAL  = SYSTICK_RESET;
	SysTick->LOAD = SYSTICK_MILLISECONDS(10);
	NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIORITY);
	SysTick->CTRL   = CTRL_CLCK_SRC;
	SysTick->CTRL  |= CTRL_ENABLE;
	SysTick->CTRL  |= CTRL_TICK_INT;
	schedule();
}

void SysTick_Handler()
{
	schedule();
}

// static uint32_t __myOS__stack_pointer__[TASKS_NUMB];
int32_t getNextContext(int32_t current_context)
{
	uint32_t returned_stack;
	if(INVALID_TASK == __myOS__task__index__)
	{
		/* This should be the first context change */
		returned_stack = __myOS__stack_pointer__[FIRST_TASK];
		__myOS__task__index__ = 0;
	}
	else if(LAST_TASK == __myOS__task__index__)
	{
		/* Round-robin scheduler, we go back to the first task */
		__myOS__stack_pointer__[LAST_TASK] = current_context;
		returned_stack = __myOS__stack_pointer__[FIRST_TASK];
		__myOS__task__index__ = 0;
	}
	else
	{
		/* Round-robin scheduler, we go to the next tast */
		__myOS__stack_pointer__[__myOS__task__index__] = current_context;
		++__myOS__task__index__;
		returned_stack = __myOS__stack_pointer__[__myOS__task__index__];
	}
	return returned_stack;
}

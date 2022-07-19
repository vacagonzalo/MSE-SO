#include <stdint.h>
#include "Leds/leds.h"
#include "../myOS/myOS.h"

/* EVIL FUNCTION DON'T USE! */
void never_use_this_delay_for_test_only()
{
	for(int32_t i=0;i<900000;i++);
}

void init_platform()
{
	leds_init();
}

void task1()
{
	while(1)
	{
		leds_write(LED1, LED_ON);
		never_use_this_delay_for_test_only();
		leds_write(LED1, LED_OFF);
		never_use_this_delay_for_test_only();
	}
}

void task2()
{
	while(1)
	{
		leds_write(LED2, LED_ON);
		never_use_this_delay_for_test_only();
		leds_write(LED2, LED_OFF);
		never_use_this_delay_for_test_only();
	}
}

void task3()
{
	while(1)
	{
		leds_write(LED3, LED_ON);
		never_use_this_delay_for_test_only();
		leds_write(LED3, LED_OFF);
		never_use_this_delay_for_test_only();
	}
}

int main(void)
{
	init_platform();

	myOS_create_task(task1);
	myOS_create_task(task2);
	myOS_create_task(task3);
	myOS_init_scheduler();

	for(;;) { }
}

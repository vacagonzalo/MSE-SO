#include <stdint.h>
#include "Leds/leds.h"

/* EVIL FUNCTION DON'T USE! */
void never_use_this_delay_for_test_only()
{
	for(int32_t i=0;i<900000;i++);
}

void init_platform()
{
	leds_init();
}

int main(void)
{
	init_platform();
    /* Loop forever */
	for(;;)
	{
		leds_write(LED1, LED_ON);
		leds_write(LED2, LED_ON);
		leds_write(LED3, LED_ON);
		never_use_this_delay_for_test_only();
		leds_write(LED1, LED_OFF);
		leds_write(LED2, LED_OFF);
		leds_write(LED3, LED_OFF);
		never_use_this_delay_for_test_only();
	}
}

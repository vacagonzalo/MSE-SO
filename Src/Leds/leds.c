#include "leds.h"
#include "../platform.h"

#define LED1_ODR (1<<0)
//TODO LED2, LED3, LED4

uint16_t ODRs[] = {
		LED1_ODR
		//TODO LED2, LED3, LED4
};

void leds_init()
{
	/* All the leds are connected to PORTB */
	RCC->AHB1ENR   |= (1<<1);            /* Enable the GPIOB clock   */

	/* LED1 */
	GPIOB->MODER   |= (1<<0);            /* Set PB0 as output        */
	GPIOB->OTYPER  &~ (1<<0);            /* Set PB0 as Push-Pull     */
	GPIOB->OSPEEDR |= (1<<1);            /* Set PB0 as Fast-speed    */
	GPIOB->PUPDR   &= ((1<<1) | (1<<0)); /* PB0 no pull-up/pull-down */

	//TODO LED2, LED3, LED4
}

void leds_write(leds_name_t led, leds_state_t state)
{
	uint16_t odr = ODRs[led];
	if(LED_ON == state)
	{
		GPIOB->ODR |= odr;
	}
	else
	{
		GPIOB->ODR &= ~odr;
	}
}

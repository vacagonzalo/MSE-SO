#include "leds.h"
#include "../platform.h"

#define LED1_ODR (1<<0)
#define LED2_ODR (1<<7)
#define LED3_ODR (1<<14)

uint16_t ODRs[] = {
		LED1_ODR,
		LED2_ODR,
		LED3_ODR
};

void leds_init()
{
	/* All the LEDs are connected to PORTB */
	RCC->AHB1ENR   |= (1<<1);              /* Enable the GPIOB clock    */

	/* LED1 (on-board PB0)*/
	GPIOB->MODER   |= (1<<0);              /* Set PB0 as output         */
	GPIOB->OTYPER  &~ (1<<0);              /* Set PB0 as Push-Pull      */
	GPIOB->OSPEEDR |= (1<<1);              /* Set PB0 as Fast-speed     */
	GPIOB->PUPDR   &= ((1<<1) | (1<<0));   /* PB0 no pull-up/pull-down  */

	/* LED2 (on-board PB7) */
	GPIOB->MODER   |= (1<<14);             /* Set PB7 as output         */
	GPIOB->OTYPER  &~ (1<<7);              /* Set PB7 as Push-Pull      */
	GPIOB->OSPEEDR |= (1<<15);             /* Set PB7 as Fast-speed     */
	GPIOB->PUPDR   &= ((1<<15) | (1<<14)); /* PB0 no pull-up/pull-down  */

	/* LED3 (on-board PB14) */
	GPIOB->MODER   |= (1<<28);             /* Set PB14 as output        */
	GPIOB->OTYPER  &~ (1<<14);             /* Set PB14 as Push-Pull     */
	GPIOB->OSPEEDR |= (1<<29);             /* Set PB14 as Fast-speed    */
	GPIOB->PUPDR   &= ((1<<29) | (1<<28)); /* PB14 no pull-up/pull-down */
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

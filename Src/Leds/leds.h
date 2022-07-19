#ifndef LEDS_LEDS_H_
#define LEDS_LEDS_H_

typedef enum
{
	LED1,
	LED2,
	LED3
} leds_name_t;

typedef enum
{
	LED_OFF,
	LED_ON
} leds_state_t;

void leds_init();

void leds_write(leds_name_t led, leds_state_t state);

#endif /* LEDS_LEDS_H_ */

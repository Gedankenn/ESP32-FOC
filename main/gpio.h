#include "driver/gpio.h"

// *----gpio config -----------------------
#define GPIO_OUTPUT_A_LOW    18
#define GPIO_OUTPUT_A_HIGH    19

#define GPIO_OUTPUT_B_LOW    20
#define GPIO_OUTPUT_B_HIGH    21

#define GPIO_OUTPUT_C_LOW    22
#define GPIO_OUTPUT_C_HIGH    23

#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<GPIO_OUTPUT_A_LOW) | (1ULL<<GPIO_OUTPUT_A_HIGH) | (1ULL<<GPIO_OUTPUT_B_LOW) | (1ULL<<GPIO_OUTPUT_B_HIGH) | (1ULL<<GPIO_OUTPUT_C_LOW) | (1ULL<<GPIO_OUTPUT_C_HIGH) )

void gpio_init(void);
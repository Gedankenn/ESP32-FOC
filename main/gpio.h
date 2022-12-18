#include "driver/gpio.h"

// *----gpio config -----------------------

// #define BLDC_DRV_EN_GPIO    18
// #define BLDC_DRV_FAULT_GPIO 19
// #define BLDC_DRV_OVER_CURRENT_FAULT MCPWM_SELECT_F0

#define GPIO_PWM0A_OUT 19   //Set GPIO 19 as PWM0A
#define GPIO_PWM0B_OUT 18   //Set GPIO 18 as PWM0B
#define GPIO_PWM1A_OUT 17   //Set GPIO 17 as PWM1A
#define GPIO_PWM1B_OUT 16   //Set GPIO 16 as PWM1B
#define GPIO_PWM2A_OUT 15   //Set GPIO 15 as PWM2A
#define GPIO_PWM2B_OUT 14   //Set GPIO 14 as PWM2B

void gpio_init(void);
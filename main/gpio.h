#include "driver/gpio.h"

// *----gpio config -----------------------

#define BLDC_DRV_EN_GPIO    18
#define BLDC_DRV_FAULT_GPIO 19
#define BLDC_DRV_OVER_CURRENT_FAULT MCPWM_SELECT_F0

#define BLDC_PWM_AH_GPIO 21
#define BLDC_PWM_AL_GPIO 22
#define BLDC_PWM_BH_GPIO 23
#define BLDC_PWM_BL_GPIO 25
#define BLDC_PWM_CH_GPIO 26
#define BLDC_PWM_CL_GPIO 27

void gpio_init(void);
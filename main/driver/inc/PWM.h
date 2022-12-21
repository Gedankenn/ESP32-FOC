#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
// #include "driver/mcpwm_prelude.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "gpio.h"
#include "math.h"
#include "VF.h" 

#define BLDC_MCPWM_PERIOD 100
#define BLDC_MCPWM_FREQUENCY 1000

void PWM_init(void);
void update_motor_speed_callback(double rpm, double theta);
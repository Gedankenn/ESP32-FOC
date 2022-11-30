#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "gpio.h"

#define BLDC_MCPWM_PERIOD 100
void PWM_init(void);
void update_motor_speed_callback(double rpm);
void atualiza_velocidade(double rpm);
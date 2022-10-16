#include "driver/mcpwm_prelude.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "gpio.h"

#define BLDC_MCPWM_TIMER_RESOLUTION_HZ 10000000 // 10MHz, 1 tick = 0.1us
//Counter resolution in Hz, ranges from around 300KHz to 80MHz. The step size of each count tick equals to (1 / resolution_hz) seconds
#define BLDC_MCPWM_PERIOD              500      // 50us, 20KHz //Number of count ticks within a period
#define BLDC_SPIN_DIRECTION_CCW        false    // define the spin direction
#define BLDC_SPEED_UPDATE_PERIOD_US    200000   // 200ms

#define BLDC_MCPWM_GEN_INDEX_HIGH 0
#define BLDC_MCPWM_GEN_INDEX_LOW  1

void PWM_init(void);
void update_motor_speed_callback(void *arg, double rpm);
void atualiza_velocidade(double rpm);
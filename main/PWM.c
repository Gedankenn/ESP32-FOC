#include "PWM.h"

extern double Tmax;
static const char *TAG = "PWM";

void update_motor_speed_callback(double comp_value){
    double duty;
    // duty=100*(comp_value/1000);
    duty = comp_value;
    ESP_LOGI(TAG, " update motor speed: V = %f  duty cycle: %f",comp_value,duty);
    for (int i = 0; i < 3; i++) {
        // ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(cmprs[i], comp_value));
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, duty);
        // mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0B, duty);
        // mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM1A, duty);
        // mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM1B, duty);
        // mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM2A, duty);
        // mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM2B, duty);


    }
}

void atualiza_velocidade(double V){
    int comp_value;
    comp_value = V*BLDC_MCPWM_PERIOD/Tmax;
    update_motor_speed_callback(comp_value);
    ESP_LOGI(TAG, "PWM Count =  %d | V = %f",comp_value,V);
}

void PWM_init(void){

    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GPIO_PWM0B_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, GPIO_PWM1A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, GPIO_PWM1B_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, GPIO_PWM2A_OUT);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2B, GPIO_PWM2B_OUT);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000;    //frequency = 1000Hz
    pwm_config.cmpr_a = 60.0;       //duty cycle of PWMxA = 60.0%
    pwm_config.cmpr_b = 50.0;       //duty cycle of PWMxb = 50.0%
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   //Configure PWM0A & PWM0B with above settings
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);   //Configure PWM1A & PWM1B with above settings
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);   //Configure PWM1A & PWM1B with above settings

    mcpwm_start(MCPWM_UNIT_0,MCPWM_TIMER_0);
    mcpwm_start(MCPWM_UNIT_0,MCPWM_TIMER_1);
    mcpwm_start(MCPWM_UNIT_0,MCPWM_TIMER_2);
}
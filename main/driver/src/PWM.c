#include "PWM.h"


extern double Tmax;
static const char *TAG  = "PWM";
extern double pi;

void update_motor_speed_callback(double comp_value, double theta){
    double duty_A, duty_B, duty_C;
    duty_A = comp_value*FEM_sin(theta);
    duty_B = comp_value*FEM_sin(theta + pi/3.0);
    duty_C = comp_value*FEM_sin(theta - pi/3.0);
    
    if(duty_A >= 0)
    {
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, duty_A);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0B, 0);
    }
    else
    {
        duty_A = -1*duty_A;
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, 0);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0B, duty_A);
    }
    if(duty_B >= 0)
    {
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM0A, duty_B);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM0B, 0);
    }
    else
    {
        duty_B = -1*duty_B;
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM0A, 0);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM0B, duty_B);
    }
    if(duty_C >= 0)
    {
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM0A, duty_C);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM0B, 0);
    }
    else
    {
        duty_C = -1*duty_C;
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM0A, 0);
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM0B, duty_C);
    }
}

void PWM_init(void){

    // mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);
    // mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, GPIO_PWM0B_OUT);
    // mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, GPIO_PWM1A_OUT);
    // mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, GPIO_PWM1B_OUT);
    // mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, GPIO_PWM2A_OUT);
    // mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2B, GPIO_PWM2B_OUT);

    mcpwm_pin_config_t pin_config = {
        .mcpwm0a_out_num = GPIO_PWM0A_OUT,
        .mcpwm0b_out_num = GPIO_PWM0B_OUT,
        .mcpwm1a_out_num = GPIO_PWM1A_OUT,
        .mcpwm1b_out_num = GPIO_PWM1B_OUT,
        .mcpwm2a_out_num = GPIO_PWM2A_OUT,
        .mcpwm2b_out_num = GPIO_PWM2B_OUT,
    };
    mcpwm_set_pin(MCPWM_UNIT_0, &pin_config);


    mcpwm_config_t pwm_config;
    pwm_config.frequency = BLDC_MCPWM_FREQUENCY;    //frequency = 1000Hz
    pwm_config.cmpr_a = 0.0;       //duty cycle of PWMxA = 60.0%
    pwm_config.cmpr_b = 0.0;       //duty cycle of PWMxb = 50.0%
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);   //Configure PWM0A & PWM0B with above settings
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);   //Configure PWM1A & PWM1B with above settings
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);   //Configure PWM1A & PWM1B with above settings

    mcpwm_start(MCPWM_UNIT_0,MCPWM_TIMER_0);
    mcpwm_start(MCPWM_UNIT_0,MCPWM_TIMER_1);
    mcpwm_start(MCPWM_UNIT_0,MCPWM_TIMER_2);
}
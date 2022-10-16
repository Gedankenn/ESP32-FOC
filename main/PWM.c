#include "PWM.h"

static const char *TAG = "PWM";
mcpwm_cmpr_handle_t comparators[3];

void update_motor_speed_callback(void *arg, double rpm){
    mcpwm_cmpr_handle_t *cmprs = (mcpwm_cmpr_handle_t *)arg;
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(cmprs[i], rpm));
    }
}

void atualiza_velocidade(double rpm){
    update_motor_speed_callback(comparators, rpm);
}

void PWM_init(void){
    ESP_LOGI(TAG, "Create MCPWM timer");
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT, 
        .resolution_hz = BLDC_MCPWM_TIMER_RESOLUTION_HZ, //Counter resolution in Hz, ranges from around 300KHz to 80MHz. The step size of each count tick equals to (1 / resolution_hz) seconds
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
        .period_ticks = BLDC_MCPWM_PERIOD, //Number of count ticks within a period
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

    ESP_LOGI(TAG, "Create MCPWM operator");
    mcpwm_oper_handle_t operators[3];
    mcpwm_operator_config_t operator_config = {
        .group_id = 0,
    };
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &operators[i]));
    }

    ESP_LOGI(TAG, "Connect operators to the same timer");
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_operator_connect_timer(operators[i], timer));
    }

    ESP_LOGI(TAG, "Create comparators");
    mcpwm_comparator_config_t compare_config = {
        .flags.update_cmp_on_tez = true,
    };
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_new_comparator(operators[i], &compare_config, &comparators[i]));
        // set compare value to 0, we will adjust the speed in a period timer callback
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparators[i], 0));
    }


    ESP_LOGI(TAG, "Create PWM generators");
    mcpwm_gen_handle_t generators[3][2] = {};
    mcpwm_generator_config_t gen_config = {};
    const int gen_gpios[3][2] = {
        {BLDC_PWM_AH_GPIO, BLDC_PWM_AL_GPIO},
        {BLDC_PWM_BH_GPIO, BLDC_PWM_BL_GPIO},
        {BLDC_PWM_CH_GPIO, BLDC_PWM_CL_GPIO},
    };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            gen_config.gen_gpio_num = gen_gpios[i][j];
            ESP_ERROR_CHECK(mcpwm_new_generator(operators[i], &gen_config, &generators[i][j]));
        }
    }

    ESP_LOGI(TAG, "Set generator actions");
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_generator_set_actions_on_timer_event(generators[i][BLDC_MCPWM_GEN_INDEX_HIGH],
                        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH),
                        MCPWM_GEN_TIMER_EVENT_ACTION_END()));
        ESP_ERROR_CHECK(mcpwm_generator_set_actions_on_compare_event(generators[i][BLDC_MCPWM_GEN_INDEX_HIGH],
                        MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparators[i], MCPWM_GEN_ACTION_LOW),
                        MCPWM_GEN_COMPARE_EVENT_ACTION_END()));
        ESP_ERROR_CHECK(mcpwm_generator_set_actions_on_brake_event(generators[i][BLDC_MCPWM_GEN_INDEX_HIGH],
                        MCPWM_GEN_BRAKE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_OPER_BRAKE_MODE_CBC, MCPWM_GEN_ACTION_LOW),
                        MCPWM_GEN_BRAKE_EVENT_ACTION_END()));
        ESP_ERROR_CHECK(mcpwm_generator_set_actions_on_brake_event(generators[i][BLDC_MCPWM_GEN_INDEX_HIGH],
                        MCPWM_GEN_BRAKE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_OPER_BRAKE_MODE_CBC, MCPWM_GEN_ACTION_LOW),
                        MCPWM_GEN_BRAKE_EVENT_ACTION_END()));
    }

    ESP_LOGI(TAG, "Setup deadtime");
    mcpwm_dead_time_config_t dt_config = {
        .posedge_delay_ticks = 5,
    };
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_generator_set_dead_time(generators[i][BLDC_MCPWM_GEN_INDEX_HIGH], generators[i][BLDC_MCPWM_GEN_INDEX_HIGH], &dt_config));
    }
    dt_config = (mcpwm_dead_time_config_t) {
        .negedge_delay_ticks = 5,
        .flags.invert_output = true,
    };
    for (int i = 0; i < 3; i++) {
        ESP_ERROR_CHECK(mcpwm_generator_set_dead_time(generators[i][BLDC_MCPWM_GEN_INDEX_HIGH], generators[i][BLDC_MCPWM_GEN_INDEX_LOW], &dt_config));
    }

    ESP_LOGI(TAG, "Turn off all the gates");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            ESP_ERROR_CHECK(mcpwm_generator_set_force_level(generators[i][j], 0, true));
        }
    }

    // ESP_LOGI(TAG, "Start a timer to adjust motor speed periodically");
    // esp_timer_handle_t periodic_timer = NULL;
    // const esp_timer_create_args_t periodic_timer_args = {
    //     .callback = update_motor_speed_callback,
    //     .arg = comparators,
    // };
    // ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    // ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, BLDC_SPEED_UPDATE_PERIOD_US));

    ESP_LOGI(TAG, "Start the MCPWM timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));
}
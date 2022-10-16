#include "gpio.h"


void gpio_init(void){
// //zero-initialize the config structure.
//     gpio_config_t io_conf = {};
//     //disable interrupt
//     io_conf.intr_type = GPIO_INTR_DISABLE;
//     //set as output mode
//     io_conf.mode = GPIO_MODE_OUTPUT;
//     //bit mask of the pins that you want to set,e.g.GPIO18/19
//     io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
//     //disable pull-down mode
//     io_conf.pull_down_en = 0;
//     //disable pull-up mode
//     io_conf.pull_up_en = 0;
//     //configure GPIO with the given settings
//     gpio_config(&io_conf);

    gpio_config_t drv_en_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1 << BLDC_DRV_EN_GPIO,
    };
    ESP_ERROR_CHECK(gpio_config(&drv_en_config));
    gpio_set_level(BLDC_DRV_EN_GPIO, 0);
}
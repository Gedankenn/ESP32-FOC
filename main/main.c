//* Fabio Slika Stella
//*    Implementation of FOC BLDC starting method

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "adc.h"
#include "gpio.h"

int adc_raw[2][10];
int voltage[2][10];

adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t adc1_cali_handle;
bool do_calibration1;

TaskHandle_t handle_FOC = NULL;

//! Function that executes de necessary steps for the FOC implementation
void FOC(void *arg)
{
    
}

void app_main(void)
{

    adc_init();
    gpio_init();


    printf("Minimum free heap size: %"PRIu32" bytes\n", esp_get_minimum_free_heap_size());

    while(1) {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN0, &adc_raw[0][0]));
        ESP_LOGI(TAG_ADC, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN0, adc_raw[0][0]);

        if (do_calibration1) {
            ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc1_cali_handle, adc_raw[0][0], &voltage[0][0]));
            ESP_LOGI(TAG_ADC, "ADC%d Channel[%d] Cali Voltage: %d mV", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN0, voltage[0][0]);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc1_handle));
    if (do_calibration1) {
        example_adc_calibration_deinit(adc1_cali_handle);
    }
}
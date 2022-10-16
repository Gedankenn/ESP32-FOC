//* Fabio Slika Stella
//*    Implementation of FOC BLDC starting method OPEN LOOP

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <math.h>
#include "esp_timer.h"
#include "esp_attr.h"

#include "adc.h"
#include "gpio.h"
#include "FEM.h"
//#include "PWM.h"

int adc_raw[2][10];
double voltage[2][10];
double ref;
double rad;
double theta            = 0;
double V                = 0;

const double nADC       = 4096;
const double max_rpm    = 10000;
const double adc_to_rpm = max_rpm/nADC;
const double pi         = 3.141592;
const double rpm_to_rad = 2*pi/60;
const double Tmax       = 12;
const double Gf         = Tmax/(max_rpm*rpm_to_rad);
const double gain_adc   = max_rpm/4095;
const double F_adc      = 10000;
const double T_adc      = 1/F_adc;



const TickType_t xDelay = 100 / portTICK_PERIOD_MS;
TaskHandle_t handle_FOC = NULL;
adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t adc1_cali_handle;
bool do_calibration1;

//! Function that executes de necessary steps for the FOC implementation
//! First try will be in open loop
const static char *TAG_FOC = "FOC";

void FOC(void *arg)
{
    ESP_LOGI(TAG_FOC, "Entrada da task FOC");
    //** Leitura da referencia
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN0, &adc_raw[0][0]));
    ESP_LOGI(TAG_ADC, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN0, adc_raw[0][0]);
    ref = adc_raw[0][0]*adc_to_rpm;
    ref = ref*gain_adc;

    //* ideally the ref will be a value of angular velocity in rpm
    rad = ref*rpm_to_rad;
    theta = fmod((theta+rad*T_adc),(2*pi));

    V = ref*Gf;
    if(V >= Tmax){
        V = Tmax;
    }
    //atualiza_velocidade(rad);
    printf("V = %f\n",V);
    printf("rad = %f\n",rad);
    vTaskDelay(xDelay*100);
}

void app_main(void)
{

    adc_init();
    gpio_init();
    //PWM_init();

    printf("Minimum free heap size: %"PRIu32" bytes\n", esp_get_minimum_free_heap_size());
    //xTaskCreate(FOC,"OPEN-LOOP",1024,NULL,5,&handle_FOC);

    while(1) {
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN0, &adc_raw[0][0]));
        ESP_LOGI(TAG_ADC, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN0, adc_raw[0][0]);
        vTaskDelay(100*xDelay);
    }
    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc1_handle));
    if (do_calibration1) {
        example_adc_calibration_deinit(adc1_cali_handle);
    }
}
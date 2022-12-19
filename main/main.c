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
#include "PWM.h"
#include "VF.h"


int adc_raw[2][10];
double voltage[2][10];
double ref;
double rad;
double theta            = 0;
double V                = 0;

const double nADC       = 4095;
const double max_rpm    = 10000;
const double pi         = 3.141592;
const double rpm_to_rad = 2*pi/60;
const double Tmax       = 12;
const double Gf         = Tmax/(max_rpm);
const double gain_adc   = max_rpm/nADC;
const double F_adc      = 10000;
const double T_adc      = 1/F_adc;



const TickType_t xDelay = 10 / portTICK_PERIOD_MS;
TaskHandle_t handle_FOC = NULL;
adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t adc1_cali_handle;
bool do_calibration1;

void app_main(void)
{

    adc_init();
    gpio_init();
    PWM_init();

    printf("Minimum free heap size: %"PRIu32" bytes\n", esp_get_minimum_free_heap_size());
    xTaskCreate(VF,"OPEN-LOOP",2048,NULL,5,&handle_FOC);

    while(1)
    {
        vTaskDelay(xDelay);
    }
}
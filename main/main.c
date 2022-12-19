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


const TickType_t xDelay = 1 / portTICK_PERIOD_MS;
TaskHandle_t handle_VF = NULL;

void app_main(void)
{

    adc_init();
    gpio_init();
    PWM_init();

    printf("Minimum free heap size: %"PRIu32" bytes\n", esp_get_minimum_free_heap_size());
    xTaskCreate(OpenLoopVF_Control,"OPEN-LOOP",2048,NULL,5,&handle_VF);

    while(1)
    {
        vTaskDelay(xDelay);
    }
}
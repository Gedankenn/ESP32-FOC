#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "esp_log.h"
#include "soc/soc_caps.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

//*-------------- ADC CONFIG -------------------
#define ADC1_GPIO36_CHANNEL     ADC_CHANNEL_0
#define ADC1_CHANNEL_0_GPIO_NUM 36

//*ADC1 Channels
#define EXAMPLE_ADC1_CHAN0      ADC_CHANNEL_0

const static char *TAG_ADC = "ADC";
// *--------- END CONFIG ------------------------

void adc_init(void);
bool example_adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle);
void example_adc_calibration_deinit(adc_cali_handle_t handle);
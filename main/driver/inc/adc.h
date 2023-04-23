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

#define nADC        (4095.0)
#define pi          (3.141592)
#define gain_adc    (max_rpm/nADC)
#define F_adc       100.0
#define T_adc       (1/F_adc)
// *--------- END CONFIG ------------------------

void adc_init(void);
bool example_adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle);
void example_adc_calibration_deinit(adc_cali_handle_t handle);
int GetADCValue(void);
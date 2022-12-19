//! Function that executes de necessary steps for the FOC implementation
//! First try will be in open loop
#include "VF.h"

const static char *TAG_VF = "VF";

void VF(void *arg)
{
    while(1){
        ESP_LOGI(TAG_VF, "Entrada da task VF");
        //** Leitura da referencia
        ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN0, &adc_raw[0][0]));
        // ESP_LOGI(TAG_ADC, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN0, adc_raw[0][0]);
        ref = adc_raw[0][0];
        ref = ref*gain_adc;

        //* ideally the ref will be a value of angular velocity in rpm
        rad = ref*rpm_to_rad;
        theta = fmod((theta+rad*T_adc),(2*pi));

        V = ref*Gf;
        if(V >= Tmax){
            V = Tmax;
        }
        atualiza_velocidade(V, theta);
        printf("V = %f\n",V);
        printf("rad = %f\n",rad);
        printf("theta = %f\n",theta);
        vTaskDelay(xDelay);

    }
}

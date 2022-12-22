//! Function that executes de necessary steps for the FOC implementation
//! First try will be in open loop
#include "application/inc/VF.h"
#include "driver/inc/PWM.h"


const static char *TAG_VF = "Open-Loop VF";
const TickType_t xDelay_VF = 100 / portTICK_PERIOD_MS;

void OpenLoopVF_Control(void *arg)
{
    int ref = 0;
    double rad = 0;
    double theta = 0;
    double Volts = 0;
    while(1){
        ESP_LOGI(TAG_VF, "Entrada da task VF");
        ref = GetADCValue();
        //* ideally the ref will be a value of angular velocity in rpm
        rad = ref*gain_adc*rpm_to_rad; //* this rad value is an estimation of the velocity in radians/s.

        theta = fmod((theta+rad*T_adc),(2*pi)); //* the position in radians

        Volts = ref*Gf*gain_adc; //* the actual VF control
        if(Volts >= Tmax){
            Volts = Tmax;
        }
        UpdateSpeed(Volts, theta); //* send a reference in Volts.
        printf("ref = %d\n",ref);
        printf("V = %f\n",Volts);
        printf("rad = %f\n",rad);
        printf("theta = %f\n",theta);
        vTaskDelay(xDelay_VF);

    }
}

double FEM_sin(double ang)
{
    return sin(ang);
}

void UpdateSpeed(double Volts, double theta){
    double comp_value;
    comp_value = Volts*BLDC_MCPWM_PERIOD/Tmax;
    update_motor_speed_callback(comp_value, theta);
}

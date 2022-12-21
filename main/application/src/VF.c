//! Function that executes de necessary steps for the FOC implementation
//! First try will be in open loop
#include "application/inc/VF.h"


const static char *TAG_VF = "Open-Loop VF";

void OpenLoopVF_Control(void *arg)
{
    int ref;
    double rad;
    while(1){
        ESP_LOGI(TAG_VF, "Entrada da task VF");
        ref = GetADCValue();
        //* ideally the ref will be a value of angular velocity in rpm
        rad = ref*gain_adc*rpm_to_rad; //* this rad value is an estimation of the velocity in radians/s.

        theta = fmod((theta+rad*T_adc),(2*pi)); //* the position in radians

        V = ref*Gf; //* the actual VF control
        if(V >= Tmax){
            V = Tmax;
        }
        UpdateSpeed(V, theta); //* send a reference in Volts.
        printf("V = %f\n",V);
        printf("rad = %f\n",rad);
        printf("theta = %f\n",theta);
        vTaskDelay(xDelay);

    }
}

double FEM_sin(double ang)
{
    return sin(ang);
}

void UpdateSpeed(double V, double theta){
    double comp_value;
    comp_value = V*BLDC_MCPWM_PERIOD/Tmax;
    update_motor_speed_callback(comp_value, theta);
}

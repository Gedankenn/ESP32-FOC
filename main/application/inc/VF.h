#include "driver/inc/adc.h"


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

double ref;
double rad;
double theta;
double V    ;

#define rpm_to_rad  (2*pi/60)
#define max_rpm     (10000)
#define Tmax        (12)
#define Gf          (Tmax/(max_rpm))


void OpenLoopVF_Control(void *arg);
double FEM_sin(double ang);
void UpdateSpeed(double rpm, double theta);
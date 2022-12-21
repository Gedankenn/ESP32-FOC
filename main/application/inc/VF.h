#include "driver/inc/adc.h"

double ref;
double rad;
double theta            = 0;
double V                = 0;

const double rpm_to_rad = 2*pi/60;
const double Tmax       = 12;
const double Gf         = Tmax/(max_rpm);
const double max_rpm    = 10000;


void OpenLoopVF_Control(void *arg);
double FEM_sin(double ang);
void UpdateSpeed(double rpm, double theta);
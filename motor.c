#include "xc.h"
#include "motor.h"
#include <stdlib.h>
#include <math.h>

void drive(float speed_left, float speed_right) {
    // tank drive style stepper control - give values -1 - 1
    // max speed is ~7 rev/s with 1.8 degree per pwm osc - 200 ticks per rev in full step mode
    // assumed 8MHz FRC osc, 4,000,000 ticks per sec. 200 tick * ~3 rev/s = 620 ticks/sec (max speed from signal generator)
    // 4,000,000 / 620 =  6451.61, this is period for max speed
    if (speed_left < 0) {_LATA0 = 0;} else {_LATA0 = 1;};
    if (speed_left == 0) {OC1CON1bits.OCM=  0b000;} else {OC1CON1bits.OCM=  0b110;};
    OC1RS = (int) (4000000/620)/(fabsf(speed_left));
    OC1R = OC1RS/2;

    if (speed_right < 0) {_LATA1 = 1;} else {_LATA1 = 0;};
    if (speed_right == 0) {OC2CON1bits.OCM=  0b000;} else {OC2CON1bits.OCM=  0b110;};
    OC2RS = (int) (4000000/620)/(fabsf(speed_right));
    OC2R = OC2RS/2;
};

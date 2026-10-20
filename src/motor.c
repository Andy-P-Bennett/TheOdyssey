#include "xc.h"
#include "motor.h"

void drive(int speed_left, int speed_right) {
    // tank drive style stepper control - give values -1 - 1
    // max speed is ~7 rev/s with 1.8 degree per pwm osc - 200 ticks per rev in full step mode
    // assumed 8MHz FRC osc, 4,000,000 ticks per sec. 200 tick * 7 rev/s = 1400 ticks/sec
    // 4,000,000 / 1400 =  2857.14, this is period for max speed
    if (speed_left < 0) {_LATA0 = 0} else (_LATA0 = 1);
    OC1RS = speed_left * (4000000/1400);
    OC1R = OC1RS/2;
    if (speed_right < 0) {_LATA1 = 0} else (_LATA1 = 1);
    OC2RS = speed_right * (4000000/1400);
    OC2R = OC2RS/2;
};

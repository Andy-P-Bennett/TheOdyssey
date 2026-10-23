#include "xc.h"
#include "motor.h"
#include <stdlib.h>
#include <math.h>

#define FCY_HZ 4000000.0f
#define TIMER_HZ (FCY_HZ/64.0f) // 62.5 kHz with 1:64 prescaler
#define STEPS_PER_REV 200.0f
#define MAX_SPEED 3.0f
#define MIN_SPEED 0.005f // rev/sec, slowest speed we can do with 16 bit timer
#define MAX_TICKS_PER_SEC (STEPS_PER_REV * MAX_SPEED) // 600 ticks/sec
#define PI 3.14159265f
#define WHEEL_DIAMETER 0.1f // in meters
#define IWD 0.05f // in meters, inter-wheel distance


void drive(float speed_left, float speed_right) {
    // tank drive style stepper control - give values in rev/s
    // max speed is ~3 rev/s with 1.8 degree per pwm osc - 200 ticks per rev in full step mode
    // T2/3 1:64 prescalar 62.5kHz, 16 us per tick. 200 tick * ~3 rev/s = 600 ticks/sec (max speed from signal generator)
    // 62500 / 600 =  104.167, this is period for max speed ~ less period is faster speed
    // 62500 / ? = 65534 -> ? = 0.954 hz (steps/sec)-> 0.954 steps/sec / 200 steps/rev = 
    // 0.00477 rev/sec, this is the slowest speed we can do with 16 bit timer
    float period1, period2;

    // constrain speed values to be within the min/max range
    if (fabsf(speed_left) < MIN_SPEED) {speed_left = 0;}
    else if (speed_left > MAX_SPEED) {speed_left = MAX_SPEED;}
    else if (speed_left < -MAX_SPEED) {speed_left = -MAX_SPEED;}
    if (fabsf(speed_right) < MIN_SPEED) {speed_right = 0;}
    else if (speed_right > MAX_SPEED) {speed_right = MAX_SPEED;}
    else if (speed_right < -MAX_SPEED) {speed_right = -MAX_SPEED;}

    // set the direction pins and pwm period for each stepper
    if (speed_left < 0) {_LATA0 = 0;} else {_LATA0 = 1;};
    if (speed_left == 0) {OC1CON1bits.OCM=  0b000;} else {OC1CON1bits.OCM=  0b110;};
    period1 = TIMER_HZ / (STEPS_PER_REV * fabsf(speed_left));
    if (period1 > 65535.0f) {period1 = 65535.0f;};
    OC1RS = (unsigned int) (period1);
    OC1R = OC1RS/2;

    // set the direction pins and pwm period for each stepper - PROBLEM PIN
    if (speed_right < 0) {_LATB0 = 1;} else {_LATB0 = 0;};
    if (speed_right == 0) {OC2CON1bits.OCM=  0b000;} else {OC2CON1bits.OCM=  0b110;};
    period2 = TIMER_HZ / (STEPS_PER_REV * fabsf(speed_right));
    if (period2 > 65535.0f) {period2 = 65535.0f;};
    OC2RS = (unsigned int) (period2);
    OC2R = OC2RS/2;
};

void drive_mps(float left_ms, float right_ms) {
    // conversion layer to drive motors in m/s instead of rev/s
    float k = 1.0f / (PI * WHEEL_DIAMETER);   // m/s -> rev/s conversion factor
    drive(left_ms * k, right_ms * k);
};

void arcade_drive(float velocity, float turn_rate) {
    // velocity in m/s, turn_rate in rad/s - assumed left turn positive 
    // base velocity, +- r*w, where r = IWD/2, depending on L/R
    drive_mps(velocity - turn_rate * (IWD/2.0f), 
              velocity + turn_rate * (IWD/2.0f));
}
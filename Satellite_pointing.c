//turn servo until it is pointing at the antenna, then turn on the laser
#include "xc.h"
#include "Definitions.h"
#include "motor.h"


void satellite_comm (void){
    //set up PWM for servo
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 0b111;
    OC2CON2bits.SYNCSEL = 0b11111;
    OC2CON2bits.OCTRIG = 0;
    OC2RS = 20000;
    OC2R = 500;
    
    OC2CON1bits.OCM = 0b110; // Start PWM on pin 14. Set to zero to disable.

    int sharp_front = ADC1BUF0;

     if (sharp_front < highest_sharp_thresh) { // there should be a case to stop driving not a case to keep driving, constantly setting the same value for pwm will cause problems
        drive(full_speed, full_speed); //keep driving forward until close to wall
    }   else {
        drive(0, 0); // stop
    }

        
    while (1) {
        int ir_top = ADC1BUF9; 
        if (ir_top < ir_thresh) {
            OC2R ++; // turn servo until it points at the antenna
            for(int i = 0; i <= 100; i++) {}
        }
    _LATB7 = 1;//turn on laser
    }
}



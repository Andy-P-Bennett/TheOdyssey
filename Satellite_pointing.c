//turn servo until it is pointing at the antenna, then turn on the laser
#include "xc.h"
#include "Definitions.h"
#include "motor.h"


satellite_comm {
    //set up PWM for servo
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 0b111;
    OC2CON2bits.SYNCSEL = 0b11111;
    OC2CON2bits.OCTRIG = 0;
    OC2RS = 20000;
    OC2R = 500;
    
    //OC2CON1bits.OCM = 0b110; // Start PWM on pin 14. Set to zero to disable.


    antenna_value = 0; //this is a placeholder value, will need to be updated after testing
    ir_sensing = ANSB14; 
    if (sharp_front < close_sharp_thresh) {
        drive(full_speed, full_speed); //keep driving forward until close to wall
    }   else {
        drive(0, 0); // stop
    }
    while (ir_sensing < antenna_value) {
        OC2R ++; // turn servo until it points at the antenna
        for(int i = 0; i <100, i++) {
            
        }
    }
    _LATB7 = 1;//turn on laser

}



#include "xc.h"
#pragma config FNOSC = FRC //8 MHz oscillator

int stepsTaken = 0;
int timesUp = 0;

void __attribute__((interrupt, no_auto_psv)) _OC1Interrupt(void) {
    _OC1IF = 0; // Clear the interrupt flag
    stepsTaken++;
}
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    _T1IF = 0; // Clear the interrupt flag
    timesUp = 1;
}

int main(void) {    
    ANSA = 0;
    TRISA = 0;
    LATA = 0;
    _RCDIV = 0;

    TRISAbits.TRISA0 = 0; //pin 2 DO - dir pin
    TRISAbits.TRISA1 = 0; // pin 3 DO - enable pin
    TRISAbits.TRISA2 = 1; // pin 7 DI
    
    LATAbits.LATA1 = 0; //lo enable stepper 
   
    OC1CON1 = 0;
    OC1CON2 = 0;
    
    OC1CON1bits.OCTSEL = 0b111;
    OC1CON2bits.SYNCSEL = 0b11111;
    OC1CON2bits.OCTRIG = 0;
    
    OC1RS = 19999; //set the period - 8000000/2 = 4000000, 
    // we want 200hz for full step 1 rev/s- 4000000/200 - ~19,999
    OC1R = OC1RS/2; //default 50% DC for half DC
    OC1CON1bits.OCM=  0b000; // Start PWM on pin 14. Set to 0 to stop PWM. 
    
    _OC1IP = 4; // Select the interrupt priority
    _OC1IE = 1; // Enable the OC2 interrupt
    _OC1IF = 0; // Clear the interrupt flag
    
    _T1IF = 4;
    _T1IE = 1;
    _T1IF = 0;
    
//    while(1) {
//        static unsigned int dir = 1;
//        if (stepsTaken > 400) {
//            if (dir) {
//                dir = 0;
//            } else {
//                dir = 1;
//            }
//            _LATA0 = dir;
//
//            // Go numberOfSteps in the reverse direction
//            stepsTaken = 0;
//        }
//    }
    enum theStateEnum {waiting, cw1, ccw1, cw2, ccw2_inf};
    enum theStateEnum theState = waiting;
    
    while(1) {
        if (PORTAbits.RA2) {
            switch(theState) {
            case waiting:
                stepsTaken = 0;
                theState = cw1;
                OC1CON1bits.OCM=  0b110; // Start PWM on pin 14. Set to 0 to stop PWM. 

                break;
            case cw1: //clockwise 5 rev at 1 rev/s
                _LATA0 = 1;
                OC1RS = 19999;
                OC1R = OC1RS/2;
                if (stepsTaken > 1000) {
                    theState = ccw1;
                    stepsTaken = 0;
                }
                break;
            case ccw1: //counterclockwise 5 sec at 1/2 rev/s - 2.5 rev 
                _LATA0 = 0;
                OC1RS = 39999;
                OC1R = OC1RS/2;
                if (stepsTaken > 500) {
                    theState = cw2;
                    stepsTaken = 0;
                }
                break;
            case cw2: //clockwise 3 rev at 1 rev/s - 3 rev
                _LATA0 = 1;
                OC1RS = 19999;
                OC1R = OC1RS/2;
                if (stepsTaken > 600) {
                    theState = ccw2_inf;
                    stepsTaken = 0;
                }
                break;
            case ccw2_inf: //counterclockwise at 4 rev/s indefinitely
                _LATA0 = 0;
                OC1RS = 4999;
                OC1R = OC1RS/2;
                break;
            default:
                break;
            } 
        } else {
            theState = waiting;
            OC1CON1bits.OCM=  0b000; // Start PWM on pin 14. Set to 0 to stop PWM. 

        }
    }
    return 0;
}

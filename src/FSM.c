#include "xc.h"
#include "FSM.h"

int fsm(theState) {

    
    if (PORTAbits.RA2) {
        switch(theState) {
        case IDLE:
            stepsTaken = 0;
            theState = cw1;
            OC1CON1bits.OCM=  0b110; // Start PWM on pin 14. Set to 0 to stop PWM. 

            break;
        case FORWARD: //clockwise 5 rev at 1 rev/s
            _LATA0 = 1;
            OC1RS = 19999;
            OC1R = OC1RS/2;
            if (stepsTaken > 1000) {
                theState = ccw1;
                stepsTaken = 0;
            }
            break;
        case LEFT: //counterclockwise 5 sec at 1/2 rev/s - 2.5 rev 
            _LATA0 = 0;
            OC1RS = 39999;
            OC1R = OC1RS/2;
            if (stepsTaken > 500) {
                theState = cw2;
                stepsTaken = 0;
            }
            break;
        case RIGHT: //clockwise 3 rev at 1 rev/s - 3 rev
            _LATA0 = 1;
            OC1RS = 19999;
            OC1R = OC1RS/2;
            if (stepsTaken > 600) {
                theState = ccw2_inf;
                stepsTaken = 0;
            }
            break;
        case REVERSE: //counterclockwise at 4 rev/s indefinitely
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
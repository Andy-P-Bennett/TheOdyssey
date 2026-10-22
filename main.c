#include "xc.h"
// #include "FSM.h"
#include "motor.h"
#pragma config FNOSC = FRC //8 MHz oscillator

int stepsTaken = 0;
int steps2Taken = 0;
int timesUp = 0;

void __attribute__((interrupt, no_auto_psv)) _OC1Interrupt(void) {
    _OC1IF = 0; // Clear the interrupt flag
    stepsTaken++;
}
void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void) {
    _OC2IF = 0; // Clear the interrupt flag
    steps2Taken++;
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

    TRISAbits.TRISA0 = 0; //pin 2 DO - Left Stepper dir pin
    TRISAbits.TRISA1 = 0; // pin 3 DO - Right Stepper dir pin
    TRISAbits.TRISA2 = 1; // pin 7 DI - unused
    
    // LATAbits.LATA1 = 0; //lo enable stepper  - we don't need enable, just set pwm
   
    OC1CON1 = 0;
    OC1CON2 = 0;
    
    OC1CON1bits.OCTSEL = 0b111;
    OC1CON2bits.SYNCSEL = 0b11111;
    OC1CON2bits.OCTRIG = 0;
    
    OC1RS = 19999; //set the period - 8000000/2 = 4000000, 
    // we want 200hz for full step 1 rev/s- 4000000/200 - ~19,999
    OC1R = OC1RS/2; //default 50% DC for half DC
    OC1CON1bits.OCM=  0b000; // Start PWM on pin 14. Set to 0 to stop PWM. 
    
    _OC1IP = 4; // Select the Left Stepper interrupt priority
    _OC1IE = 1; // Enable the OC1 interrupt
    _OC1IF = 0; // Clear the interrupt flag

    OC2CON1 = 0;
    OC2CON2 = 0;
    
    OC2CON1bits.OCTSEL = 0b111;
    OC2CON2bits.SYNCSEL = 0b11111;
    OC2CON2bits.OCTRIG = 0;
    
    OC2RS = 19999; //set the period - 8000000/2 = 4000000, 
    // we want 200hz for full step 1 rev/s- 4000000/200 - ~19,999
    OC2R = OC2RS/2; //default 50% DC for half DC
    OC2CON1bits.OCM=  0b000; // Start PWM on pin 4. Set to 0 to stop PWM. 
    
    _OC2IP = 4; // Select the Right Stepper interrupt priority
    _OC2IE = 1; // Enable the OC2 interrupt
    _OC2IF = 0; // Clear the interrupt flag
    
    _T1IF = 4; // Timer 1 interrupt priority
    _T1IE = 1;
    _T1IF = 0;
    

    enum milestoneStateEnum {IDLE, FORWARD, LEFT, FORWARD_2, TURN_180, FORWARD_3, STOP};
    enum milestoneStateEnum mileState = IDLE;
    while(1) {
        // fsm() - this is how we'll do things in real code
        // below is the milestone fsm for simplicity, delete after milestone.
        switch(mileState) {
        case IDLE:
            stepsTaken = 0;
            mileState = FORWARD;
            break;
        case FORWARD: 
            drive(1,1);
            if (stepsTaken > 1000) {
                mileState = LEFT;
                stepsTaken = 0;
            }
            break;
        case LEFT:
            drive(-.5,.5);
            if (stepsTaken > 500) {
                mileState = IDLE;
                stepsTaken = 0;
            }
            break;
        case FORWARD_2:
            drive(1,1);
            if (stepsTaken > 1000) {
                mileState = TURN_180;
                stepsTaken = 0;
            }
            break;
        case TURN_180:
            drive(-1,1);
            if (stepsTaken > 1000) {
                mileState = FORWARD_3;
                stepsTaken = 0;
            }
            break;
        case FORWARD_3:
            drive(1,1);
            if (stepsTaken > 1000) {
                mileState = STOP;
                stepsTaken = 0;
            }
            break;
        case STOP:
            drive(0,0);
            // mileState = IDLE;
            break;
        }
    }
}

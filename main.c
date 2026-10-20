#include "xc.h"
#include "FSM.h"
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

    TRISAbits.TRISA0 = 0; //pin 2 DO - Left Stepper dir pin
    TRISAbits.TRISA1 = 0; // pin 3 DO - Right Stepper dir pin
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
    
    _OC1IP = 4; // Select the Left Stepper interrupt priority
    _OC1IE = 1; // Enable the OC1 interrupt
    _OC1IF = 0; // Clear the interrupt flag
    
    _T1IF = 4; // Timer 1 interrupt priority
    _T1IE = 1;
    _T1IF = 0;
    

    
    while(1) {
        fsm()
    }
}

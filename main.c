#include "xc.h"
// #include "FSM.h"
#include "motor.h"
#include "Line_following_base.h"
#pragma config FNOSC = FRC //8 MHz oscillator
#pragma config FWDTEN = OFF // Disable watchdog timer while testing
//...otherwise, the robot resets every like 3 minutes and acts possessed
#pragma config OSCIOFNC = OFF   // OSCO pin is RA3 (I/O), not CLKO - otherwise pin 8 is possessed


//volatile int stepsTaken = 0;
//volatile int steps2Taken = 0;
//volatile int timesUp = 0;

//void __attribute__((interrupt, no_auto_psv)) _OC1Interrupt(void) {
//    _OC1IF = 0; // Clear the interrupt flag
//    stepsTaken++;
//}
//void __attribute__((interrupt, no_auto_psv)) _OC2Interrupt(void) {
//    _OC2IF = 0; // Clear the interrupt flag
//    steps2Taken++;
//}

// This timer is not yet fully initialized. Do so before using it.
// Note that timers 2/3 are used for the stepper PWM.
//void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
//    _T1IF = 0; // Clear the interrupt flag
//    timesUp = 1;
//}

int main(void) {    
    ANSA = 0;
    TRISA = 0;
    LATA = 0;
    ANSB = 0;
    TRISB = 0;
    LATB = 0;
    _RCDIV = 0;

    TRISBbits.TRISB9 = 0; //pin 13 DO - Left Stepper dir pin
    TRISBbits.TRISB8 = 0; // pin 12 DO - Right Stepper dir pin
   
    //Left stepper PWM setup
    OC3CON1 = 0;
    OC3CON2 = 0;
    
    OC3CON1bits.OCTSEL = 0b000; //select timer 2 for left stepper pwm
    T2CONbits.TCKPS = 0b10; //prescaler 64:1 ~ 62.5kHz, 16 us per tick
    T2CONbits.TON = 1; //turn on timer 2
    OC3CON2bits.SYNCSEL = 0b11111;
    OC3CON2bits.OCTRIG = 0;
    
    OC3RS = 19999; //set the period - 8000000/2 = 4000000, 
    // we want 200hz for full step 1 rev/s- 4000000/200 - ~19,999
    OC3R = OC3RS/2; //default 50% DC for half DC
    OC3CON1bits.OCM=  0b000; // Start PWM on pin 14. Set to 0 to stop PWM. 
    
//    _OC3IP = 4; // Select the Left Stepper interrupt priority
//    _OC3IE = 1; // Enable the OC3 interrupt
//    _OC3IF = 0; // Clear the interrupt flag

    //Right stepper PWM setup
    OC1CON1 = 0;
    OC1CON2 = 0;
    
    OC1CON1bits.OCTSEL = 0b000; //select timer 3 for right stepper pwm
    T3CONbits.TCKPS = 0b10; //prescaler 64:1 
    T3CONbits.TON = 1; //turn on timer 3
    OC1CON2bits.SYNCSEL = 0b11111;
    OC1CON2bits.OCTRIG = 0;
    
    OC1RS = 19999; //set the period - 8000000/2 = 4000000, //old math, but overwritten anyway
    // we want 200hz for full step 1 rev/s- 4000000/200 - ~19,999
    OC1R = OC1RS/2; //default 50% DC for half DC
    OC1CON1bits.OCM=  0b000; // Start PWM on pin 4. Set to 0 to stop PWM. 
    
//    _OC2IP = 4; // Select the Right Stepper interrupt priority
//    _OC2IE = 1; // Enable the OC2 interrupt
//    _OC2IF = 0; // Clear the interrupt flag
    
//    _T1IP = 4; // Timer 1 interrupt priority
//    _T1IE = 1;
//    _T1IF = 0;
    qrd_init();
    drive(0.5, 0.5);

    while(1) {
        // fsm()
        line_following_fsm();
        // drive(0.5, 0.5);
    }
}

#include "xc.h"
#include "motor.h"
#include "Line_following_base.h"
#include "AnalogInitialization.h"
#include "Canyon_navigation_base.h"
#include "Definitions.h"
#include "BallSort.h"
#pragma config FNOSC = FRC //8 MHz oscillator
#pragma config FWDTEN = OFF // Disable watchdog timer while testing
//...otherwise, the robot resets every like 3 minutes and acts possessed
#pragma config OSCIOFNC = OFF   // OSCO pin is RA3 (I/O), not CLKO - otherwise pin 8 is possessed


volatile int stepsTaken = 0;
//volatile int steps2Taken = 0;
//volatile int timesUp = 0;

void __attribute__((interrupt, no_auto_psv)) _OC3Interrupt(void) {
    _OC3IF = 0; // Clear the interrupt flag
    stepsTaken++;
}

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
    
   _OC3IP = 4; // Select the Left Stepper interrupt priority
   _OC3IE = 1; // Enable the OC3 interrupt
   _OC3IF = 0; // Clear the interrupt flag

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
    
    // _OC1IP = 4; // Select the Right Stepper interrupt priority
    // _OC1IE = 1; // Enable the OC1 interrupt
    // _OC1IF = 0; // Clear the interrupt flag
    
    //    _T1IP = 4; // Timer 1 interrupt priority
    //    _T1IE = 1;
    //    _T1IF = 0;
    init_an();
    drive(0.5, 0.5);
    enum primary_state {line_follow, canyon_navigation, retrieve_sample, return_sample, satellite_comm, check_wall, intersection};
    enum primary_state ps = line_follow;

    int time = 0; // set up timer this is just a place holder
    volatile int sample_recieved = 0; // use these as checks for returning to transmit
    volatile int sample_deposited = 0;
    volatile int canyon_navigated = 0;

    while(1) {
        int sharp_front = ADC1BUF0; // pin 2
        int sharp_right = ADC1BUF1; // pin 3
        int qrd_right = ADC1BUF4; // pin 6
        int qrd_center = ADC1BUF13; // pin 7
        int qrd_left = ADC1BUF14; // pin 8
        int qrd_int = ADC1BUF12; // pin 15
        // int qrd_sample = ADC1BUF11; // pin 16
        int ir_right = ADC1BUF10; // pin 17
        // int ir_top = ADC1BUF9; // pin 18
        int cs;
        int rs;
        switch(ps) {
            case(line_follow):
                if (ir_right < ir_thresh && !sample_recieved) { // identify if sample is ready to be recieved
                    ps = retrieve_sample;
                    break;
                }
                else if (qrd_int <= line_qrd_thresh) { // identify if intersection
                    break; //FOR TESTING PURPOSES, REMOVE LATER
                    drive(turn_speed, full_speed);
                    stepsTaken = 0;
                    ps = intersection;
                    break;
                }
                else if (sharp_right > right_sharp_thresh) { // wall or bin detected on right
                    drive(turn_speed, -turn_speed);
                    stepsTaken = 0;
                    ps = check_wall;
                    break;
                }
                line_following_fsm();            
                break;
            
            case (canyon_navigation): // switch to line following if any track qrd senses the line
                if (qrd_left >= line_qrd_thresh || qrd_right >= line_qrd_thresh || qrd_center >= line_qrd_thresh || qrd_int <= line_qrd_thresh) {
                    if (sharp_right > right_sharp_thresh) {drive(-turn_speed, turn_speed);} // turn left if wall on right
                    else {drive(-turn_speed, turn_speed);} // turn right if no wall on right
                    stepsTaken = 0;
                    while (stepsTaken < steps_in_turn) {} // this is here to allow the robot to turn without exiting the function
                    drive(full_speed, full_speed); // return to forward drive
                    ps = line_follow;
                    canyon_navigated = 1; // notes that the canyon portion is complete
                }
                else if (canyon_navigation_fsm(stepsTaken)) {stepsTaken = 0;} // will not run canyon nav if the QRDs pick up the line
                break;

            case(retrieve_sample): 
                cs = CollectSample(stepsTaken, time);
                if (cs == 1) {
                    stepsTaken = 0;
                    time = 0;
                }
                else if (cs == 2) {
                    ps = line_follow; // I think this is all that is needed, maybe reset timer and steps?
                    sample_recieved = 1;
                }
                break;
            
            case(return_sample): 
                rs = ReturnSample(stepsTaken, time);
                if (rs == 1) {
                    stepsTaken = 0;
                    time = 0;
                }
                else if (rs == 2) {
                    ps = line_follow; // I think this is all that is needed, maybe reset timer and steps?
                    sample_deposited = 1;
                }
                break;
            
            case(satellite_comm):
                if (sharp_front >= close_sharp_thresh) {
                    drive(0, 0);
                    // start pwm for locating satellite and checkin the top IR sensor
                }
                else {line_following_fsm;}
                break;

            case(check_wall):
                if (stepsTaken >= steps_in_turn) {
                    if (sharp_front >= front_sharp_thresh) {ps = canyon_navigation;}
                    else {ps = return_sample;}
                    drive(-turn_speed, turn_speed); // return to facing forward
                    stepsTaken = 0;
                    while (stepsTaken < steps_in_turn) {} // I couldn't think of a better way to do this lol
                    drive(full_speed, full_speed);
                }
                break;

            case(intersection):
                if (stepsTaken >= steps_in_curve) {
                    if (sample_recieved && sample_deposited && canyon_navigated) { // this is the statement to check if the course is complete
                        ps = satellite_comm;
                    }
                    else {
                        ps = line_follow;
                        drive(full_speed, full_speed);
                    }
                }
                break;
        }
        // line_following_fsm();
        // if (canyon_navigation_fsm(stepsTaken) == 1) {
        //     stepsTaken = 0;
        // }
        // drive(0.5, 0.5);
    }
}
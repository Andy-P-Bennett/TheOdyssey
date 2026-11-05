#include "xc.h"
#include "motor.h"
#include <stdlib.h>
#include <math.h>


#pragma config FNOSC = FRC //8 MHz oscillator


//int TMR1 = 0;

enum m1_states {stop, straight, adjust_left, adjust_right, turn_left, turn_right}
enum m1_states dir = stop;


    // Right QRD set-up : set pin 6 (AN4) as analog input-----------------------------
    _CH0SA = 0b0000; // positive input
    _CH0NA = 0b00; // negative input
    _TRISB2 = 1; // set pin 6 to input
    _ANSB2 = 1; // set pin 6 to analog

    // Left QRD set-up : set pin 8 (AN14) as analog input------------------------------
    _CH0SA = 0b0000; // positive input
    _CH0NA = 0b00; // negative input
    _TRISA3 = 1; // set pin 8 to input
    _ANSA3 = 1; // set pin 8 to analog
    
    // Center QRD set-up : set pin 7 (AN13) as analog input----------------------------
    _CH0SA = 0b0000; // positive input
    _CH0NA = 0b00; // negative input
    _TRISA2 = 1; // set pin 7 to input
    _ANSA2 = 1; // set pin 7 to analog

    // Intersection QRD set-up : set pin 15 (AN12) as analog input----------------------
    _CH0SA = 0b0000; // positive input
    _CH0NA = 0b00; // negative input
    _TRISB12 = 1; // set pin 15 to input
    _ANSB12 = 1; // set pin 15 to analog
    
     // disable A/D module during configuration------------------------------------
    _ADON = 0;    



    // AD1CON1----------------------------------------------------------------------
    _MODE12 = 0;  // 10-bit resolution
    _FORM = 0;    // unsigned integer output
    _SSRC = 7;    // auto convert
    _ASAM = 1;    // auto sample on
    
    // AD1CON2----------------------------------------------------------------------
    _PVCFG = 0;   // use VDD as positive reference
    _NVCFG = 0;   // use VSS as negative reference
    _BUFREGEN = 1;// store results in buffer corresponding to channel number
    _CSCNA = 1;   // scanning mode
    _SMPI = 1;    // begin new sampling sequence after every sample (1 pin)
    _ALTS = 0;    // sample MUXA only

    // AD1CON3---------------------------------------------------------------------
    _ADRC = 0;    // use system clock
    _SAMC = 1;    // sample every A/D period
    _ADCS = 0x3F; // TAD = 64*TCY

    //set up QRD pins as analog inputs---------------------------------------------
    _CSS4 = 1;  // set AN4 for A/D
    _CSS12 = 1; // set AN12 for A/D
    _CSS13 = 1; // set AN13 for A/D
    _CSS14 = 1; // set AN14 for A/D
    _ADON = 1;  // enable A/D
    
    
    // set motor direction pins as digital outputs----------------------------------
    _TRISB8 = 0; // set RB8 to digital
    _ANSB8 = 0; //Set RB8 as output
    _TRISB9 = 0; // set RB9 to digital
    _ANSB9 = 0; //Set RB9 as output

    
        if ((ADC1BUF4 >= 1 * 309.303 && ADC1BUF0 <= 1 * 309.303) || (ADC1BUF4 <= 1 * 309.303 && ADC1BUF0 >= 1 * 309.303)) { // set threshold to 1 V
            _LATB15 = 1;
        }else {
            _LATB15 = 0;
        }
 

//-------------------------------------------------------------------------


int main(void) {



    _RCDIV = 0;
    
    // set up pin 14 as PWM
    OC1CON1 = 0;
    OC1CON2 = 0;
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1CON1bits.OCTSEL = 0b111;
    OC1CON1bits.OCM = 0b110;
    OC1CON2bits.OCTRIG = 0;
    _OC1IP = 4; // set priority for OC1 interrupt
    _OC1IF = 0; // clear OC1 flag
    _OC1IE = 1; // enable interrupt on OC1
    
    
    // set pin 2 as digital input
    _ANSA0 = 0;
    _TRISA0 = 1;

    
    // set pin 4 as digital output
    _ANSB0 = 0;
    _TRISB0 = 0;
    
    // set up states
    
    while (1) {
        switch (dir) {
            case (stop):
                if (QRD intersection is black AND QRD right is black AND QRD left is black AND QRD center is white) {
                    dir = straight; // switch to drive straight
                    //code to drive_straight
                }
                break;
            
            case (straight):
                if (L and I are white and R is black) {
                    dir = turn_left;
                    //code to turn left 90 degrees

                }
                else if (L and I are black and R is white) {
                    dir = turn_right;
                    //code to turn right 90 degrees

                }
                else if (I and R are black and L is white) {
                    dir = adjust_right;
                    //code to adjust to the right slightly
                }
            
                else if (I and L are black and R is white) {
                    dir = adjust_left;
                    //code to adjust to the left slightly
                }

                else if (I and R and L and C are black) {
                    dir = stop;
                    //code to stop motors
                }
                break;
                
            case (adjust_left):
                if (L and R are black and C is white) {
                    dir = straight;
                    //code to drive straight
                }
                else if (I and R and L and C are black) {
                    dir = stop;
                    //code to stop
                }
                break;
                
            case (adjust_right):
                if (L and R are black and C is white) {
                    dir = straight;
                    //code to drive straight
                }
                else if (I and R and L and C are black) {
                    dir = stop;
                    //code to stop
                }
                break;
                
            
            case (turn_left):
                if (L and R are black and C is white) {
                    dir = straight;
                    //code to drive_straight
                }
                else if (I and R and L and C are black) {
                    dir = stop;
                    //code to stop
                }
                break;

            case (turn_right):
                if (L and R are black and C is white) {
                    dir = straight;
                    //code to drive_straight
                }
                else if (I and R and L and C are black) {
                    dir = stop;
                    //code to stop
                }
                break;
        }
    }
    
    return 0;
}
//-------------------------------------------------------------------------

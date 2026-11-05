#include "xc.h"
#include "motor.h"
#include <stdlib.h>
#include <math.h>


//#pragma config FNOSC = FRC //8 MHz oscillator


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
    _TRISB8 = 0; // set RB8 to digital output
    _ANSB8 = 0; 
    _TRISB9 = 0; // set RB9 to digital output
    _ANSB9 = 0; 

    qrd_left == ADC1BUF14;
    qrd_right == ADC1BUF4;
    qrd_center == ADC1BUF13;
    qrd_intersection == ADC1BUF12;

 

//-------------------------------------------------------------------------


int main(void) {

// set up states
    
    while (1) {
        switch (dir) {
            case (stop):
                if ((qrd_intersection <= 1 * 309.303) && (qrd_right <= 1 * 309.303) && (qrd_left <= 1 * 309.303) && (qrd_center >= 1 * 309.303)) {
                //Int, Left, Right are black, and Center is white    
                    dir = straight; // switch to drive straight
                    //code to drive_straight
                }
                break;
            
            case (straight):
                if ((qrd_left >= 1 * 309.303) && (qrd_intersection >= 1 * 309.303) && (qrd_right <= 1 * 309.303)) {
                //Left and Int are white, and Right is black
                    dir = turn_left; //start turning left
                    //code to turn left 90 degrees

                }
                else if ((qrd_left <= 1 * 309.303) && (qrd_intersection <= 1 * 309.303) && (qrd_right >= 1 * 309.303)) {
                //Left and Int are black, and Right is white
                    dir = turn_right; //start turning right
                    //code to turn right 90 degrees

                }
                else if ((qrd_intersection <= 1 * 309.303) && (qrd_right <= 1 * 309.303) && (qrd_left >= 1 * 309.303)) {
                    //Int and Right are black, and Left is white
                    dir = adjust_left; //start turning to the left
                    //code to adjust to the left slightly
                }
            
                else if ((qrd_intersection <= 1 * 309.303) && (qrd_left <= 1 * 309.303) && (qrd_right >= 1 * 309.303)) {
                //Int and Left are black, and Right is white
                    dir = adjust_right; //start turning to the right
                    //code to adjust to the right slightly
                }

                else if ((qrd_intersection <= 1 * 309.303) && (qrd_right <= 1 * 309.303) && (qrd_left <= 1 * 309.303) && (qrd_center <= 1 * 309.303)) {
                //Int, Left, Right, and Center are black
                    dir = stop; //stop the motors
                    //code to stop motors
                }
                break;
                
            case (adjust_left):
                if ((qrd_left <= 1 * 309.303) && (qrd_right <= 1 * 309.303) && (qrd_center >= 1 * 309.303)) {
                //Left and Right are black, and Center is white   
                    dir = straight; //switch to drive straight
                    //code to drive straight
                }
                else if ((qrd_intersection <= 1 * 309.303) && (qrd_right <= 1 * 309.303) && (qrd_left <= 1 * 309.303) && (qrd_center <= 1 * 309.303)) {
                //Int, Left, Right, and Center are black    
                    dir = stop; //switch to stop
                    //code to stop
                }
                break;
                
            case (adjust_right):
                if ((qrd_left <= 1 * 309.303) && (qrd_right <= 1 * 309.303) && (qrd_center >= 1 * 309.303)) {
                //Left and Right are black, and Center is white   
                    dir = straight; //switch to drive straight
                    //code to drive straight
                }
                else if ((qrd_intersection <= 1 * 309.303) && (qrd_right <= 1 * 309.303) && (qrd_left <= 1 * 309.303) && (qrd_center <= 1 * 309.303)) {
                //Int, Left, Right, and Center are black    
                    dir = stop;
                    //code to stop
                }
                break;
                
            
            case (turn_left):
                if ((qrd_left <= 1 * 309.303) && (qrd_right <= 1 * 309.303) && (qrd_center >= 1 * 309.303)) {
                //Left and Right are black, and Center is white    
                    dir = straight;
                    //code to drive_straight
                }
                else if ((qrd_intersection <= 1 * 309.303) && (qrd_right <= 1 * 309.303) && (qrd_left <= 1 * 309.303) && (qrd_center <= 1 * 309.303)) {
                //Int, Left, Right, and Center are black   
                    dir = stop;
                    //code to stop
                }
                break;

            case (turn_right):
                if ((qrd_left <= 1 * 309.303) && (qrd_right <= 1 * 309.303) && (qrd_center >= 1 * 309.303)) {
                //Left and Right are black, and Center is white    
                    dir = straight;
                    //code to drive_straight
                }
                else if ((qrd_intersection <= 1 * 309.303) && (qrd_right <= 1 * 309.303) && (qrd_left <= 1 * 309.303) && (qrd_center <= 1 * 309.303)) {
                //Int, Left, Right, and Center are black   
                    dir = stop;
                    //code to stop
                }
                break;
        }
    }
    
    return 0;
}
//-------------------------------------------------------------------------

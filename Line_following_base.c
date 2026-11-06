#include "xc.h"
#include "motor.h"
#include "Line_following_base.h"

void qrd_init(void) {
    // Right QRD set-up : set pin 6 (AN4) as analog input-----------------------------
    _TRISB2 = 1; // set pin 6 to input
    _ANSB2 = 1; // set pin 6 to analog

    // Left QRD set-up : set pin 8 (AN14) as analog input------------------------------
    _TRISA3 = 1; // set pin 8 to input
    _ANSA3 = 1; // set pin 8 to analog
    
    // Center QRD set-up : set pin 7 (AN13) as analog input----------------------------
    _TRISA2 = 1; // set pin 7 to input
    _ANSA2 = 1; // set pin 7 to analog

    // Intersection QRD set-up : set pin 15 (AN12) as analog input----------------------
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
    _SMPI = 3;    // begin new sampling sequence after every sample (4 pins - 1)
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
    // _TRISB8 = 0; // set RB8 to digital output
    // _ANSB8 = 0; 
    // _TRISB9 = 0; // set RB9 to digital output
    // _ANSB9 = 0; 
}

//-------------------------------------------------------------------------
enum m1_states dir = stop;
#define THRESH 1000
#define BLACK(x) ((x) < THRESH)
#define WHITE(x) ((x) > THRESH)
void line_following_fsm(void) {

    int qrd_left = ADC1BUF14;
    int qrd_right = ADC1BUF4;
    int qrd_center = ADC1BUF13;
    int qrd_int = ADC1BUF12;

    // Failsafe stop if all sensors are black
    if (BLACK(qrd_left) && BLACK(qrd_right) && BLACK(qrd_center) && BLACK(qrd_int)) {
        dir = stop;
        drive(0, 0);
        return;
    }

    switch (dir) {
        case (stop):
            if ((BLACK(qrd_int)) && (BLACK(qrd_right)) && (BLACK(qrd_left)) && (WHITE(qrd_center))) {
            //Int, Left, Right are black, and Center is white    
                dir = straight; // switch to drive straight
                //code to drive_straight
                drive(0.5, 0.5);
            }
            break;
        
        case (straight):
            if ((WHITE(qrd_left)) && (WHITE(qrd_int)) && (BLACK(qrd_right))) {
            //Left and Int are white, and Right is black
                dir = turn_left; //start turning left
                drive(0.3, 0.5);

            }
            else if ((BLACK(qrd_left)) && (BLACK(qrd_int)) && (WHITE(qrd_right))) {
            //Left and Int are black, and Right is white
                dir = turn_right; //start turning right
                drive(0.5, 0.3);
            }
            else if ((BLACK(qrd_int)) && (BLACK(qrd_right)) && (WHITE(qrd_left))) {
                //Int and Right are black, and Left is white
                dir = adjust_left; //start turning to the left
                drive(0.3, 0.5);
            }
        
            else if ((BLACK(qrd_int)) && (BLACK(qrd_left)) && (WHITE(qrd_right))) {
            //Int and Left are black, and Right is white
                dir = adjust_right; //start turning to the right
                drive(0.5, 0.3);
            }
            break;
            
        case (adjust_left):
        case (adjust_right):
        case (turn_left):
        case (turn_right):
            if ((BLACK(qrd_left)) && (BLACK(qrd_right)) && (WHITE(qrd_center))) {
            //Left and Right are black, and Center is white   
                dir = straight; //switch to drive straight
                drive(0.5, 0.5);
            }
            break;
    }
}
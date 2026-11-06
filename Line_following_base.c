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
enum m1_states prev_state = stop;
#define THRESH 1000
#define BLACK(x) ((x) > THRESH)
#define WHITE(x) ((x) < THRESH)
#define full_speed 0.5
#define adjust_speed 0.3
#define turn_speed 0
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
            prev_state = stop;
            if ((BLACK(qrd_int)) && (BLACK(qrd_left)) && (WHITE(qrd_center)) &&  (BLACK(qrd_right))) {
            //Int, Left, Right are black, and Center is white    
                dir = straight; // switch to drive straight
                //code to drive_straight
                drive(full_speed, full_speed);
            }
            else if ((WHITE(qrd_left)) && (WHITE(qrd_center)) && (BLACK(qrd_right))) {
            //Left and Int are white, and Right is black
                dir = adjust_left; //small adjustment left
                drive(adjust_speed, full_speed); // slow left wheel to slight turn left

            }
            else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (WHITE(qrd_right))) {
            //Left and Int are black, and Right is white
                dir = turn_right; //large adjustment right
                drive(full_speed, turn_speed); // stop right wheel to hard turn right
            }
            else if ((WHITE(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
                //Int and Right are black, and Left is white
                dir = turn_left; //large adjustment left
                drive(turn_speed, full_speed); // stop left wheel to hard turn left
            }
        
            else if ((BLACK(qrd_left)) && (WHITE(qrd_center)) && (WHITE(qrd_right))) {
            //Int and Left are black, and Right is white
                dir = adjust_right; //small adjustment right
                drive(full_speed, adjust_speed); // slow right wheel to slight turn right
            }
            break;
        
        case (straight):
            prev_state = straight;
            if ((WHITE(qrd_left)) && (WHITE(qrd_center)) && (BLACK(qrd_right))) {
            //Left and Int are white, and Right is black
                dir = adjust_left; //small adjustment left
                drive(adjust_speed, full_speed); // slow left wheel to slight turn left

            }
            else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (WHITE(qrd_right))) {
            //Left and Int are black, and Right is white
                dir = turn_right; //large adjustment right
                drive(full_speed, turn_speed); // stop right wheel to hard turn right
            }
            else if ((WHITE(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
                //Int and Right are black, and Left is white
                dir = turn_left; //large adjustment left
                drive(turn_speed, full_speed); // stop left wheel to hard turn left
            }
        
            else if ((BLACK(qrd_left)) && (WHITE(qrd_center)) && (WHITE(qrd_right))) {
            //Int and Left are black, and Right is white
                dir = adjust_right; //small adjustment right
                drive(full_speed, adjust_speed); // slow right wheel to slight turn right
            }
            else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
                dir = stop; // stop when completely missing the line
                drive(0, 0);
            }
            break;
            
        case (adjust_left):
            prev_state = adjust_left;
            if ((BLACK(qrd_left)) && (WHITE(qrd_center)) &&  (BLACK(qrd_right))) {
                //Left and Int are white, and Right is black
                dir = straight; //small adjustment left
                drive(full_speed, full_speed); // slow left wheel to slight turn left

            }
            else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (WHITE(qrd_right))) {
            //Left and Int are black, and Right is white
                dir = turn_right; //large adjustment right
                drive(full_speed, turn_speed); // stop right wheel to hard turn right
            }
            else if ((WHITE(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
                //Int and Right are black, and Left is white
                dir = turn_left; //large adjustment left
                drive(turn_speed, full_speed); // stop left wheel to hard turn left
            }
        
            else if ((BLACK(qrd_left)) && (WHITE(qrd_center)) && (WHITE(qrd_right))) {
            //Int and Left are black, and Right is white
                dir = adjust_right; //small adjustment right
                drive(full_speed, adjust_speed); // slow right wheel to slight turn right
            }
            else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
                dir = stop; // stop when completely missing the line
                drive(0, 0);
            }
            break;

        case (adjust_right):
            prev_state = adjust_right;
            if ((WHITE(qrd_left)) && (WHITE(qrd_center)) && (BLACK(qrd_right))) {
            //Left and Int are white, and Right is black
                dir = adjust_left; //small adjustment left
                drive(adjust_speed, full_speed); // slow left wheel to slight turn left

            }
            else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (WHITE(qrd_right))) {
            //Left and Int are black, and Right is white
                dir = turn_right; //large adjustment right
                drive(full_speed, turn_speed); // stop right wheel to hard turn right
            }
            else if ((WHITE(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
                //Int and Right are black, and Left is white
                dir = turn_left; //large adjustment left
                drive(turn_speed, full_speed); // stop left wheel to hard turn left
            }
        
            else if ((BLACK(qrd_left)) && (WHITE(qrd_center)) &&  (BLACK(qrd_right))) {
            //Int and Left are black, and Right is white
                dir = straight; //small adjustment right
                drive(full_speed, full_speed); // slow right wheel to slight turn right
            }
            else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
                dir = stop; // stop when completely missing the line
                drive(0, 0);
            }
            break;

        case (turn_left):
            prev_state = turn_left;
            if ((WHITE(qrd_left)) && (WHITE(qrd_center)) && (BLACK(qrd_right))) {
            //Left and Int are white, and Right is black
                dir = adjust_left; //small adjustment left
                drive(adjust_speed, full_speed); // slow left wheel to slight turn left

            }
            else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (WHITE(qrd_right))) {
            //Left and Int are black, and Right is white
                dir = turn_right; //large adjustment right
                drive(full_speed, turn_speed); // stop right wheel to hard turn right
            }
            else if ((BLACK(qrd_left)) && (WHITE(qrd_center)) &&  (BLACK(qrd_right))) {
                dir = straight; // return to straight
                drive(full_speed, full_speed); // stop left wheel to hard turn left
            }
        
            else if ((BLACK(qrd_left)) && (WHITE(qrd_center)) && (WHITE(qrd_right))) {
            //Int and Left are black, and Right is white
                dir = adjust_right; //small adjustment right
                drive(full_speed, adjust_speed); // slow right wheel to slight turn right
            }
            else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
                dir = stop; // stop when completely missing the line
                drive(0, 0);
            }
        case (turn_right):
            prev_state = turn_right;
            if ((WHITE(qrd_left)) && (WHITE(qrd_center)) && (BLACK(qrd_right))) {
            //Left and Int are white, and Right is black
                dir = adjust_left; //small adjustment left
                drive(adjust_speed, full_speed); // slow left wheel to slight turn left

            }
            else if ((BLACK(qrd_left)) && (WHITE(qrd_center)) &&  (BLACK(qrd_right))) {
                dir = straight; // return to stright
                drive(full_speed, full_speed); // stop right wheel to hard turn right
            }
            else if ((WHITE(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
                //Int and Right are black, and Left is white
                dir = turn_left; //large adjustment left
                drive(turn_speed, full_speed); // stop left wheel to hard turn left
            }
        
            else if ((BLACK(qrd_left)) && (WHITE(qrd_center)) && (WHITE(qrd_right))) {
            //Int and Left are black, and Right is white
                dir = adjust_right; //small adjustment right
                drive(full_speed, adjust_speed); // slow right wheel to slight turn right
            }
            else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
                dir = stop; // stop when completely missing the line
                drive(0, 0);
            };
    }
}
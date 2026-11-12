#include <xc.h>
#include "motor.h"
#include "Canyon_navigation_base.h"

void sharp_init(void) {
    // right sharp set-up (pin3, AN1, RA1)
    _TRISA1 = 1; // set pin 3 to input
    _ANSA1 = 1; // set pin 3 to analog
    
    // left sharp set-up (pin2, AN0, RA0)
    _TRISA0 = 1; // set pin 2 to input
    _ANSA0 = 1; // set pin 2 to analog


    // analog configuration
    _ADON = 0; // disable analog during set-up
    
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
    _SMPI = 1;    // begin new sampling sequence after every sample (2 pins - 1)
    _ALTS = 0;    // sample MUXA only

    // AD1CON3---------------------------------------------------------------------
    _ADRC = 0;    // use system clock
    _SAMC = 1;    // sample every A/D period
    _ADCS = 0x3F; // TAD = 64*TCY

    //set up sharp pins as analog inputs---------------------------------------------
    _CSS4 = 0;  // remove AN4 for A/D
    _CSS12 = 0; // remove AN12 for A/D
    _CSS13 = 0; // remove AN13 for A/D
    _CSS14 = 0; // remove AN14 for A/D
     _CSS0 = 1;  // set AN0 for A/D
    _CSS1 = 1; // set AN1 for A/D
    _CSS9 = 1; // set AN9 for A/D


//This should only work if the the bits for the qrd and any other analog is turned off. Should they all be powered at once?

    _ADON = 1;  // enable A/D
}

enum m1_states dir = straight;
enum m1_states prev_state = idle;
#define SIDE_THRESH 500
#define FRONT_THRESH 500
#define SIDE_WALL(x) ((x) > SIDE_THRESH)
#define FRONT_WALL(x) ((x) > FRONT_THRESH)
#define full_speed 0.8
#define adjust_speed 0.4
#define turn_speed 0.2

void canyon_navigation_fsm(void) {
    int sharp_left = ADC1BUF0;
    int sharp_right = ADC1BUF1;
    int ir_front = ADC1BUF9;

    switch (dir) {
        case (straight):
            prev_state = straight;
            if ((FRONT_WALL(ir_front)) && (SIDE_WALL(sharp_right))) {
            //runs into a wall and has a wall on the right
                dir = left; //turn 90 degrees left
                drive(turn_speed, full_speed); // stop left wheel to hard turn left
            }
            else if ((FRONT_WALL(ir_front)) && (SIDE_WALL(sharp_left))) {
                //runs into a wall and has a wall on the left
                dir = right; //turn 90 degrees right
                drive(full_speed, turn_speed); // stop right wheel to hard turn right
            }
            break;
        

        case (left):
            prev_state = left;
            if (stepsTaken > 500) {
            //90 degree left turn
                dir = straight; //complete turn and continue straight
                drive(full_speed, full_speed);

            }

        case (right):
            prev_state = right;
            if (steps2Taken > 500) {
            //90 degree right turn
                dir = straight; //complete turn and continue straight
                drive(full_speed, full_speed);

            }
        case (idle):
            prev_state = idle;
            dir = straight; //complete turn and continue straight
            drive(full_speed, full_speed);

         }

}


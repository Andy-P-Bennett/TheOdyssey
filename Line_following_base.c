#include "xc.h"
#include "motor.h"
#include "Line_following_base.h"


//-------------------------------------------------------------------------
static enum m1_states dir = straight;
static enum m1_states prev_state = stop;
#define THRESH 500
#define BLACK(x) ((x) > THRESH)
#define WHITE(x) ((x) < THRESH)
#define full_speed 0.8
#define adjust_speed 0.4
#define turn_speed 0.2

void line_following_fsm(void) {
    int qrd_left = ADC1BUF14;
    int qrd_right = ADC1BUF4;
    int qrd_center = ADC1BUF13;
    int qrd_int = ADC1BUF12;

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
            // else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
            //     dir = stop; // stop when completely missing the line
            //     drive(0, 0);
            // }
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
            // else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
            //     dir = stop; // stop when completely missing the line
            //     drive(0, 0);
            // }
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
            // else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
            //     dir = stop; // stop when completely missing the line
            //     drive(0, 0);
            // }
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
            // else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
            //     dir = stop; // stop when completely missing the line
                drive(0, 0);
            // }
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
            // else if ((BLACK(qrd_left)) && (BLACK(qrd_center)) && (BLACK(qrd_right))) {
            //     dir = stop; // stop when completely missing the line
            //     drive(0, 0);
            // };
    }
}
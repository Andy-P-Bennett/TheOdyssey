#include <xc.h>
// #include "main.c"
#include "motor.h"
#include "Canyon_navigation_base.h"


static enum can_states can = idle;
static enum can_states prev_state = idle;
#define SIDE_THRESH 112 // 5*(112/1024) = .55V, which is about 30 cm away from the wall, per psc spec sheet
#define FRONT_THRESH 69 // 5*(69/1024) = .336V, which is about 20 cm
#define SIDE_WALL(x) ((x) > SIDE_THRESH)
#define FRONT_WALL(x) ((x) > FRONT_THRESH)
#define full_speed 1.6
#define turn_speed 1.0
#define steps_in_turn 480 // set for 1/4 step

int canyon_navigation_fsm(int steps) {
    int sharp_front = ADC1BUF0;
    int sharp_right = ADC1BUF1;

    switch (can) {
        case (forward):
            prev_state = forward;
            if ((FRONT_WALL(sharp_front)) && (SIDE_WALL(sharp_right))) {
            //runs into a wall and has a wall on the right
                can = left; //turn 90 degrees left
                drive(-turn_speed, turn_speed); //turn left
                return 1; //reset steps to 0
            }
            else if ((FRONT_WALL(sharp_front)) && (!SIDE_WALL(sharp_right))) {
                //runs into a wall and has a wall on the left
                can = right; //turn 90 degrees right
                drive(turn_speed, -turn_speed); //turn right
                return 1; //reset steps to 0;
            }
        return 0; //keep counting steps until 90 degree turn is complete

        case (left):
            prev_state = left;
            if (steps > steps_in_turn) {
            //90 degree left turn
                can = forward; //complete turn and continue forward
                drive(full_speed, full_speed);
                return 1; //reset steps to 0
            }
        return 0; //keep counting steps until 90 degree turn is complete

        case (right):
            prev_state = right;
            if (steps > steps_in_turn) {
            //90 degree right turn
                can = forward; //complete turn and continue forward
                drive(full_speed, full_speed);
                return 1; //reset steps to 0
            }
        return 0; //keep counting steps until 90 degree turn is complete

        case (idle):
            prev_state = idle;
            can = forward; //complete turn and continue forward
            drive(full_speed, full_speed);
            return 1; //reset steps to 0
        }
        return 0; //keep counting steps until 90 degree turn is complete
}

#define DIFF_THRESH 100 // arbitrary number i picked, change later
#define WALL_THRESH 179 // check this number to ~30 cm?
static enum {straight, adjust_right, adjust_left, turn} lr_speed = straight;

int lr_can_nav(int steps) {
    int sharp_left = ADC1BUF0;
    int sharp_right = ADC1BUF1;
// enum m1_states {stop, straight, adjust_left, adjust_right, turn_left, turn_right};
    switch (lr_speed) {
        case (straight):
            if (sharp_left - sharp_right > DIFF_THRESH) {
                lr_speed = adjust_right;
                drive(turn_speed, full_speed);
                // add code to change motor speeds
                return 1; // don't reset counter
            }
            else if (sharp_right - sharp_left > DIFF_THRESH) {
                lr_speed = adjust_left;
                drive(turn_speed, full_speed);
                //add code to change motor speeds
                return 1; // don't reset counter
            }
            else if (sharp_right < WALL_THRESH) {
                lr_speed = turn; // to the right
                drive(turn_speed, -turn_speed);
                return 1;
            }
            else if (sharp_left < WALL_THRESH) {
                lr_speed = turn; // to the left
                drive(-turn_speed, turn_speed);
                return 1;
            }
            return 0;

        case (adjust_right):
            if (steps > steps_in_turn) {
                lr_speed = forward; //complete turn and continue forward
                drive(full_speed, full_speed);
                return 1; //reset steps to 0
            }
            else if (sharp_right - sharp_left > DIFF_THRESH) {
                lr_speed = adjust_left;
                drive(turn_speed, full_speed);
                return 1;
            }
            return 0;
        
        case (adjust_left):
            if (steps > steps_in_turn) {
                lr_speed = forward;
                drive(full_speed, full_speed);
                return 1;
            }
            else if (sharp_left - sharp_right > DIFF_THRESH) {
                lr_speed = adjust_right;
                drive(full_speed, turn_speed);
                return 1;
            }
            return 0;
        
        case (turn):
            if (steps > steps_in_turn) {
            //90 degree right turn
                lr_speed = forward; //complete turn and continue forward
                drive(full_speed, full_speed);
                return 1; //reset steps to 0
            }
            return 0;
    }
}
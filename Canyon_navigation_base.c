#include <xc.h>
// #include "main.c"
#include "motor.h"
#include "Canyon_navigation_base.h"


static enum can_states can = idle;
static enum can_states prev_state = idle;
#define SIDE_THRESH 217 // 3.3*(217/1024) = .7V, which is about 20cm away from the wall, per psc spec sheet
#define FRONT_THRESH 500
#define SIDE_WALL(x) ((x) > SIDE_THRESH)
#define FRONT_WALL(x) ((x) > FRONT_THRESH)
#define full_speed 1.6
#define turn_speed 0.5

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
            if (steps > 300) {
            //90 degree left turn
                can = forward; //complete turn and continue forward
                drive(full_speed, full_speed);
                return 1; //reset steps to 0
            }
        return 0; //keep counting steps until 90 degree turn is complete

        case (right):
            prev_state = right;
            if (steps > 300) {
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


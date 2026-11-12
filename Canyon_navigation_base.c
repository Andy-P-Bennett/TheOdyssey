#include <xc.h>
#include "main.c"
#include "motor.h"
#include "Canyon_navigation_base.h"


enum can_states dir = straight;
enum can_states prev_state = idle;
#define SIDE_THRESH 500 // 3.3*(500/1024) = 1.61V, which is about 8cm away from the wall, per psc spec sheet
#define FRONT_THRESH 500
#define SIDE_WALL(x) ((x) > SIDE_THRESH)
#define FRONT_WALL(x) ((x) > FRONT_THRESH)
#define full_speed 0.8
#define turn_speed 0.5

void canyon_navigation_fsm(void) {
    int sharp_front = ADC1BUF0;
    int sharp_right = ADC1BUF1;

    switch (dir) {
        case (straight):
            prev_state = straight;
            if ((FRONT_WALL(sharp_front)) && (SIDE_WALL(sharp_right))) {
            //runs into a wall and has a wall on the right
                dir = left; //turn 90 degrees left
                drive(-turn_speed, turn_speed); //turn left
            }
            else if ((FRONT_WALL(sharp_front)) && (!SIDE_WALL(sharp_right))) {
                //runs into a wall and has a wall on the left
                dir = right; //turn 90 degrees right
                drive(turn_speed, -turn_speed); //turn right
            }
            break;
        

        case (left):
            prev_state = left;
            if (stepsTaken > 400) {
            //90 degree left turn
                dir = straight; //complete turn and continue straight
                drive(full_speed, full_speed);

            }

        case (right):
            prev_state = right;
            if (stepsTaken > 400) {
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


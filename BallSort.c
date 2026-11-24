#include <xc.h>
#include "BallSort.h"
#include "motor.h"
#include "Definitions.h"

#define TIME_THRESH 100 // I haven't done the calculations for this or set the timere up yet +++unique to this function+++
#define steps_to_return 500 // I don't know what this should be, I wasn't able to measure it


enum steps cur_step_col = stop_movement; // state of collection
int CollectSample(int steps_taken, int time) {
    switch (cur_step_col) {
        case (stop_movement):
            drive(turn_speed, -turn_speed); // right turn
            cur_step_col = center_robot; // move robot forward to center between return the troughs
            return 1; // reset step counting

        case(center_robot):
            if (steps_taken >= steps_in_turn) {
                drive(full_speed, full_speed); // move straight
                cur_step_col = turn_to_objective; // change to state of turning
                return 1; // reset step counting
            }
            return 0;
        
        case (turn_to_objective):
            if (steps_taken >= steps_in_turn) {
                drive(full_speed, full_speed); // move straight to collect sample
                cur_step_col = drive_to_objective; // change to state of forward motion
                return 1; // reset step counting
            }
            return 0;

        case (drive_to_objective):
            if (ADC1BUF0 >= close_sharp_thresh) { // check sharp sensor to determine distance to drop
                drive(0, 0);
                cur_step_col = wait;
                return 1; // reset timer
            }
            return 0;
        
        case (wait):
            if (time >= TIME_THRESH) {
                drive(-full_speed, -full_speed); // reverse robot
                cur_step_col = return_to_line; // change to state of returning to the line
                return 1; // reset step counting
            }
            return 0;
        
        case (drive_to_line):
            if (ADC1BUF13 >= line_qrd_thresh) {
                drive(-turn_speed, turn_speed); // turn left, this could be adjusted so the robot roughly rotates about the center qrd
                cur_step_col = reorientate;
                return 1;
            }
            return 0;
        
        case (reorientate):
            return return_to_line(left, steps_taken);
    }
}

// Adam 11/24
enum steps cur_step_ret = init; // state of sample return
int ReturnSample(int steps_taken) {
    int sample_white;
    switch (cur_step_ret) {
        case (init):
            drive(full_speed, full_speed); // move straight to return sample
            cur_step_ret = center_robot; // change to state of turning
            if(ADC1BUF11 >= sample_qrd_thresh) {sample_white = 1;}
            else{sample_white = 0;}
            return 0;

        case (stop_movement):
            if (!sample_white) { // check sample
                drive(-turn_speed, turn_speed); // left turn - only 
            }
            else {drive(turn_speed, -turn_speed);} // right turn
            cur_step_ret = turn_to_objective; // change to state of turning
            return 1; // reset step counting

        case(center_robot):
            if (steps_taken >= steps_in_turn) {
                cur_step_ret = stop_movement; // change to state of turning
                return 1; // reset step counting
            }
            return 0;
        
        case (turn_to_objective):
            if (steps_taken >= steps_in_turn) {
                drive(full_speed, full_speed); // move straight to drop sample
                cur_step_ret = drive_to_objective; // change to state of forward motion
                return 1; // reset step counting
            }
            return 0;

        case (drive_to_objective):
            if (steps_taken >= steps_to_return) {
                drive(0, 0);
                cur_step_ret = drive_to_line;
                return 1; // reset timer
            }
            return 0;
        
        case (drive_to_line):
            if (ADC1BUF13 >= line_qrd_thresh) {
                drive(full_speed, full_speed); // turn left, this could be adjusted so the robot roughly rotates about the center qrd
                cur_step_ret = reorientate;
                return 1;
            }
            return 0;
        
        case  (reorientate):
            if (sample_white) {return return_to_line(right, steps_taken);}
            else {return return_to_line(left, steps_taken);}
    }
}
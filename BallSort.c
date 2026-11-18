#include <xc.h>
#include "BallSort.h"
#include "motor.h"

#define SAMPLE_QRD_THRESH 500 // 500 for now
#define CENTER_QRD_THRESH 500 // should match the line_following_base.c thresholds
#define SHARP_THRESH 200 // this definitely needs to be higher, I'm just not sure how close we need to get
#define TIME_THRESH 100 // I haven't done the calculations for this or set the timere up yet
#define steps_in_turn 480
#define steps_to_return 500 // I don't know what this should be, I wasn't able to measure it
#define speed 1.0

// need to include a line in the main function to run the CollectSample function when the right IR sensor is high
enum steps cur_step_col = stop_movement; // state of collection
int CollectSample(int steps_taken, int time) {
    switch (cur_step_col) {
        case (stop_movement):
            drive(speed, -speed); // right turn
            cur_step_col = turn_to_objective; // change to state of turning
            return 1; // reset step counting
        
        case (turn_to_objective):
            if (steps_taken >= steps_in_turn) {
                drive(speed, speed); // move straight to collect sample
                cur_step_col = drive_to_objective; // change to state of forward motion
                return 1; // reset step counting
            }
            return 0;

        case (drive_to_objective):
            if (ADC1BUF0 >= SHARP_THRESH) { // check sharp sensor to determine distance to drop
                drive(0, 0);
                cur_step_col = wait;
                return 1; // reset timer
            }
            return 0;
        
        case (wait):
            if (time >= TIME_THRESH) {
                drive(-speed, -speed); // reverse robot
                cur_step_col = return_to_line; // change to state of returning to the line
                return 1; // reset step counting
            }
            return 0;
        
        case (return_to_line):
            if (ADC1BUF13 >= CENTER_QRD_THRESH) {
                drive(-speed, speed); // turn left, this could be adjusted so the robot roughly rotates about the center qrd
                cur_step_col = reorientate;
                return 1;
            }
            return 0;
        
        case (reorientate):
            if (steps_taken >= steps_in_turn) {
                drive(speed, speed);
                return 0;
            }
            return 0;
    }
}

enum steps cur_step_ret = stop_movement; // state of sample return
int ReturnSample(int steps_taken, int time) {
    switch (cur_step_ret) {
        case (stop_movement):
            if (ADC1BUF11 >= SAMPLE_QRD_THRESH) { // check sample
                drive(-speed, speed); // left turn
            }
            else {drive(speed, -speed);} // right turn
            cur_step_ret = turn_to_objective; // change to state of turning
            return 1; // reset step counting
        
        case (turn_to_objective):
            if (steps_taken >= steps_in_turn) {
                drive(speed, speed); // move straight to collect sample
                cur_step_ret = drive_to_objective; // change to state of forward motion
                return 1; // reset step counting
            }
            return 0;

        case (drive_to_objective):
            if (steps_taken >= steps_to_return) {
                drive(0, 0);
                cur_step_ret = wait;
                return 1; // reset timer
            }
            return 0;
        
        case (wait):
            if (time >= TIME_THRESH) {
                drive(-speed, -speed); // reverse robot
                cur_step_ret = return_to_line; // change to state of returning to the line
                return 1; // reset step counting
            }
            return 0;
        
        case (return_to_line):
            if (ADC1BUF13 >= CENTER_QRD_THRESH) {
                drive(-speed, speed); // turn left, this could be adjusted so the robot roughly rotates about the center qrd
                cur_step_ret = reorientate;
                return 1;
            }
            return 0;
        
        case (reorientate):
            if (steps_taken >= steps_in_turn) {
                drive(speed, speed);
                return 0;
            }
            return 0;
    }
}
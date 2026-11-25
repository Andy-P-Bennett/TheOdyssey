#define stepsize 4 // 1/4 step

//motor values
#define steps_in_turn 480 // this is for a pivot turn
#define steps_in_curve  480 // this is for a wide turn
#define turn_speed 0.8
#define full_speed 1.0

// line following speeds (values from the working milestone, kept separate from turn_speed/full_speed)
#define lf_full_speed   0.8
#define lf_adjust_speed 0.4 // inner wheel on a small adjustment (50%)
#define lf_turn_speed   0.2 // inner wheel on a hard turn (25%)

//threshold values
#define line_qrd_thresh 500
#define sample_qrd_thresh 800 // white ball goes low
#define front_sharp_thresh 202 // 0.65V / 3.3V = (207 / 1024), 20 cm
// #define right_sharp_thresh 170 // 3.3 * (130 / 1024) = .42V, which is about 30 cm away from the wall, per psc spec sheet
#define right_sharp_thresh 186 // 0.6V / 3.3V = 186 / 1024, 22 cm
#define return_sharp_thresh 326 // 3.3 * (326 / 1024) = 1.05V, 12 cm
#define close_sharp_thresh 50 // this is just a placeholder, this is for either being close enough to park at the end or collect the ball
#define ir_thresh 100 //place holder value
#define highest_sharp_thresh 807 // 2.6V / 3.3V = 807 / 1024


// canyon walls are 58 cm apart
// Sample return is 20 cm from center line
// bot is 16 cm wide
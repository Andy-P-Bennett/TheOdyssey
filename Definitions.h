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
#define sample_qrd_thresh 400 // white ball goes low
#define front_sharp_thresh 104 // 5*(69/1024) = .336V, which is about 20 cm
#define right_sharp_thresh 170 // 5*(112/1024) = .55V, which is about 30 cm away from the wall, per psc spec sheet
#define close_sharp_thresh 50 // this is just a placeholder, this is for either being close enough to park at the end or collect the ball
#define ir_thresh 100 //place holder value
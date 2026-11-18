//motor values
#define steps_in_turn 480
#define turn_speed 0.8
#define full_speed 1.0

//threshold values
#define line_qrd_thresh 500
#define sample_qrd_thresh 500 // this may need updating after testing
#define front_sharp_thresh 69 // 5*(69/1024) = .336V, which is about 20 cm
#define right_sharp_thresh 112 // 5*(112/1024) = .55V, which is about 30 cm away from the wall, per psc spec sheet
#define ir_thresh 100 //place holder value
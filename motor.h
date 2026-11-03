#ifndef motor
#define motor
void drive(float speed_left, float speed_right);
    // tank drive style stepper control - give values in rev/s
void drive_mps(float left_ms, float right_ms);
    // conversion layer to drive motors in m/s instead of rev/s
void arcade_drive(float velocity, float turn_rate);
    // velocity in m/s, turn_rate in rad/s - assumed left turn positive 
    // base velocity, +- r*w, where r = IWD/2, depending on L/R
#endif
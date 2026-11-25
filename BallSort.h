#ifndef BallSort
#define BallSort
enum steps{init, stop_movement, center_robot,turn_to_objective, drive_to_objective, wait, drive_to_line, reorientate}; // stop movement initializes the behavior, wait just waits for the ball to drop
int sample_white;
int CollectSample(int, int);
int ReturnSample(int);
#endif // Line_following_base
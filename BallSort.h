#ifndef BallSort
#define BallSort
enum steps{stop_movement, turn_to_objective, drive_to_objective, wait, return_to_line, reorientate}; // stop movement initializes the behavior, wait just waits for the ball to drop
int CollectSample(int, int);
int ReturnSample(int, int);
#endif // Line_following_base
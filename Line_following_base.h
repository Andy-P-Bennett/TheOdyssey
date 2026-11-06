#ifndef Line_following_base
#define Line_following_base
enum m1_states {stop, straight, adjust_left, adjust_right, turn_left, turn_right, find_line}; // adjust is small adjustment and turn is large adjustment
void qrd_init(void);
void line_following_fsm(void);
#endif // Line_following_base
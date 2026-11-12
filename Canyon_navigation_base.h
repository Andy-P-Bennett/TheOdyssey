#ifndef Canyon_navigation_base
#define Canyon_navigation_base
enum can_states {left, right, forward, idle};
int canyon_navigation_fsm(int steps);
#endif //Canyon_navigation_base
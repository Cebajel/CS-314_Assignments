#include "../include/clock.h"

Clock clock;

Clock::Clock(){
    time = 0;
}

unsigned int Clock::get_time() const{
    return time;
}

void Clock::increment() {
    time += 1;
    return;
}

void Clock::decrement() {
    time -= 1;
    return;
}
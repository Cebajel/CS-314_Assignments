#ifndef PROCESS_H_
#define PROCESS_H_

#include "clock.h"
enum Type {CPU, IO};
#include <queue>

class Process {
    private:
    bool _is_complete;
    bool _is_waiting;
    bool _is_blocked;
    bool _is_executing;
    // std::chrono::time_point<std::chrono::high_resolution_clock>
    unsigned int temp_waiting_start;
    unsigned int temp_waiting_end;
    unsigned int temp_blocked_start;
    unsigned int temp_blocked_end;
    unsigned int temp_burst_start;
    unsigned int temp_burst_end;

    public:
    const Clock* clock;
    Type type;
    const int pid;
    std::queue<int> burst_values;
    unsigned int response_time;
    unsigned int pending_burst_time;
    unsigned int blocked_time;
    unsigned int waiting_time;
    unsigned int total_burst_time;
    unsigned int turnaround_time;
    float response_ratio;
    float penalty_ratio;
    unsigned int start;
    unsigned int end;

    Process(int, Clock*, unsigned int, std::queue<int>);
    bool is_complete() const;
    bool is_waiting() const;
    bool is_blocked() const;
    bool is_executing() const;
    void mark_complete();
    void mark_waiting();
    void mark_wait_complete();
    void mark_blocked();
    void mark_unblocked();
    void mark_executing();
    void mark_executing_complete();
    void change_type();
    bool dec_burst_value();
};
#endif
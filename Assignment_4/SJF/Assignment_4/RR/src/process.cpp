#include "../include/process.h"
#include <iostream>

Process::Process(int pid_in, Clock* clock_in, unsigned int strt, std::queue<int> burst_in) : pid(pid_in), clock(clock_in), start(strt) {
    waiting_time = 0;
    total_burst_time = 0;
    blocked_time = 0;
    _is_complete = false;
    _is_waiting = false;
    _is_blocked = false;
    burst_values = burst_in;
    pending_burst_time = burst_values.front();
    burst_values.pop();
    type = CPU;
    response_time = -1;
}

bool Process::is_complete() const {
    return _is_complete;
}

bool Process::is_waiting() const {
    return _is_waiting;
}

bool Process::is_blocked() const {
    return _is_blocked;
}

bool Process::is_executing() const {
    return _is_executing;
}

void Process::mark_complete() {
    end = clock->get_time();
    turnaround_time = (end - start);
    _is_complete = true;
    response_ratio = (waiting_time + total_burst_time)*1.0 / total_burst_time;
    penalty_ratio = total_burst_time*1.0 / (waiting_time + total_burst_time);
    std::cout << "Process PID: "<< pid <<" completed with :\n\t";
    std::cout << "Turnaround time: " << turnaround_time << "\n\t";
    std::cout << "Response time: " << response_time << "\n\t";
    std::cout << "Total Waiting time: " << waiting_time << "\n\t";
    std::cout << "Total Burst time: "<< total_burst_time << "\n\t";
    std::cout << "Total I/O time: "<< blocked_time << "\n\t";
    std::cout << "Total Response ratio: "<< response_ratio << "\n\t";
    std::cout << "Total Penalty ratio: "<< penalty_ratio << "\n\n";
    
    return;
}

void Process::mark_waiting() {
    if(!is_waiting()){
        _is_waiting = true;
        temp_waiting_start = clock->get_time();
    }
    return;
}

void Process::mark_wait_complete() {
    if(is_waiting()){
        _is_waiting = false;
        temp_waiting_end = clock->get_time();
        waiting_time += (temp_waiting_end - temp_waiting_start);
    }
    return;
}

void Process::mark_blocked() {
    if(!is_blocked()){
        _is_blocked = true;
        temp_blocked_start = clock->get_time();
    }
    return;
}

void Process::mark_unblocked() {
    if(is_blocked()){
        _is_blocked = false;
        temp_blocked_end = clock->get_time();
        blocked_time += (temp_blocked_end - temp_blocked_start);
    }
    return;
}

void Process::mark_executing() {
    if(!is_executing()){
        _is_executing = true;
        temp_burst_start = clock->get_time();
    }
    return;
}

void Process::mark_executing_complete() {
    if(is_executing()){
        _is_executing = false;
        temp_burst_end = clock->get_time();
        total_burst_time += (temp_burst_end - temp_burst_start);
    }
    return;
}

void Process::change_type() {
    if(type == CPU) {
        type = IO;
    } else {
        type = CPU;
    }
    return;
}

bool Process::dec_burst_value() {
    pending_burst_time--;
    if (pending_burst_time <= 0) {
        return true;
    }
    return false;
}
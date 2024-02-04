#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include "process.h"
#include "cpu.h"
#include "clock.h"
#include "io.h"

#include <queue>

class Scheduler {
    private:
    std::queue<Process*> *new_queue;
    std::queue<Process*> *ready_queue;
    Cpu* cpu;
    Io* io;
    unsigned int total_waiting_time;
    unsigned int total_blocked_time;
    unsigned int total_burst_time;
    unsigned int total_turnaround_time;
    unsigned int total_response_time;
    const int total_processes;

    public:
    Clock* clock;

    Scheduler(Cpu*, Clock*, std::queue<Process*>*, Io*);
    void enqueue_ready(Process*);
    void dequeue_ready();
    void enqueue_new(Process*);
    void dequeue_new();
    Process* choose_next_process() const;
    void schedule_next_process();
    void start_scheduling();
    void check_new_process();
};
#endif
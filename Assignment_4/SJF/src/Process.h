// Process.h
#ifndef PROCESS_H
#define PROCESS_H

#include <vector>

class Process {
public:
    int pid;
    int arrival_time;
    std::vector<std::pair<int, char>> bursts;
    int curr = 0;

    /* 0 - not done, 1 - ready/in queue/running, 2 - completed */
    int status = 0;

    /* metrics */
    int og_arrival_time;
    int completion_time;
    int turnaround_time;
    int cpu_burst = 0;
    int response_time = -1;
    int waiting_time = 0;
    float penalty_ratio;
};

#endif // PROCESS_H

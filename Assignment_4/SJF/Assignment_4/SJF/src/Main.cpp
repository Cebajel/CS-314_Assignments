// Main.cpp
#include <bits/stdc++.h>
#include "Scheduler.h"
using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: ./<executable> <process.dat file>";
        return 0;
    }

    // load process vector from file
    vector<Process> processes, finalList;
    loadProcessVector(argv[1], processes);

    int cycles = 0;
    // run the scheduler
    finalList = sjf_scheduler(processes, processes.size(), cycles);

    // calculate and print metrics
    calculate_metrics(finalList, cycles);

    return 0;
}

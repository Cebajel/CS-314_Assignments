#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "../include/process.h"
#include "../include/clock.h"
#include "../include/cpu.h"
#include "../include/scheduler.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Please provide path to Process File\n";
        exit(EXIT_SUCCESS);
    }

    Clock* clock = new Clock();
    Cpu* cpu = new Cpu();
    Io* io = new Io();

    queue<Process*>* new_queue = new queue<Process*>;
    ifstream myfile;
    myfile.open(argv[1]);
    string myline;
    int pid = 0;
    if ( myfile.is_open() ) {
        while ( myfile.good() ) {
            getline(myfile, myline);
            stringstream stream(myline);
            queue<int> burst_values;
            while(stream){
                int n;
                stream>>n;
                if (n == -1){
                    break;
                }
                burst_values.push(n);
            }
            int arrival_time = burst_values.front();
            burst_values.pop();
            Process * proc = new Process(pid, clock, arrival_time, burst_values);
            new_queue->push(proc);
            pid++;
        }
    }


    Scheduler* scheduler = new Scheduler(cpu, clock, new_queue, io);
    scheduler->start_scheduling();

    return 0;
}

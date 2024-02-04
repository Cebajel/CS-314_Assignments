// Scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "Process.h"
using namespace std;

int loadProcessVector(char *inputFile, vector<Process> &processes);

vector<Process> sjf_scheduler(vector<Process> processes, int totalProcesses, int &cycles);

void calculate_metrics(vector<Process> finalList, int cycles);

#endif // SCHEDULER_H

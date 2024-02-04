// Scheduler.cpp
#include <bits/stdc++.h>
#include "Scheduler.h"
using namespace std;

bool compareProcessesByPid(const Process &a, const Process &b)
{
    return a.pid < b.pid;
}

void calculate_metrics(vector<Process> finalList, int cycles)
{
    sort(finalList.begin(), finalList.end(), compareProcessesByPid);

    int t = finalList.size();
    float avg_turn = 0.0, avg_res = 0.0, avg_wait = 0.0, avg_pen = 0.0;

    for (Process p : finalList)
    {
        avg_turn += (float)p.turnaround_time;
        avg_wait += (float)p.waiting_time;
        avg_res += (float)p.response_time;
        avg_pen += p.penalty_ratio;

        // Print metrics for each process
        cout << "Process " << p.pid << ":\n";
        cout << "\tArrival Time: " << p.og_arrival_time << endl;
        cout << "\tCompletion Time: " << p.completion_time << endl;
        cout << "\tTurnaround Time: " << p.turnaround_time << endl;
        cout << "\tResponse Time: " << p.response_time << endl;
        cout << "\tWaiting Time: " << p.waiting_time << endl;
        cout << "\tPenalty Ratio: " << p.penalty_ratio << endl;
    }

    // Calculate averages
    avg_turn /= t;
    avg_wait /= t;
    avg_res /= t;
    avg_pen /= t;

    // Print averages
    cout << "\nSystem's Overall Performance:\n";
    cout << "\tSystem Throughput: " << (float)finalList.size() / cycles << "\n";
    cout << "\tTotal Cycles: " << cycles << "\n";
    cout << "\tAverage Turnaround Time: " << avg_turn << endl;
    cout << "\tAverage Waiting Time: " << avg_wait << endl;
    cout << "\tAverage Response Time: " << avg_res << endl;
    cout << "\tAverage Penalty Ratio: " << avg_pen << endl;
}

struct CompareProcess
{
    bool operator()(const Process &a, const Process &b)
    {
        return a.bursts[0].first > b.bursts[0].first;
    }
};

struct CompareArrival
{
    bool operator()(const Process &a, const Process &b)
    {
        return a.arrival_time > b.arrival_time;
    }
};

vector<Process> sjf_scheduler(vector<Process> processes, int totalProcesses, int &cycles)
{
    vector<Process> finalList;
    // gives the process with shortest burst time
    priority_queue<Process, vector<Process>, CompareProcess> processQueue;

    // nextInLine Processes
    priority_queue<Process, vector<Process>, CompareArrival> nextInLine;

    for (Process p : processes)
        nextInLine.push(p);

    int time = nextInLine.top().arrival_time;
    int io_available_time = 0;

    // run until both are not empty
    while (!nextInLine.empty() || !processQueue.empty())
    {
        while (!nextInLine.empty() && nextInLine.top().arrival_time <= time)
        {
            processQueue.push(nextInLine.top());
            nextInLine.pop();
        }

        if (processQueue.empty())
        {
            time = nextInLine.top().arrival_time;
            continue;
        }

        Process process = processQueue.top();
        processQueue.pop();

        // printing stuff and metrics
        // printf("Process %d is running at time %d\n", process.pid, time);

        // only to be updated first time
        if (process.response_time == -1)
            process.response_time = time - process.arrival_time;

        process.waiting_time += time - process.arrival_time;
        // Update time
        time += process.bursts[0].first;
        // Remove CPU burst
        process.bursts.erase(process.bursts.begin());

        // If IO follows, update arrival time
        if (process.bursts.size() > 0)
        {
            // this process will again be available for CPU burst at the below time
            process.arrival_time = max(time, io_available_time) + process.bursts[0].first;
            // printf("Process %d is running at time %d -----type: I/O\n", process.pid, max(time, io_available_time));
            // IO device will next be free at this time
            io_available_time = process.arrival_time;
            // Remove IO burst
            process.bursts.erase(process.bursts.begin());
            nextInLine.push(process);
        }
        else
        {
            process.completion_time = time;
            process.turnaround_time = process.completion_time - process.og_arrival_time;
            // process.waiting_time = process.turnaround_time - process.cpu_burst;
            // cpu_burst /  turnaround
            process.penalty_ratio = (float)(process.cpu_burst) / (process.waiting_time + process.cpu_burst);

            finalList.push_back(process);
            // printf("Process %d completed at time %d\n", process.pid, time);
        }
    }

    cycles = time;
    return finalList;
}

int loadProcessVector(char *inputFile, vector<Process> &processes) {
    FILE *file = fopen(inputFile, "r");
    if (file == NULL)
    {
        cout << "Error opening file.\n";
        return -1;
    }

    // Skip the first three lines
    for (int i = 0; i < 3; ++i)
    {
        char line[200];
        if (fgets(line, sizeof(line), file) != NULL)
        {
            continue;
        }
    }

    int totalProcesses = 0;
    while (true)
    {
        Process newProcess; // Create a new process for each iteration

        // Read arrival time
        if (fscanf(file, "%d", &newProcess.arrival_time) != 1)
            break;

        newProcess.pid = totalProcesses;
        newProcess.og_arrival_time = newProcess.arrival_time;
        int total_burst = 0;
        int n = 0, temp = 0;
        while (true)
        {
            if (fscanf(file, "%d", &temp) == EOF || temp == -1)
                break;

            if (n % 2 == 0)
            {
                newProcess.bursts.push_back({temp, 'C'});
                total_burst += temp;
            }
            else
                newProcess.bursts.push_back({temp, 'I'});

            n++;
        }

        newProcess.cpu_burst = total_burst;
        processes.push_back(newProcess); // Add the new process to the vector
        totalProcesses++;
    }

    fclose(file);

    return totalProcesses;
}
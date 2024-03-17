#include "../include/scheduler.h"
#include <iomanip>
#include <iostream>

Scheduler::Scheduler(Cpu* cpu_in, Clock* clk, std::queue<Process*>* queue_in, Io* io_in): cpu(cpu_in), 
clock(clk), io(io_in), total_processes(queue_in->size()) {
    new_queue = queue_in;
    ready_queue = new std::queue<Process*>;
    enqueue_ready(new_queue->front());
    dequeue_new();
    total_turnaround_time = 0;
    total_blocked_time = 0;
    total_burst_time = 0;
    total_waiting_time = 0;
    total_response_time = 0;
    total_penalty_ratio = 0;
}

void Scheduler::enqueue_ready(Process* proc) {
    ready_queue->push(proc);
    proc->mark_waiting();
    return;
}

void Scheduler::dequeue_ready() {
    if (!ready_queue->empty()){
        ready_queue->front()->mark_wait_complete();
        ready_queue->pop();
    }
    return;
}

void Scheduler::enqueue_new(Process* proc) {
    new_queue->push(proc);
    return;
}

void Scheduler::dequeue_new() {
    if (!new_queue->empty()){
        new_queue->pop();
    }
    return;
}

Process* Scheduler::choose_next_process() const {
    return ready_queue->front();
}

void Scheduler::schedule_next_process() {
    if((!cpu->is_idle())){
        enqueue_ready(cpu->executing_process);
        cpu->unload_process();
    }
    if(!ready_queue->empty()){
        cpu->load_process(choose_next_process());
        if (cpu->executing_process->response_time == -1) {
            cpu->executing_process->response_time = clock->get_time() - cpu->executing_process->start;
        }
        cpu->executing_process = choose_next_process();
        dequeue_ready();
    }
    return;
}

void Scheduler::start_scheduling() {
    unsigned int start = clock->get_time();
    while((!ready_queue->empty()) || (!cpu->is_idle()) || (!new_queue->empty()) || (!io->is_idle())){
        if ((clock->get_time() % TIME_QUANTUM) == 0){
            schedule_next_process();
        }

        clock->increment();
        check_new_process();
        
        Process* temp_process = cpu->executing_process;
        switch(cpu->check()){
            case EXECUTION_REMAINING:
            io->enqueue(temp_process);
            break;

            case EXECUTION_COMPLETE:
            total_turnaround_time += temp_process->turnaround_time;
            total_blocked_time += temp_process->blocked_time;
            total_waiting_time += temp_process->waiting_time;
            total_burst_time += temp_process->total_burst_time;
            total_response_time += temp_process->response_time;
            total_penalty_ratio += temp_process->penalty_ratio;
            break;

            default:
            break;
        }

        temp_process = io->executing_process;
        switch(io->check()){
            case EXECUTION_REMAINING:
            enqueue_ready(temp_process);
            break;

            case EXECUTION_COMPLETE:
            total_turnaround_time += temp_process->turnaround_time;
            total_blocked_time += temp_process->blocked_time;
            total_waiting_time += temp_process->waiting_time;
            total_burst_time += temp_process->total_burst_time;
            total_response_time += temp_process->response_time;
            total_penalty_ratio += temp_process->penalty_ratio;
            break;

            default:
            break;
        }
    }
    unsigned int end = clock->get_time();
    unsigned int total_time_taken = (end-start);
    std::cout << "System Averages and Throughput:\n\t";
    std::cout << "Average Turnaround Time: " << (total_turnaround_time*1.0/total_processes) << std::setprecision(9) << "\n\t";
    std::cout << "Average Waiting Time: " << (total_waiting_time*1.0/total_processes) << std::setprecision(9) << "\n\t";
    std::cout << "Average Response Time: " << (total_response_time*1.0/total_processes) << std::setprecision(9) << "\n\t";
    std::cout << "Average Burst Time: " << (total_burst_time*1.0/total_processes) << std::setprecision(9) << "\n\t";
    std::cout << "Average I/O Time: " << (total_blocked_time*1.0/total_processes) << std::setprecision(9) << "\n\t";
    std::cout << "Average Penalty Ratio: " << (total_penalty_ratio*1.0/total_processes) << std::setprecision(9) << "\n\t";
    std::cout << "System Throughput: " << (total_processes*1.0/total_time_taken) << std::setprecision(9) << "\n\t";
    std::cout << "Total Time Taken: " << (total_time_taken) << std::setprecision(9) << "\n\n";
    std::cout << "Thank You!\n";
}

void Scheduler::check_new_process() {
    while ((!new_queue->empty()) && (new_queue->front()->start <= clock->get_time())) {
        Process* temp_proc = new_queue->front();
        enqueue_ready(temp_proc);
        dequeue_new();
    }
    return;
}
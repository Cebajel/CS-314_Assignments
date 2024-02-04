#include "../include/io.h"

Io::Io(){
    ready_queue = new std::queue<Process*>;
    executing_process = nullptr;
    _is_idle = true;
    idle_time = 0;
}

void Io::enqueue(Process* proc) {
    ready_queue->push(proc);
    // proc->mark_waiting();
    return;
}

void Io::dequeue() {
    // ready_queue->front()->mark_wait_complete();
    ready_queue->pop();
    return;
}

void Io::unload_process() {
    executing_process->mark_unblocked();
    executing_process = nullptr;
    _is_idle = true;
    return;
}

void Io::load_process() {
    Process* proc = ready_queue->front();
    dequeue();
    executing_process = proc;
    _is_idle = false;
    executing_process->mark_blocked();
    return;
}

bool Io::is_idle() const {
    return _is_idle;
}

CHECK_RESULT Io::check() {
    if((executing_process != nullptr) && executing_process->dec_burst_value()) {
        Process* temp_process = executing_process;
        unload_process();
        
        if(!ready_queue->empty()) {
            load_process();
        }

        if (temp_process->burst_values.empty()) {
            temp_process->mark_complete();
            return EXECUTION_COMPLETE;
        } else {
            temp_process->pending_burst_time = temp_process->burst_values.front();
            temp_process->burst_values.pop();
            temp_process->change_type();
            return EXECUTION_REMAINING;
        }
    }

    if(!ready_queue->empty() && is_idle()) {
        load_process();
    }

    if(is_idle()) {
        idle_time++;
    }

    return IDLE;
}
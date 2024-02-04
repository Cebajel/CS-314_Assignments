#include "../include/cpu.h"

Cpu::Cpu() {
    _is_idle = true;
    idle_time = 0;
}

void Cpu::unload_process() {
    executing_process->mark_executing_complete();
    executing_process = nullptr;
    _is_idle = true;
    return;
}

void Cpu::load_process(Process* proc) {
    executing_process = proc;
    proc->mark_executing();
    _is_idle = false;
    return;
}

bool Cpu::is_idle() const {
    return _is_idle;
}

CHECK_RESULT Cpu::check() {
    if(is_idle()) {
        idle_time++;
    }

    if((!is_idle()) && executing_process->dec_burst_value()){
        
        Process* temp_process = executing_process;
        unload_process();
        
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

    return IDLE;
}
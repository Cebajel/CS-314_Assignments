#ifndef IO_H_
#define IO_H_

#include "process.h"
#include<queue>

#ifndef CHECK_RESULT_
#define CHECK_RESULT_
enum CHECK_RESULT {EXECUTION_COMPLETE, EXECUTION_REMAINING, IDLE};
#endif

class Io{
    private:
    bool _is_idle;
    int idle_time;
    std::queue<Process*>* ready_queue;

    public:
    Io();
    Process* executing_process;
    void enqueue(Process*);
    void dequeue();
    void unload_process();
    void load_process();
    bool is_idle() const;
    CHECK_RESULT check();
};
#endif
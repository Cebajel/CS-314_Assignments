#ifndef CPU_H_
#define CPU_H_

#ifndef CHECK_RESULT_
#define CHECK_RESULT_
enum CHECK_RESULT {EXECUTION_COMPLETE, EXECUTION_REMAINING, IDLE};
#endif

#include "process.h"

class Cpu{
    private:
    bool _is_idle;
    int idle_time;

    public:

    Cpu();
    Process* executing_process;
    void unload_process();
    void load_process(Process*);
    bool is_idle() const;
    CHECK_RESULT check();
};
#endif
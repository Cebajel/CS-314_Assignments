#ifndef CLOCK_H_
#define CLOCK_H_

#ifndef TIME_QUANTUM
#define TIME_QUANTUM 1
#endif

class Clock{
    private:
    unsigned int time;

    public:
    Clock();
    unsigned int get_time() const;
    void increment();
    void decrement();
};
#endif
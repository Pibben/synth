#ifndef __CLOCK_H__
#define __CLOCK_H__

class Clock {
public:
    Clock() : mCounter(0) {}
    unsigned int getTick() { return mCounter; }
    void tick() { mCounter++; }
private:
    unsigned int mCounter;
};

#endif
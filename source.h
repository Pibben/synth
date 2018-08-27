#ifndef __SOURCE_H__
#define __SOURCE_H__

#include <stddef.h> //NULL

#include "clock.h"

class Source {
public:
    Source() : mCurrentClock(0xffffffff), mCurrentSample(0.0f) {}
    virtual ~Source() {}
    float getSample(unsigned int clock) {
        if(clock != mCurrentClock) {
            mCurrentClock = clock;
            prepareSample();
        }
        return mCurrentSample;
    }
protected:
    unsigned int mCurrentClock;
    virtual void prepareSample() = 0;
    float mCurrentSample;
};

class Constant : public Source {
public:
    Constant(float val) : Source() {mCurrentSample = val;}
    virtual void prepareSample() { }
};


#endif

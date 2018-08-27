#ifndef __OSC_H__
#define __OSC_H__

#include <cmath>
#include "sink.h"

class Osc : public Source {
public:
    Osc() {}
    virtual ~Osc() {}
    virtual void prepareSample() = 0;
    Sink& CV() { return mInput; }
protected:
    Sink mDetune;
    Sink mInput;
    float getPitch() { return exp2(mInput.getSample(mCurrentClock))*27.5f; }
};

class SawOsc : public Osc {
public:
    SawOsc() : Osc(), time(0) {}
    virtual void prepareSample();
private:
    unsigned int time;
};

class SineOsc : public Osc {
public:
    SineOsc() : Osc(), time(0) {}
    virtual void prepareSample();
private:
    unsigned int time;
};

class SquareOsc : public Osc {
public:
    SquareOsc() : Osc(), time(0) {}
    virtual void prepareSample();
private:
    unsigned int time;
};

class NoiseOsc : public Osc {
public:
    NoiseOsc() : Osc() {}
    virtual void prepareSample();
};

#endif

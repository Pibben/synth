#ifndef __CONTOURGENERATOR_H__
#define __CONTOURGENERATOR_H__

#include "constants.h"
#include "source.h"
#include "sink.h"

class ContourGenerator : public Source {
public:
    ContourGenerator() : mPrevGate(false), mTrigged(false) {}
    virtual ~ContourGenerator() {}
    virtual void prepareSample() = 0;
protected:
    bool mPrevGate;
    bool mTrigged;
};

class ADSR : public ContourGenerator {
public:
    /*
    ADSR(float a, float d, float s, float r, Source* gate, Source* trigger)
        : mTarget(1.0), mAttackRate(1.0/(a*SAMPLE_RATE)), mDecayRate(-1.0/(d*SAMPLE_RATE)), mS(s),
        mReleaseRate(1.0/(-r*SAMPLE_RATE)), mGate(gate), mTrigger(trigger), mState(IDLE) {}
        */
    ADSR() : mTarget(0), mAttackRate(0), mDecayRate(0), mReleaseRate(0), time(0), mState(IDLE) {}
    virtual ~ADSR() {}
    virtual void prepareSample();
    Sink& gate() { return mGate; }
    Sink& trigger() { return mTrigger; }
    Sink& a() { return mA; }
    Sink& d() { return mD; }
    Sink& s() { return mS; }
    Sink& r() { return mR; }
private:
    Sink mA;
    Sink mD;
    Sink mS;
    Sink mR;
    Sink mGate;
    Sink mTrigger;
    float mTarget;
    float mAttackRate;
    float mDecayRate;
    float mReleaseRate;
    unsigned int time;
    enum {IDLE, ATTACK, DECAY, SUSTAIN, RELEASE } mState;
};

#endif


#include "contourgenerator.h"

#include <stdio.h>

void ADSR::prepareSample(void) {

    //State transition
    switch(mState) {
        case IDLE:
            if(mTrigger.getSample(mCurrentClock) > 0.0f) {
                mState = ATTACK;
                mTarget = 1.0;
            }
            break;
        case ATTACK:
            if(mGate.getSample(mCurrentClock) == 0.0f) {
                mState = RELEASE;
                mTarget = 0.0;
                break;
            }

            mCurrentSample += mAttackRate;
            if(mCurrentSample >= mTarget) {
                mState = DECAY;
                mTarget = mS.getSample(mCurrentClock);
            }
            break;
        case DECAY:
            if(mTrigger.getSample(mCurrentClock) > 0.0f) {
                mState = ATTACK;
                mTarget = 1.0;
                break;
            }
            if(mGate.getSample(mCurrentClock) == 0.0f) {
                mState = RELEASE;
                mTarget = 0.0;
                break;
            }
            mCurrentSample += mDecayRate;
            if(mCurrentSample <= mTarget) {
                mState = SUSTAIN;
            }
            break;
        case SUSTAIN:
            if(mTrigger.getSample(mCurrentClock) > 0.0f) {
                mState = ATTACK;
                mTarget = 1.0;
                break;
            }
            if(mGate.getSample(mCurrentClock) == 0.0f) {
                mState = RELEASE;
                mTarget = 0.0;
            } 

            break;
        case RELEASE:
            if(mTrigger.getSample(mCurrentClock) > 0.0f) {
                mState = ATTACK;
                mTarget = 1.0;
                break;
            }
            mCurrentSample += mReleaseRate;
            if(mCurrentSample <= mTarget) {
                mState = IDLE;
            }
            break;
    }
};

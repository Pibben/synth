#ifndef __VCA_H__
#define __VCA_H__

#include "source.h"

class VCA : public Source {
public:
    VCA() {}
    virtual void prepareSample() {
        mCurrentSample = mControlInput.getSample(mCurrentClock) * mSignalInput.getSample(mCurrentClock);
    }
    Sink& control() { return mControlInput; }
    Sink& signal() { return mSignalInput; }
private:
    Sink mControlInput;
    Sink mSignalInput;

};

#endif

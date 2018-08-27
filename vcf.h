#ifndef __VCF_H__
#define __VCF_H__

#include "sink.h"

class VCF : public Source {
public:
    VCF() {
        xn[0] = xn[1] = xn[2] = 0.0f;
        yn[0] = yn[1] = yn[2] = 0.0f;
    }
    virtual void prepareSample();
    Sink& q() { return mQControl; }
    Sink& f() { return mFControl; }
    Sink& signal() { return mSignalInput; }
private:
    float filter(float in);
    Sink mQControl;
    Sink mFControl;
    Sink mSignalInput;
    float xn[3];
    float yn[3];
    float mOffset;
    float mScale;
};

#endif


#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include "source.h"

class Keyboard {
public:
    Keyboard() : mCV(this), mTrigger(this), mGate(this) {}
    Source& CV()      { return mCV; }
    Source& trigger() { return mTrigger; }
    Source& gate()    { return mGate; }

    virtual float getNote() = 0;
    virtual float getTrig() = 0;
    virtual float getGate() = 0;

private:
    class Control : public Source {
    public:
        Control(Keyboard* kbd) : mKbd(kbd) {}
        void prepareSample() {
            mCurrentSample = mKbd->getNote();
        }
    private:
        Keyboard* mKbd;
    } mCV;
    class Trigger : public Source {
    public:
        Trigger(Keyboard* kbd) : mKbd(kbd) {}
        void prepareSample() {
            mCurrentSample = mKbd->getTrig();
        }
    private:
        Keyboard* mKbd;
    } mTrigger;
    class Gate : public Source {
    public:
        Gate(Keyboard* kbd) : mKbd(kbd) {}
        void prepareSample() {
            mCurrentSample = mKbd->getGate();
        }
    private:
        Keyboard* mKbd;
    } mGate;
};

#endif

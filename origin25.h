#ifndef __ORIGIN25_H__
#define __ORIGIN25_H__

#ifdef _WIN32
#include <windows.h>
#endif
#include <alsa/asoundlib.h>

#include "keyboard.h"
#include "synth.h"
#include <thread>
#include <vector>

class Origin25 : public Keyboard {
public:
    Origin25();
    virtual float getNote();
    virtual float getTrig();
    virtual float getGate();

    Source& knob(int idx) { return knobs[idx]; };
    Source& dataEntry()   { return mDataEntry; }
    Source& getModWheel() { return modWheel; }

private:
    class Knob : public Source {
        virtual void prepareSample() {
        }
    public:
        void setValue(float value) {
            mCurrentSample = value;
        }
    };
    static const int MODULATION_WHEEL = 1;
    static const int DATA_ENTRY = 7;
    static const int KNOB_1 = 74;
    static const int KNOB_2 = 71;
    static const int KNOB_3 = 81;
    static const int KNOB_4 = 91;
    static const int KNOB_5 = 16;
    static const int KNOB_6 = 80;
    static const int KNOB_7 = 19;
    static const int KNOB_8 = 2;

    void setKeyPressed(unsigned char key);
    void setKeyReleased(unsigned char key);
#ifdef _WIN32
    static void CALLBACK midiCallback(HMIDIIN handle, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
#endif
    bool mKeyspressed[128];
    bool mTrig;
    int  mNote;
    snd_seq_t *seq_handle;
    int in_port;
    std::thread thread;
    void threadFunc();
    std::vector<Knob> knobs;
    Knob mDataEntry;
    Knob modWheel;
};

#endif

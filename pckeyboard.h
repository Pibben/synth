#ifndef __PCKEYBOARD_H__
#define __PCKEYBOARD_H__

#include "keyboard.h"
#include "synth.h"

class PCKeyboard : public Keyboard {
public:
    PCKeyboard();
    virtual float getNote();
    virtual float getTrig();
    virtual float getGate();

private:
    void setKeyPressed(unsigned char key);
    void setKeyReleased(unsigned char key);

    static void thrFunc(void* param);

    bool mKeyspressed[256];
    unsigned char mKeyNoteMap[256];
    bool mTrig;
    int  mNote;
};

#endif

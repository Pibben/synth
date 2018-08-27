#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <cmath>

#include "pckeyboard.h"

PCKeyboard::PCKeyboard() {
    for(int i = 0; i < 256; ++i) {
        mKeyspressed[i] = false;
        mKeyNoteMap[i] = 0;
    }
#ifdef _WIN32
    mKeyNoteMap['Z'] = 36; //C3
    mKeyNoteMap['S'] = 37; //C#3
    mKeyNoteMap['X'] = 38; //D3
    mKeyNoteMap['D'] = 39; //D#3
    mKeyNoteMap['C'] = 40; //E3
    mKeyNoteMap['V'] = 41; //F3
    mKeyNoteMap['G'] = 42; //F#3
    mKeyNoteMap['B'] = 43; //G3
    mKeyNoteMap['H'] = 44; //G#3
    mKeyNoteMap['N'] = 45; //A4
    mKeyNoteMap['J'] = 46; //A#4
    mKeyNoteMap['M'] = 47; //B4
    mKeyNoteMap['Q'] = 48; //C4
    mKeyNoteMap[VK_OEM_COMMA] = 48; //C4
    mKeyNoteMap['L'] = 49; //C#4
    mKeyNoteMap['2'] = 49; //C#4
    mKeyNoteMap['W'] = 50; //D4
    mKeyNoteMap[VK_OEM_PERIOD] = 50; //D4
    mKeyNoteMap['3'] = 51; //D#4
    mKeyNoteMap[VK_OEM_3] = 51; //D#4
    mKeyNoteMap['E'] = 52; //E4
    mKeyNoteMap[VK_OEM_MINUS] = 52; //E4
    mKeyNoteMap['R'] = 53; //F4
    mKeyNoteMap['5'] = 54; //F#4
    mKeyNoteMap['T'] = 55; //G4
    mKeyNoteMap['6'] = 56; //G#4
    mKeyNoteMap['Y'] = 57; //A5
    mKeyNoteMap['7'] = 58; //A#5
    mKeyNoteMap['U'] = 59; //B5
    mKeyNoteMap['I'] = 60; //C5
    mKeyNoteMap['9'] = 61; //C#5
    mKeyNoteMap['O'] = 62; //D5
    mKeyNoteMap['0'] = 63; //D#5
    mKeyNoteMap['P'] = 64; //E5 
    mKeyNoteMap[VK_OEM_6] = 65; //F5
    mKeyNoteMap[VK_OEM_4] = 66; //F#5
    mKeyNoteMap[VK_OEM_1] = 67; //G5

    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)thrFunc,this,0,0);
#endif
}

float PCKeyboard::getNote() {
    for(int i = 0; i < 256; ++i) {
        if(mKeyspressed[i] && mKeyNoteMap[i] != 0) {
            mNote = mKeyNoteMap[i];
            break;
        }
    }

    return mNote ? 440.0f*pow(2.0f,(mNote-45.0f)/12.0f) : 0.0f;
}

void PCKeyboard::setKeyPressed(unsigned char key) {
    if(!mKeyspressed[key] && mKeyNoteMap[key] != 0) {
        mTrig = true;
    }
    mKeyspressed[key] = true;
}

void PCKeyboard::setKeyReleased(unsigned char key) {
    mKeyspressed[key] = false;
}

float PCKeyboard::getTrig() {
    bool retval = mTrig;
    mTrig = false;
    return retval ? 1.0f : 0.0f;
}

float PCKeyboard::getGate() {
    for(int i = 0; i < 256; ++i) {
        if(mKeyspressed[i] && mKeyNoteMap[i] != 0) {
            return 1.0;
        }
    }

    return 0.0f;
}

#ifdef _WIN32
void PCKeyboard::thrFunc(void* param) {
    PCKeyboard* kbd = reinterpret_cast<PCKeyboard*>(param);

    HANDLE hStdin;
    INPUT_RECORD irInBuf[128]; 
    DWORD cNumRead;
    hStdin = GetStdHandle(STD_INPUT_HANDLE);

    while(true) {
        ReadConsoleInput( 
            hStdin,      // input buffer handle 
            irInBuf,     // buffer to read into 
            128,         // size of read buffer 
            &cNumRead); // number of records read 

        for(unsigned int i = 0; i < cNumRead; ++i) {
            if(irInBuf[i].EventType == KEY_EVENT) {
                if(irInBuf[i].Event.KeyEvent.bKeyDown) {
                    kbd->setKeyPressed(irInBuf[i].Event.KeyEvent.wVirtualKeyCode);
                } else {
                    kbd->setKeyReleased(irInBuf[i].Event.KeyEvent.wVirtualKeyCode);
                }
            }
        }
    }
}
#endif

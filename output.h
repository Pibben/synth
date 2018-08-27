#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#ifdef _WIN32
#include <dsound.h>
#endif

#include "sink.h"

class Output : public Sink {
public:
    Output();

private:
    static const int mBufLen  = (1024*8);
#ifdef _WIN32
    LPDIRECTSOUNDBUFFER m_pBuffer;
    DWORD m_writePos;
#endif

    void play(void* stream, long len);
    void update();
#ifdef _WIN32
    static void WINAPI threadRout(void* param);
#endif
    void start();

    float mix_buf[mBufLen];
    Clock* clock;
};

#endif

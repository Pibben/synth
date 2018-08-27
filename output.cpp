
#include <assert.h>
#include <cstring>
#include <cstdio>
#ifdef _WIN32
#include <dsound.h>
#endif

#include "output.h"
#if 0
#include "alsa/asoundlib.h"
#endif

#include <pulse/simple.h>
#include <pulse/error.h>


void Output::play(void* stream, long len) {

    short* buf = (short *)stream;
    const unsigned int num_samples = len / 2;

    assert(num_samples < mBufLen);

    std::memset(mix_buf, 0, num_samples*sizeof(float));

    for(unsigned int i = 0; i < num_samples; ++i) {
        mix_buf[i] = mSource->getSample(clock->getTick());
        clock->tick();
        if(mix_buf[i] > 1.0f) mix_buf[i] = 1.0f;
        if(mix_buf[i] < -1.0f) mix_buf[i] = -1.0f;

        buf[i] = (short)(mix_buf[i] * 32767.0f);
    }
}


void Output::update() {

#ifdef _WIN32
    DWORD  playPos = 0;
    DWORD  writeLen;
    LPVOID p1, p2;
    DWORD  l1, l2;

    m_pBuffer->GetCurrentPosition(&playPos,NULL); 

    if (m_writePos < playPos)
        writeLen = playPos - m_writePos;
    else
        writeLen = mBufLen - (m_writePos - playPos);

    m_pBuffer->Lock(m_writePos, writeLen, &p1, &l1, &p2, &l2, 0);

    if ((p1) && (l1>0)) Output::play(p1, l1);
    if ((p2) && (l2>0)) Output::play(p2, l2);

    m_pBuffer->Unlock(p1, l1, p2, l2);
    m_writePos += writeLen;

    if (m_writePos >= mBufLen) m_writePos -= mBufLen;
#endif
}
#ifdef _WIN32
void WINAPI Output::threadRout(void* param) {
    Output* output = reinterpret_cast<Output*>(param);
    while (1) {
        output->update();
        Sleep(25);
    }
}
#endif

void Output::start() {
    /* The Sample format to use */
    static const pa_sample_spec ss = { PA_SAMPLE_S16LE, 44100, 1 };
    static const int BUFSIZE = 128;

    pa_buffer_attr ba;
    ba.tlength = pa_usec_to_bytes(50*1000, &ss);
    ba.minreq = pa_usec_to_bytes(0, &ss);
    ba.maxlength = pa_usec_to_bytes(50*1000, &ss);

    pa_simple *s = NULL;
    int error;

    /* Create a new playback stream */
    if (!(s = pa_simple_new(NULL, "synth", PA_STREAM_PLAYBACK, NULL, "playback",
            &ss, NULL, &ba, &error))) {
        printf( __FILE__": pa_simple_new() failed: %s\n",
                pa_strerror(error));
        goto finish;
    }
    for (;;) {
        uint8_t buf[BUFSIZE];
#if 0
        pa_usec_t latency;
        if ((latency = pa_simple_get_latency(s, &error)) == (pa_usec_t) -1) {
            fprintf(stderr, __FILE__": pa_simple_get_latency() failed: %s\n", pa_strerror(error));
            goto finish;
        }
        fprintf(stderr, "%0.0f usec \r", (float)latency);
#endif

        play(buf, BUFSIZE*2);

        /* ... and play it */
        if (pa_simple_write(s, buf, BUFSIZE*2, &error) < 0) {
            printf( __FILE__": pa_simple_write() failed: %s\n",
                    pa_strerror(error));
            goto finish;
        }
    }
    /* Make sure that every single sample was played */
    if (pa_simple_drain(s, &error) < 0) {
        printf( __FILE__": pa_simple_drain() failed: %s\n",
                pa_strerror(error));
        goto finish;
    }

finish:
    if (s)
        pa_simple_free(s);

#ifdef _WIN32
    DSBUFFERDESC mBufDesc;
    WAVEFORMATEX format;

    LPDIRECTSOUND		m_pDS;
    LPDIRECTSOUNDBUFFER	m_pPrimary;

    m_writePos = 0;
    ::DirectSoundCreate(0, &m_pDS, 0);

    memset(&mBufDesc, 0, sizeof(mBufDesc));
    mBufDesc.dwSize = sizeof(DSBUFFERDESC);
    mBufDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    
    memset(&format, 0, sizeof(format));
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 1;
    format.nSamplesPerSec = 44100;
    format.nAvgBytesPerSec = format.nSamplesPerSec * sizeof(unsigned short);
    format.nBlockAlign = sizeof(unsigned short);
    format.wBitsPerSample = 8*sizeof(unsigned short);

    m_pDS->SetCooperativeLevel(GetConsoleWindow(), DSSCL_EXCLUSIVE);
    m_pDS->CreateSoundBuffer(&mBufDesc,&m_pPrimary, NULL);
    m_pPrimary->SetFormat(&format);

    mBufDesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2;
    mBufDesc.dwBufferBytes = mBufLen;
    mBufDesc.lpwfxFormat = &format;
    m_pDS->CreateSoundBuffer(&mBufDesc,&m_pBuffer,NULL);

    m_pBuffer->Play(0, 0, DSBPLAY_LOOPING);

    SetThreadPriority(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)threadRout,this,0,0),THREAD_PRIORITY_TIME_CRITICAL);
#endif
}

Output::Output() {
    this->clock = new Clock();

    start();
}


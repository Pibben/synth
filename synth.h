#ifndef __SYNTH_H__
#define __SYNTH_H__

#include "source.h"

class Synth {
public:
    Synth();
    Source* mInstrument;
};

void play(void *stream, long len);
void dispatch(float pitch);
void dispatch(unsigned char note);

#endif
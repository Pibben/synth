
#include <cmath>
#include <stdlib.h> //rand()
#include <cstdio>

#include "constants.h"
#include "osc.h"

static const float global_amp = 0.6f;

void SawOsc::prepareSample() {
    const float realtime = time*invSAMPLE_RATE;
    const float pitch = getPitch();
    mCurrentSample = fmodf(realtime*pitch, 2.0f) - 1.0f;
    mCurrentSample *= global_amp;
    time++;
}

//Hz = exp2(cv)*27.5
void SquareOsc::prepareSample() {
    const float realtime = time*invSAMPLE_RATE;
    const float pitch = getPitch();
    printf("Pitch: %f\n", pitch);
    const float periodo_square = 1.0f/pitch;
    if (fmodf(realtime, periodo_square) > (periodo_square/2.0f))
        mCurrentSample = 1.0f;
    else
        mCurrentSample = -1.0f;
    mCurrentSample *= global_amp;
    time++;
}


void SineOsc::prepareSample() {
    const float realtime = time*invSAMPLE_RATE;
    const float pitch = getPitch();
    mCurrentSample = sin(TWOPI*pitch*realtime);
    mCurrentSample *= global_amp;
    time++;
}


void NoiseOsc::prepareSample() {
    mCurrentSample = rand()/(float)RAND_MAX;
    mCurrentSample *= global_amp;
}

#include <cmath>

#include "constants.h"
#include "vcf.h"

void VCF::prepareSample() {
    mCurrentSample = filter(mSignalInput.getSample(mCurrentClock));
}

float VCF::filter(float in) {
    float c,c2,a1,a2,b1,b2;

    const float cutoff = (mFControl.getSample(mCurrentClock)+mOffset)*mScale;
    const float resonance = mQControl.getSample(mCurrentClock);
    const float param_tan = PI_samplerate * cutoff;

    if (cutoff > 0.0f) {// Low-pass
        c=1.0f / tan(param_tan);
        c2 = c*c;

        a1 = 1.0f / ( 1.0f + resonance * c + c2);
        a2 = 2.0f * a1;
        b1 = 2.0f * ( 1.0f - c2) * a1;
    } else {// High-pass
        c = tan(-param_tan);
        c2 = c*c;

        a1 = 1.0f / ( 1.0f + resonance * c + c2);
        a2 = -2*a1;
        b1 = 2.0f * ( c2 - 1.0f) * a1;
    }

    b2 = ( 1.0f - resonance * c + c2) * a1;

    yn[0] = a1*in + a2*xn[1] + a1*xn[2] - b1*yn[1] - b2*yn[2];

    xn[2] = xn[1];
    xn[1] = in;
    yn[2] = yn[1];
    yn[1] = yn[0];

    return yn[0];
}

#include <assert.h>

#include "constants.h"
#include "contourgenerator.h"
#include "origin25.h"
#include "pckeyboard.h"
#include "osc.h"
#include "output.h"
#include "synth.h"
#include "vca.h"
#include "vcf.h"


Synth::Synth() {
    Origin25 kbd;
    ADSR adsr;
    kbd.gate() >> adsr.gate();
    kbd.trigger() >> adsr.trigger();
    0.1f >> adsr.a();
    0.1f >> adsr.d();
    0.4f >> adsr.s();
    0.1f >> adsr.r();

    SquareOsc osc;
    kbd.CV() >> osc.CV();

    VCA vca;
    adsr >> vca.control();
    osc >> vca.signal();

    VCF vcf;
    kbd.knob(0) >> vcf.f();
    kbd.knob(1) >> vcf.q();
    vca >> vcf.signal();

    Output o;
    vcf >> o;

#if 0
    Origin25* kbd = new Origin25();
    //PCKeyboard* kbd = new PCKeyboard();
    ADSR* sdsr = new ADSR(0.1f, 0.1f, 0.4f, 0.1f, &kbd->gate(), &kbd->trigger());
    Osc* osc = new SquareOsc(0, &kbd->CV());
    VCA* vca = new VCA(sdsr, osc);
    SineOsc* lfo = new SineOsc(0, new Constant(0.25f));
    VCF* vcf = new VCF(&kbd->knob(1), &kbd->knob(0), vca, 1.3f, 1700.0f);
    //VCF* vcf = new VCF(lfo, &kbd->knob(0), vca, 1.3f, 1700.0f);

    Output* o = new Output(vcf);
#endif
}




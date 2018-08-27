#include <assert.h>
#include <cmath>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <alsa/asoundlib.h>
//#include <alsa/seq.h>

#include "origin25.h"

void Origin25::threadFunc() {
    while(true) {
        snd_seq_event_t *ev = NULL;
        snd_seq_event_input(seq_handle, &ev);

        switch (ev->type) {
          case SND_SEQ_EVENT_CONTROLLER:
//            fprintf(stderr, "Control event on Param %2d: %5d       \r",
//                    ev->data.control.param, ev->data.control.value);
              switch(ev->data.control.param) {
              case KNOB_1:
                  knobs[0].setValue(ev->data.control.value / 127.0f);
                  break;
              case KNOB_2:
                  knobs[1].setValue(ev->data.control.value / 127.0f);
                  break;
              case KNOB_3:
                  knobs[2].setValue(ev->data.control.value / 127.0f);
                  break;
              case KNOB_4:
                  knobs[3].setValue(ev->data.control.value / 127.0f);
                  break;
              case KNOB_5:
                  knobs[4].setValue(ev->data.control.value / 127.0f);
                  break;
              case KNOB_6:
                  knobs[5].setValue(ev->data.control.value / 127.0f);
                  break;
              case KNOB_7:
                  knobs[6].setValue(ev->data.control.value / 127.0f);
                  break;
              case KNOB_8:
                  knobs[7].setValue(ev->data.control.value / 127.0f);
                  break;
              case MODULATION_WHEEL:
                  modWheel.setValue(ev->data.control.value / 127.0f);
                  break;
              case DATA_ENTRY:
                  mDataEntry.setValue(ev->data.control.value / 127.0f);
                  break;
              }
            break;
          case SND_SEQ_EVENT_PITCHBEND:
//            fprintf(stderr, "Pitchbender event on Channel %2d: %5d   \r",
//                    ev->data.control.channel, ev->data.control.value);
            break;
          case SND_SEQ_EVENT_NOTEON:
            setKeyPressed(ev->data.note.note);
            break;
          case SND_SEQ_EVENT_NOTEOFF:
            setKeyReleased(ev->data.note.note);
            break;
        }
    }
}

Origin25::Origin25() : knobs(8) {
    for(int i = 0; i < 128; ++i) {
        mKeyspressed[i] = false;
    }

#ifdef _WIN32
    unsigned long result;
    HMIDIIN      inHandle;

    /* Open the default MIDI Out device */
    result = midiInOpen(&inHandle, 1, (DWORD)midiCallback, (DWORD_PTR)this, CALLBACK_FUNCTION);
    if (result)
    {
       printf("There was an error opening the default MIDI Out device!\r\n");
       exit(-1);
    }

    midiInStart(inHandle);
#endif

    if (snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0) < 0) {
      fprintf(stderr, "Error opening ALSA sequencer.\n");
      exit(1);
    }
    snd_seq_set_client_name(seq_handle, "Midi Listener");
    if ((in_port = snd_seq_create_simple_port(seq_handle, "ALSA Sequencer Demo",
              SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
              SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
      fprintf(stderr, "Error creating sequencer port.\n");
      exit(1);
    }

    snd_seq_port_subscribe_t* subs;
    snd_seq_port_subscribe_malloc(&subs);
    snd_seq_addr_t sender = {28,0};
    snd_seq_port_subscribe_set_sender(subs, &sender);
    snd_seq_addr_t dest = {128,0};
    snd_seq_port_subscribe_set_dest(subs, &dest);
    snd_seq_port_subscribe_set_queue(subs, 1);
    snd_seq_port_subscribe_set_time_update(subs, 1);
    snd_seq_port_subscribe_set_time_real(subs, 1);
    snd_seq_subscribe_port(seq_handle, subs);

    thread = std::thread(&Origin25::threadFunc, this);
}

float Origin25::getNote() {

    for(int i = 127; i >= 0; --i) {
        if(mKeyspressed[i]) {
            mNote = (float)i;
            break;
        }
    }

    printf("CV: %f\n", (mNote-21)/12.0);
    return (mNote-21)/12.0;
}

void Origin25::setKeyPressed(unsigned char key) {
    //printf("Press %d\n", key);
    if(!mKeyspressed[key]) {
        mTrig = true;
    }
    mKeyspressed[key] = true;
}

void Origin25::setKeyReleased(unsigned char key) {
    //printf("Release %d\n", key);
    mKeyspressed[key] = false;
}

float Origin25::getTrig() {
    bool retval = mTrig;
    mTrig = false;
    return retval ? 1.0f : 0.0f;
}

float Origin25::getGate() {
    for(int i = 0; i < 128; ++i) {
        if(mKeyspressed[i]) {
            return 1.0f;
        }
    }

    return 0.0f;
}
#ifdef _WIN32
void Origin25::midiCallback(HMIDIIN handle, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2) {
    Origin25* kbd = reinterpret_cast<Origin25*>(dwInstance);
    assert(kbd);

    if(uMsg == MIM_DATA) {
        unsigned char status =  dwParam1 & 0xff;
        unsigned char lowByte = (dwParam1 >> 8) & 0xff;
        unsigned char highByte = (dwParam1 >> 16) & 0xff;

        //unsigned char channel = status & 0x0f;
        unsigned char action = (status >> 4) & 0x0f;

        switch(action) {
        case 8:
            kbd->setKeyReleased(lowByte);
            break;
        case 9:
            kbd->setKeyPressed(lowByte);
            break;
        case 11:
            printf("Control change: knob %d, value %d\n", lowByte, highByte);
            break;
        case 14:
            unsigned short pitch = (highByte << 1) | (lowByte >> 6);
            printf("Pitch wheel: %d\n", pitch);
            break;
        }

        //printf("Channel: %d, action: %d, low: %d. high: %d\n", channel, action, lowByte, highByte);
    }
}
#endif

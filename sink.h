/*
 * sink.h
 *
 *  Created on: Mar 6, 2014
 *      Author: per
 */

#ifndef SINK_H_
#define SINK_H_

#include "source.h"

class Sink {
public:
    Sink() : mSource(nullptr) {}
    float getSample(unsigned int clock) { return mSource ? mSource->getSample(clock) : 0.0f; }
    void setSource(Source* source) { mSource = source; }
protected:
    Source* mSource;
};

inline void operator>> (Source& source, Sink& sink) { sink.setSource(&source); }
inline void operator>> (float c, Sink& sink) { sink.setSource(new Constant(c)); }

#endif /* SINK_H_ */

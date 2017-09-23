/*
  author: Sam Coward
  date: Sept 2017
*/

#ifndef PROGRESSEVENT_H
#define PROGRESSEVENT_H

#include "event.hpp"

class ProgressEvent : public Event
{
  public:
    ProgressEvent(double time, int idx);
    const int idx;

    virtual void process(EventManager &EM, SNN &snn);
};

#endif

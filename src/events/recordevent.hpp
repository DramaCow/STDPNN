#ifndef RECORDEVENT_H
#define RECORDEVENT_H

#include "event.hpp"

class RecordEvent : public Event
{
  public:
    RecordEvent(double time, int idx);
    const int idx;

    virtual void process(EventManager &EM, SNN &snn);
};

#endif

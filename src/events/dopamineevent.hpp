#ifndef DOPAMINEEVENT_H
#define DOPAMINEEVENT_H

#include "event.hpp"

class DopamineEvent : public Event
{
  public:
    DopamineEvent(double time, int group_id, double d1, double d2) : Event(time), group_id(group_id), d1(d1), d2(d2) {}
    int group_id;
    double d1, d2;
    virtual void process(EventManager &EM, SNN &snn);
};

#endif

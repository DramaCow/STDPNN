/*
  author: Sam Coward
  date: Sept 2017
*/

#ifndef EPOCHEVENT_H
#define EPOCHEVENT_H

#include "event.hpp"

class EpochEvent : public Event
{
  public:
    EpochEvent(double time, int group_id);
    const int group_id;

    virtual void process(EventManager &EM, SNN &snn);
};

#endif

#ifndef WRITEEVENT_H
#define WRITEEVENT_H

#include "event.hpp"

class WriteEvent : public Event
{
  public:
    WriteEvent(double time, std::string figure) : Event(time), figure(figure) {}
    std::string figure;
    virtual void process(EventManager &EM, SNN &snn);
};

#endif

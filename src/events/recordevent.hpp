/*
  author: Sam Coward
  date: Sept 2017
*/

#ifndef RECORDEVENT_H
#define RECORDEVENT_H

#include "event.hpp"
#include <string>

class RecordEvent : public Event
{
  public:
    RecordEvent(double time, int idx, std::string fig);
    const int idx;

    virtual void process(EventManager &EM, SNN &snn);

  private:
    std::string fig;
};

#endif

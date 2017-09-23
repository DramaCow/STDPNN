/*
  author: Sam Coward
  date: Sept 2017
*/

#ifndef ACTIONTRIALEVENT_H
#define ACTIONTRIALEVENT_H

#include "event.hpp"

class ActionTrialResetEvent : public Event
{
  public:
    ActionTrialResetEvent(double time);
    virtual void process(EventManager &EM, SNN &snn);
};

class RepeatedActionTrialEvent : public Event
{
  public:
    RepeatedActionTrialEvent(double time, int last);
    RepeatedActionTrialEvent(double time, int it, int last);
    const int it;
    const int last;

    virtual void process(EventManager &EM, SNN &snn);
};

class RandomActionTrialEvent : public Event
{
  public:
    RandomActionTrialEvent(double time, int last);
    RandomActionTrialEvent(double time, int it, int last);
    const int it;
    const int last;

    virtual void process(EventManager &EM, SNN &snn);
};

#endif

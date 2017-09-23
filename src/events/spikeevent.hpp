/*
  author: Sam Coward
  date: Sept 2017
*/

#ifndef SPIKEEVENT_H
#define SPIKEEVENT_H

#include "event.hpp"

class SpikeEvent : public Event
{
  public:
    SpikeEvent(double time, Neuron *neuron);
    Neuron *const neuron;

    virtual void process(EventManager &EM, SNN &snn);
};

#endif

#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <random>
#include <string>
#include "../allneurons.hpp"
#include "../snn.hpp"

class EventManager; // forward declaration

class Event
{
  public:
    Event(double time);
    virtual ~Event() {} // necessary for polymorphism
    const double time;

    virtual void process(EventManager &EM, SNN &snn) = 0;
};

#endif

#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <random>
#include <string>
#include "../neurons/neuron.hpp"
#include "../SNN.hpp"

class EventManager; // forward declaration

class Event
{
  public:
    Event(double time);
    virtual ~Event() {} // necessary for polymorphism
    const double time;

    virtual void process(EventManager &EM, SNN &snn) = 0;
};

class SpikeEvent : public Event
{
  public:
    SpikeEvent(double time, Neuron *neuron);
    Neuron *const neuron;

    virtual void process(EventManager &EM, SNN &snn);
};

class EpochEvent : public Event
{
  public:
    EpochEvent(double time, int group_id);
    const int group_id;

    virtual void process(EventManager &EM, SNN &snn);
};

class RecordEvent : public Event
{
  public:
    RecordEvent(double time, int idx);
    const int idx;

    virtual void process(EventManager &EM, SNN &snn);
};

class DopamineEvent : public Event
{
  public:
    DopamineEvent(double time, int group_id, double d1, double d2) : Event(time), group_id(group_id), d1(d1), d2(d2) {}
    int group_id;
    double d1, d2;
    virtual void process(EventManager &EM, SNN &snn);
};

class WriteEvent : public Event
{
  public:
    WriteEvent(double time, std::string figure) : Event(time), figure(figure) {}
    std::string figure;
    virtual void process(EventManager &EM, SNN &snn);
};

// ===

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

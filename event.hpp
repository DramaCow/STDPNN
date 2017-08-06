#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include "neuron.hpp"
#include "SNN.hpp"

class EventQueue; // forward declaration

class Event
{
  public:
    Event(double time, int type);
    virtual ~Event() {} // necessary for polymorphism
    const double time;

    virtual void process(EventQueue &EQ, SNN &snn) = 0;

    const int type;
};

class EventQueue
{
  public:
    EventQueue();
    ~EventQueue();

    void insert(Event *event);
    Event * get_min();
    void del_min();

    int size();

    const double duration;
    double t_sim;

  private:
    std::vector<Event*> event_list;
    int current_size;

    void perc_up(int idx);
    void perc_down(int idx);
    int min_child(int idx);
};

class SpikeEvent : public Event
{
  public:
    SpikeEvent(double time, int type, Neuron *neuron);
    Neuron *const neuron;

    virtual void process(EventQueue &EQ, SNN &snn);
};

class EpochEvent : public Event
{
  public:
    EpochEvent(double time, int type, int group_id);
    const int group_id;

    virtual void process(EventQueue &EQ, SNN &snn) {}
};

class RecordEvent : public Event
{
  public:
    RecordEvent(double time, int type, int idx);
    const int idx;

    virtual void process(EventQueue &EQ, SNN &snn) {}
};

#endif

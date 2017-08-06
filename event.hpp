#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <random>
#include "neuron.hpp"
#include "SNN.hpp"

class EventManager; // forward declaration

class Event
{
  public:
    Event(double time);
    virtual ~Event() {} // necessary for polymorphism
    const double time;

    virtual void process(EventManager &EM, SNN &snn) = 0;
};

class EventManager
{
  public:
    EventManager();
    ~EventManager();

    void insert(Event *event);
    Event * get_min();
    void del_min();
    int size();

    const double duration;
    double t_sim;

    std::mt19937 gen; // random number generator

    const double rec_period;
    const int rec_entries;
    std::vector<double> w1_record;
    std::vector<double> w2_record;
    std::vector<double> t_record;

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

#endif

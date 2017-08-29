#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <random>
#include <string>
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
    EventManager(double duration);
    ~EventManager();

    void insert(Event *event);
    Event * get_min();
    void del_min();
    int size();

    const double duration;

    const double epoch_freq;
    std::array<double,2> t_epoch;

    const double rec_period;

    double rec_entries;
    //std::vector<double> w1_record, w2_record, t_record;
    std::vector<double> w1_record;
    std::vector<double> w2_record;
    std::vector<double> t_record;

    std::mt19937 gen; // random number generator

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

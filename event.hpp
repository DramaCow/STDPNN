#ifndef __EVENT_H
#define __EVENT_H

#include <vector>

class Event
{
  public:
    Event(double time);
    virtual ~Event() {} // necessary for polymorphism
    const double time;

    virtual void process() = 0;
};

class SpikeEvent : public Event
{
  public:
    SpikeEvent(double time, int n_id);
    const int n_id;

    virtual void process() {}
};

class EpochEvent : public Event
{
  public:
    EpochEvent(double time, int g_id);
    const int g_id;

    virtual void process() {}
};

class RecordEvent : public Event
{
  public:
    RecordEvent(double time);

    virtual void process() {}
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

    std::vector<Event*> event_list; // TODO: remove!
  private:
    //std::vector<Event*> event_list;
    int current_size;

    void perc_up(int idx);
    void perc_down(int idx);
    int min_child(int idx);
};

#endif

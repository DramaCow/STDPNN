#ifndef __EVENT_H
#define __EVENT_H

#include <vector>

class Event
{
  public:
    Event(double time);

    double get_time();

  private:
    double time;
};

class EventQueue
{
  public:
    EventQueue();

    Event * insert(Event *event);
    Event * get_min();
    Event * del_min();

  private:
    std::vector<Event*> event_list;
    int current_size;

    void perc_up(int idx);
    void perc_down(int idx);
    int min_child(int idx);
};

#endif

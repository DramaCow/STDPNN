#ifndef __EVENT_H
#define __EVENT_H

#include <vector>

class Event
{
  public:
    Event(double time);
    const double time;
};

class EventQueue
{
  public:
    EventQueue();
    ~EventQueue();

    void insert(Event *event);
    Event * get_min();
    void del_min();

    std::vector<Event*> event_list; // TODO: remove!
  private:
    //std::vector<Event*> event_list;
    int current_size;

    void perc_up(int idx);
    void perc_down(int idx);
    int min_child(int idx);
};

#endif

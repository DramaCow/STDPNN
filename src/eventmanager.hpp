/*
  author: Sam Coward
  date: Sept 2017
*/

#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "events/event.hpp"

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

#endif

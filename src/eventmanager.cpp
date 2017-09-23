/*
  author: Sam Coward
  date: Sept 2017
*/

#include "eventmanager.hpp"

EventManager::EventManager(double duration) : 
  duration(duration),
  epoch_freq(50.0), t_epoch{0.0, 0.0},
  rec_period(1.0),

  rec_entries(int(duration/rec_period)+1),
  w1_record(rec_entries, 0),
  w2_record(rec_entries, 0),
  t_record(rec_entries, 0)
{
  event_list = {nullptr};
  current_size = 0;

  std::random_device rd; // slow rng for one-off seed (uses  device entropy)
  gen.seed(rd()); // standard mersenne_twister_engine

  for (int i = 0; i < rec_entries; ++i)
  {
    t_record[i] = rec_period*i;
  }
}

EventManager::~EventManager()
{
  for (auto &event : event_list)
  {
    delete event;
  }
}

int EventManager::size()
{
  return current_size;
}

void EventManager::insert(Event *event)
{
  event_list.push_back(event);
  current_size = current_size + 1;
  perc_up(current_size);
}

Event * EventManager::get_min()
{
  return event_list[1];
}

void EventManager::del_min()
{
  Event *delval = event_list[1];
  event_list[1] = event_list[current_size];

  current_size = current_size - 1;
  event_list.pop_back();

  perc_down(1);

  delete delval;
}

void EventManager::perc_up(int idx)
{
  while (idx/2 > 0)
  {
    if (event_list[idx]->time < event_list[idx/2]->time)
    {
      Event *tmp = event_list[idx/2];
      event_list[idx/2] = event_list[idx];
      event_list[idx] = tmp;
    }
    idx = idx/2;
  }
}

void EventManager::perc_down(int idx)
{
  while (2*idx <= current_size)
  {
    int mc = min_child(idx);
    if (event_list[idx]->time > event_list[mc]->time)
    {
      Event *tmp = event_list[idx];
      event_list[idx] = event_list[mc];
      event_list[mc] = tmp;
    }
    idx = mc;
  }
}

int EventManager::min_child(int idx)
{
  if (2*idx + 1 > current_size)
  {
    return 2*idx;
  }

  if (event_list[2*idx]->time < event_list[2*idx + 1]->time)
  {
    return 2*idx;
  }
  else
  {
    return 2*idx + 1;
  }
}

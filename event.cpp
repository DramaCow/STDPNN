#include "event.hpp"

Event::Event(double time) : time(time)
{
}

double Event::get_time()
{
  return time;
}

EventQueue::EventQueue()
{
  event_list = {nullptr};
  current_size = 0;
}

/*
  void insert(Event *event);
  Event * getMin();
  Event * delMin();
*/

Event * EventQueue::insert(Event *event)
{
  event_list.push_back(event);
  current_size = current_size + 1;
  perc_up(current_size);
  return event;
}

Event * EventQueue::get_min()
{
  return event_list[1];
}

Event * EventQueue::del_min()
{
  Event *retval = event_list[1];
  event_list[1] = event_list[current_size];

  current_size = current_size - 1;
  event_list.pop_back();

  perc_down(1);

  return retval;
}

void EventQueue::perc_up(int idx)
{
  while (idx/2 > 0)
  {
    if (event_list[idx]->get_time() < event_list[idx/2]->get_time())
    {
      Event *tmp = event_list[idx/2];
      event_list[idx/2] = event_list[idx];
      event_list[idx] = tmp;
    }
    idx = idx/2;
  }
}

void EventQueue::perc_down(int idx)
{
  while (2*idx <= current_size)
  {
    int mc = min_child(idx);
    if (event_list[idx]->get_time() > event_list[mc]->get_time())
    {
      Event *tmp = event_list[idx];
      event_list[idx] = event_list[mc];
      event_list[mc] = tmp;
    }
    idx = mc;
  }
}

int EventQueue::min_child(int idx)
{
  if (2*idx + 1 > current_size)
  {
    return 2*idx;
  }

  if (event_list[2*idx]->get_time() < event_list[2*idx + 1]->get_time())
  {
    return 2*idx;
  }
  else
  {
    return 2*idx + 1;
  }
}

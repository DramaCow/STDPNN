#include "event.hpp"

Event::Event(double time, int type) : time(time), type(type)
{
}

EventManager::EventManager() : duration(1.0), t_sim(0.0)
{
  event_list = {nullptr};
  current_size = 0;
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

SpikeEvent::SpikeEvent(double time, int type, Neuron *neuron) : Event(time, type), neuron(neuron)
{
}

void SpikeEvent::process(EventManager &EQ, SNN &snn)
{
  neuron->update(EQ.t_sim);

  if (neuron->is_spiking())
  {
    for (Synapse *&sy : snn.con.out(neuron))
    {
      sy->post->update(EQ.t_sim);
      sy->pre_spike();
      sy->post->receive_spike(sy->post->type, sy->get_w());

      double t_next = sy->post->next_spike_time(EQ.t_sim);
      if (t_next <= EQ.duration)
      {
        EQ.insert(new SpikeEvent(t_next, 0, sy->post));
      }
    }

    for (Synapse *&sy : snn.con.in(neuron))
    {
      sy->pre->update(EQ.t_sim);
      sy->post_spike();
    }

    neuron->spike();
  }
          
  double t_next = neuron->next_spike_time(EQ.t_sim);
  if (t_next <= EQ.duration)
  {
    EQ.insert(new SpikeEvent(t_next, 0, neuron));
  }
}

EpochEvent::EpochEvent(double time, int type, int group_id) : Event(time, type), group_id(group_id)
{
}

RecordEvent::RecordEvent(double time, int type, int idx) : Event(time, type), idx(idx)
{
}

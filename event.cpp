#include "event.hpp"

#define FRF 0.3
double corr_fr(double x, double y)
{
  double fr = 10.0*(1.0 + FRF*x + FRF*y);
  return fr < 0.0 ? 0.0 : fr;
}

double uncorr_fr(double x)
{
  double fr = 10.0*(1.0 + FRF*sqrt(2.0)*x);
  return fr < 0.0 ? 0.0 : fr;
}

Event::Event(double time) : time(time)
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

SpikeEvent::SpikeEvent(double time, Neuron *neuron) : Event(time), neuron(neuron)
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
      sy->post->receive_spike(sy);

      double t_next = sy->post->next_spike_time(EQ.t_sim);
      if (t_next <= EQ.duration)
      {
        EQ.insert(new SpikeEvent(t_next, sy->post));
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
    EQ.insert(new SpikeEvent(t_next, neuron));
  }
}

EpochEvent::EpochEvent(double time, int group_id) : Event(time), group_id(group_id)
{
}

RecordEvent::RecordEvent(double time, int idx) : Event(time), idx(idx)
{
}

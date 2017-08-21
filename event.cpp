#include "event.hpp"
#include <algorithm>
#include <iostream>
#include <iomanip>

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

SpikeEvent::SpikeEvent(double time, Neuron *neuron) : Event(time), neuron(neuron)
{
}

void SpikeEvent::process(EventManager &EM, SNN &snn)
{
  neuron->update(time);

  if (neuron->is_spiking())
  {
    // acting as PRE-synaptic neuron
    for (Synapse *&sy : snn.con.out(neuron))
    {
      sy->post->update(time);
      sy->pre_spike();
      sy->post->receive_spike(sy);

      double t_next = sy->post->next_spike_time(time);
      if (t_next <= sy->post->t_limit)
      {
        EM.insert(new SpikeEvent(t_next, sy->post));
      }
    }

    // acting as POST-synaptic neuron
    for (Synapse *&sy : snn.con.in(neuron))
    {
      sy->pre->update(time);
      sy->post_spike();
    }

    neuron->spike();

    neuron->spikes.push_back(time);
  }
          
  double t_next = neuron->next_spike_time(time);
  if (t_next <= neuron->t_limit)
  {
    EM.insert(new SpikeEvent(t_next, neuron));
  }
}

EpochEvent::EpochEvent(double time, int group_id) : Event(time), group_id(group_id)
{
}

void EpochEvent::process(EventManager &EM, SNN &snn)
{
  double t_period = std::exponential_distribution<double>{EM.epoch_freq}(EM.gen);
  double t_delay = t_period < EM.duration-time ? t_period : EM.duration-time;

  if (t_delay > 0.0)
  {
    EM.t_epoch[group_id] += t_delay;
    EM.insert(new EpochEvent(EM.t_epoch[group_id], group_id));

    auto begin = group_id == 0 ? std::begin(snn.ppn)       : std::begin(snn.ppn) + 500;
    auto end   = group_id == 0 ? std::begin(snn.ppn) + 500 : std::end(snn.ppn);

    double norm_var_y = std::normal_distribution<double>{0.0, 1.0}(EM.gen);
    for (auto it = begin; it < end; ++it)
    {
      (*it)->t_limit = EM.t_epoch[group_id];

      double norm_var_x = std::normal_distribution<double>{0.0, 1.0}(EM.gen);
      (*it)->fr = corr_fr(norm_var_x, norm_var_y);
  
      double t_next = (*it)->next_spike_time(time);
      if (t_next <= (*it)->t_limit)
      {
        EM.insert(new SpikeEvent(t_next, *it));
      } 
    }
  }
}

RecordEvent::RecordEvent(double time, int idx) : Event(time), idx(idx)
{
}

void RecordEvent::process(EventManager &EM, SNN &snn)
{
  int progress = 32*(time/EM.duration);
  std::cout << "\r progress [" << std::string(progress, '#') 
            << std::string(32-progress, ' ') 
            << "] " 
            //<< std::setprecision(2) 
            //<< std::fixed 
            << time 
            << "s " 
            << std::flush;

  /* record data here */
  auto begin  = std::begin(snn.ppn);
  auto middle = std::begin(snn.ppn) + 500;
  auto end    = std::end(snn.ppn);
  double sum_w;

  sum_w = 0.0;
  for (auto it = begin; it < middle; ++it)
  {
    for (Synapse *&sy : snn.con.out(*it))
    {
      sum_w += sy->get_w();
    }
  }
  EM.w1_record[idx] = (sum_w / (snn.ppn.size()/2.0)) / W_MAX;

  sum_w = 0.0;
  for (auto it = middle; it < end; ++it)
  {
    for (Synapse *&sy : snn.con.out(*it))
    {
      sum_w += sy->get_w();
    }
  }
  EM.w2_record[idx] = (sum_w / (snn.ppn.size()/2.0)) / W_MAX;

  double t_delay = EM.rec_period < (EM.duration-time) ? EM.rec_period : (EM.duration-time);
  if (t_delay > 0)
  {
    EM.insert(new RecordEvent(time + t_delay, idx+1));
  }
}

void DopamineEvent::process(EventManager &EM, SNN &snn)
{

  auto begin = group_id == 0 ? std::begin(snn.ppn)       : std::begin(snn.ppn) + 500;
  auto end   = group_id == 0 ? std::begin(snn.ppn) + 500 : std::end(snn.ppn);

  for (auto it = begin; it < end; ++it)
  {
    (*it)->d1 = d1;
    (*it)->d2 = d2;
  }
  for (Neuron *&neuron : snn.sn)
  {
    neuron->d1 = d1;
    neuron->d2 = d2;
  }
}

#define f_sal 25.0 // salient fire rate
#define f_bac 3.0  // background fire rate

ActionTrialResetEvent::ActionTrialResetEvent(double time) : Event(time)
{
}

void ActionTrialResetEvent::process(EventManager &EM, SNN &snn)
{
  for (auto n = std::begin(snn.ppn); n < std::begin(snn.ppn) + 5; ++n)
  {
    (*n)->t_limit = time + 2.0;
    (*n)->fr = f_bac;

    double t_next = (*n)->next_spike_time(time);
    if (t_next <= (*n)->t_limit)
    {
      EM.insert(new SpikeEvent(t_next, *n));
    }
  }
}

RepeatedActionTrialEvent::RepeatedActionTrialEvent(double time, int last) : Event(time), it(0), last(last)
{
}

RepeatedActionTrialEvent::RepeatedActionTrialEvent(double time, int it, int last) : Event(time), it(it), last(last)
{
}

void RepeatedActionTrialEvent::process(EventManager &EM, SNN &snn)
{
  if (it == 0)
  {
    std::sort(std::begin(snn.ppn), std::end(snn.ppn), [](Neuron *n1, Neuron *n2) { return n1->id < n2->id; });
  }

  for (auto n = std::begin(snn.ppn); n < std::begin(snn.ppn) + 5; ++n)
  {
    (*n)->t_limit = time + 0.4;
    (*n)->fr = f_sal;

    EM.insert(new ActionTrialResetEvent(time + 0.4));

    double t_next = (*n)->next_spike_time(time);
    if (t_next <= (*n)->t_limit)
    {
      EM.insert(new SpikeEvent(t_next, *n));
    }
  }
  for (auto n = std::begin(snn.ppn) + 5; n < std::end(snn.ppn); ++n)
  {
    (*n)->t_limit = time + 2.4;
    (*n)->fr = f_bac;

    double t_next = (*n)->next_spike_time(time);
    if (t_next <= (*n)->t_limit)
    {
      EM.insert(new SpikeEvent(t_next, *n));
    }
  }

  if (it < last-1)
  {
    EM.insert(new RepeatedActionTrialEvent(time+2.4, it+1, last));
  }
}

RandomActionTrialEvent::RandomActionTrialEvent(double time, int last) : Event(time), it(0), last(last)
{
}

RandomActionTrialEvent::RandomActionTrialEvent(double time, int it, int last) : Event(time), it(it), last(last)
{
}

void RandomActionTrialEvent::process(EventManager &EM, SNN &snn)
{
  std::shuffle(std::begin(snn.ppn), std::end(snn.ppn), EM.gen);

  for (auto n = std::begin(snn.ppn); n < std::begin(snn.ppn) + 5; ++n)
  {
    (*n)->t_limit = time + 0.4;
    (*n)->fr = f_sal;

    EM.insert(new ActionTrialResetEvent(time + 0.4));

    double t_next = (*n)->next_spike_time(time);
    if (t_next <= (*n)->t_limit)
    {
      EM.insert(new SpikeEvent(t_next, *n));
    }
  }
  for (auto n = std::begin(snn.ppn) + 5; n < std::end(snn.ppn); ++n)
  {
    (*n)->t_limit = time + 2.4;
    (*n)->fr = f_bac;

    double t_next = (*n)->next_spike_time(time);
    if (t_next <= (*n)->t_limit)
    {
      EM.insert(new SpikeEvent(t_next, *n));
    }
  }

  if (it < last-1)
  {
    EM.insert(new RandomActionTrialEvent(time+2.4, it+1, last));
  }
}

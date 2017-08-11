#include "event.hpp"
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

EventManager::EventManager() : 
  duration(1.0),
  epoch_freq(20000.0), t_epoch{0.0, 0.0},
  rec_period(5.0), rec_entries(int(duration/rec_period)+1),
  w_record{std::vector<double>(rec_entries,0), std::vector<double>(rec_entries,0)}, 
  t_record(rec_entries,0)
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
      if (t_next <= EM.t_epoch[sy->post->group_id])
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
  }
          
  double t_next = neuron->next_spike_time(time);
  if (t_next <= EM.t_epoch[neuron->group_id])
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

    double norm_var_y = std::normal_distribution<double>{0.0, 1.0}(EM.gen);
    //if (group_id == 0)
    for (PPNeuron *&neuron : snn.group[group_id])
    {
      double norm_var_x = std::normal_distribution<double>{0.0, 1.0}(EM.gen);
      neuron->fr = corr_fr(norm_var_x, norm_var_y);
  
      double t_next = neuron->next_spike_time(time);
      if (t_next <= EM.t_epoch[group_id])
      {
        EM.insert(new SpikeEvent(t_next, neuron));
      } 
    }
    /*
    if (group_id == 1)
    {
      double norm_var_x = std::normal_distribution<double>{0.0, 1.0}(EM.gen);
      neuron->fr = uncorr_fr(norm_var_x);
  
      double t_next = neuron->next_spike_time(time);
      if (t_next <= EM.t_epoch[group_id])
      {
        EM.insert(new SpikeEvent(t_next, neuron));
      } 
    }
    */
  }
}

RecordEvent::RecordEvent(double time, int idx) : Event(time), idx(idx)
{
}

void RecordEvent::process(EventManager &EM, SNN &snn)
{
  int progress = 32*(time/EM.duration);
  std::cout << "\r progress [" << std::string(progress, '#') << std::string(32-progress, ' ') << "] " << std::setprecision(2) << std::fixed << time << "s " << std::flush;

  for (std::size_t group_id = 0; group_id < snn.group.size(); ++group_id)
  {
    double sum_w = 0.0;
    for (PPNeuron *&neuron : snn.group[group_id])
    {
      for (Synapse *&sy : snn.con.out(neuron))
      {
        sum_w += sy->get_w();
      }
    }
    EM.w_record[group_id][idx] = sum_w / (snn.group[group_id].size * W_MAX);
  }

  double t_delay = EM.rec_period < (EM.duration-time) ? EM.rec_period : (EM.duration-time);
  if (t_delay > 0)
  {
    EM.insert(new RecordEvent(time + t_delay, idx+1));
  }
}

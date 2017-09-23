/*
  author: Sam Coward
  date: Sept 2017
*/

#include "epochevent.hpp"
#include "spikeevent.hpp"
#include "../eventmanager.hpp"

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
      /*
      if (group_id == 0) 
      {
        (*it)->fr = corr_fr(norm_var_x, norm_var_y);
      }
      else
      {
        (*it)->fr = uncorr_fr(norm_var_x);
      }
      */
  
      double t_next = (*it)->next_spike_time(time);
      if (t_next <= (*it)->t_limit)
      {
        EM.insert(new SpikeEvent(t_next, *it));
      } 
    }
  }
}

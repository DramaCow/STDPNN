#include "recordevent.hpp"
#include "../eventmanager.hpp"

#include <iostream>

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
/*
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
*/

  double t_delay = EM.rec_period < (EM.duration-time) ? EM.rec_period : (EM.duration-time);
  if (t_delay > 0)
  {
    EM.insert(new RecordEvent(time + t_delay, idx+1));
  }
}

/*
  author: Sam Coward
  date: Sept 2017
*/

#include "progressevent.hpp"
#include "../eventmanager.hpp"

#include <iostream>

ProgressEvent::ProgressEvent(double time, int idx) : Event(time), idx(idx)
{
}

void ProgressEvent::process(EventManager &EM, SNN &snn)
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

  double t_delay = EM.rec_period < (EM.duration-time) ? EM.rec_period : (EM.duration-time);
  if (t_delay > 0)
  {
    EM.insert(new ProgressEvent(time + t_delay, idx+1));
  }
  else
  {
    std::cout << "\r COMPLETE [" 
              << std::string(32, '#') 
              << "] " 
              //<< std::setprecision(2) 
              //<< std::fixed 
              << EM.duration 
              << "s " 
              << std::endl;
  }
}

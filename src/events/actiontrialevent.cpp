#include "actiontrialevent.hpp"
#include "spikeevent.hpp"
#include "../eventmanager.hpp"
#include <algorithm>

#define f_sal 25.0 // salient fire rate
#define f_bac 3.0  // background fire rate
#define SIG 50

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

  for (auto n = std::begin(snn.ppn); n < std::begin(snn.ppn) + SIG; ++n)
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
  for (auto n = std::begin(snn.ppn) + SIG; n < std::end(snn.ppn); ++n)
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

  for (auto n = std::begin(snn.ppn); n < std::begin(snn.ppn) + SIG; ++n)
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
  for (auto n = std::begin(snn.ppn) + SIG; n < std::end(snn.ppn); ++n)
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

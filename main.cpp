#include <iostream>
#include <cstdio>
#include <random>
#include <cmath>
#include <string>
#include "neuron.hpp"
#include "synapse.hpp"
#include "event.hpp"

#include "SNN.hpp"

int main(int argc, char *argv[]) 
{
  if (argc < 2)
  {
    printf("usage: %s <fig_num>\n", argv[0]);
    return 1;
  }

  // global config
  const double dt_max = 0.0005;

  // initialise simulation state
  SNN snn;

  // initialise event queue
  EventManager EM;
  EM.insert(new EpochEvent(0.0, 0));
  EM.insert(new EpochEvent(0.0, 1));
  EM.insert(new RecordEvent(0.0, 0));

  // main loop
  while (EM.t_sim <= EM.duration && EM.size() > 0)
  {
    Event *e = EM.get_min();

    // synchronous update
    bool synch_event_inserted = false;
    while (EM.t_sim < e->time && !synch_event_inserted)
    {
      double dt = (dt_max <= (e->time-EM.t_sim)) ? dt_max : (e->time-EM.t_sim);
      EM.t_sim += dt;

      for (IFNeuron *&neuron : snn.sn)
      {
        neuron->step(dt);
        if (neuron->is_spiking())
        {
          EM.insert(new SpikeEvent(EM.t_sim, neuron));
          synch_event_inserted = true;
        }
      }
    }

    // event (asynchronous) update
    if (!synch_event_inserted)
    {
      e->process(EM, snn);
      EM.del_min();
    }
  }

  return 0;
}

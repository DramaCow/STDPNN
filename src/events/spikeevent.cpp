#include "spikeevent.hpp"
#include "../eventmanager.hpp"

#include <iostream>

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

#ifdef HOMEOSTASIS
  double ave_w = 0.0;
  for (Synapse *&sy : snn.con.in(snn.sn[0]))
  {
    ave_w += sy->get_w();
  }
  ave_w /= 1000.0;
  double scaling_factor = 0.5 / (ave_w/W_MAX);
  for (Synapse *&sy : snn.con.in(snn.sn[0]))
  {
    sy->w *= scaling_factor;
  }
#endif
}

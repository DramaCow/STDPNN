#include "SNN.hpp"

SNN::SNN() : con(1001)
{
  int id = 0;
  for (Neuron *&neuron : an)
  {
    neuron = new PPNeuron(id++, EXCITATORY);
  }
  for (Neuron *&neuron : sn)
  {
    neuron = new IFNeuron(id++, EXCITATORY);
  }

  for (Neuron *&neuron : an)
  {
    con.add_edge(neuron, sn[0], (W_MAX + W_MIN)/2);
  }
}

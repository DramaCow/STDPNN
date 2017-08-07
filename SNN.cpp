#include "SNN.hpp"

SNN::SNN() : con(1001)
{
  int id = 0;
  for (PPNeuron *&neuron : an)
  {
    neuron = new PPNeuron(id++, EXCITATORY);
  }
  for (IFNeuron *&neuron : sn)
  {
    neuron = new IFNeuron(id++, EXCITATORY);
  }

  for (PPNeuron *&neuron : an)
  {
    con.add_edge(neuron, sn[0], (W_MAX + W_MIN)/2);
  }
}

SNN::~SNN()
{
  for (PPNeuron *&neuron : an)
  {
    delete neuron;
  }
  for (IFNeuron *&neuron : sn)
  {
    delete neuron;
  }
}

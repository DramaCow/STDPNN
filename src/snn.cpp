#include "snn.hpp"
#include <random>
#include <iostream>
#include "units.hpp"

SNN::SNN(double duration, int ppn_size, int sn_size) :
  ppn(ppn_size), sn(sn_size),
  con(ppn.size() + sn.size()),
  d(0.0)
{
}

SNN::~SNN()
{
  for (PPNeuron *&neuron : ppn)
  {
    delete neuron;
  }
  for (Neuron *&neuron : sn)
  {
    delete neuron;
  }
}

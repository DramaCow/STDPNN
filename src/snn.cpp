#include "snn.hpp"
#include <random>
#include <iostream>

SNN::SNN(double duration) :
  ppn(1), sn(1),
  con(ppn.size() + sn.size()),
  d(0.0)
{
  int id = 0;

  for (PPNeuron *&neuron : ppn)
  {
    neuron = new PPNeuron(id++, EXCITATORY, duration);
  }
  for (Neuron *&neuron : sn)
  {
    neuron = new IzNeuron(id++, EXCITATORY, duration);
  }

  std::random_device rd;
  std::mt19937 gen; // random number generator

  for (PPNeuron *&neuron : ppn)
  {
    double w = std::uniform_real_distribution<double>{W_MIN, W_MAX}(gen);
    Synapse *sy_d1 = new GlutamateSynapse(neuron, sn[0], w, 1.20, 6.00, 1.20, 1.30,  0.00, -0.40, -0.50, d);
    con.add_synapse(sy_d1);
  }
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

#include "snn.hpp"
#include <random>
#include <iostream>
#include "units.hpp"

SNN::SNN(double duration) :
  ppn(200), sn(1),
  con(ppn.size() + sn.size()),
  d(10.0)
{
  int id = 0;

  for (PPNeuron *&neuron : ppn)
  {
    neuron = new PPNeuron(id++, EXCITATORY, duration);
  }
  for (Neuron *&neuron : sn)
  {
    neuron = new IzNeuron(id++, EXCITATORY, duration, con);
  }

  std::random_device rd;
  std::mt19937 gen; // random number generator

  for (PPNeuron *&neuron : ppn)
  {
    double w = std::uniform_real_distribution<double>{0.0, 0.46*nS}(gen);
    Synapse *sy_d1 = new GlutamateSynapse(neuron, sn[0], w, 1.20, 6.00, 1.20, 1.30,  0.00, -0.40, -0.50, d);
    con.add_synapse(sy_d1);
  }

  /*
  for (Synapse *&sy : dynamic_cast<IzNeuron*>(sn[0])->in)
  {
    std::cout << sy->pre->id << std::endl;
  }
  */
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

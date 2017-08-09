#include "SNN.hpp"
#include <random>

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

  std::random_device rd;
  std::mt19937 gen; // random number generator

  for (PPNeuron *&neuron : an)
  {
    //con.add_edge(neuron, sn[0], (W_MAX + W_MIN)/2);
    con.add_edge(neuron, sn[0], std::uniform_real_distribution<double>{W_MIN, W_MAX}(gen));
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

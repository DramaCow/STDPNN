#include "SNN.hpp"
#include <random>

SNN::SNN(double duration) : 
  con(A_NUM+S_NUM)
{
  int id = 0;

//  sn[0] = new IzNeuron(id, EXCITATORY, -1);

  for (Neuron *&neuron : ppn)
  {
    neuron = new PPNeuron(id++, EXCITATORY, duration);
  }
  for (Neuron *&neuron : sn)
  {
    neuron = new IFNeuron(id++, EXCITATORY, duration);
  }

  std::random_device rd;
  std::mt19937 gen; // random number generator

  for (Neuron *&neuron : ppn)
  {
    con.add_edge(neuron, sn[0], std::uniform_real_distribution<double>{W_MIN, W_MAX}(gen));
  }
}

SNN::~SNN()
{
  for (Neuron *&neuron : ppn)
  {
    delete neuron;
  }
  for (Neuron *&neuron : sn)
  {
    delete neuron;
  }
}

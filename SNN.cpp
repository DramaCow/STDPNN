#include "SNN.hpp"
#include <random>

SNN::SNN() : 
  group{Group(std::begin(an), std::begin(an)+550), Group(std::begin(an)+550, std::end(an))}, 
  con(A_NUM+S_NUM)
{
  int id = 0;

  for (PPNeuron *&neuron : an)
  {
    neuron = new PPNeuron(id++, EXCITATORY, id >= A_NUM/2);
  }
  for (IFNeuron *&neuron : sn)
  {
    neuron = new IFNeuron(id++, EXCITATORY, 0 /*doesn't matter*/);
  }

  std::random_device rd;
  std::mt19937 gen; // random number generator

  for (PPNeuron *&neuron : an)
  {
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

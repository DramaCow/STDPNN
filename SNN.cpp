#include "SNN.hpp"
#include <random>

SNN::SNN(double duration) : 
  group{Group(std::begin(an), std::begin(an)+(A_NUM/2)), Group(std::begin(an)+(A_NUM/2), std::end(an))}, 
  con(A_NUM+S_NUM)
{
  int id = 0;

//  sn[0] = new IzNeuron(id, EXCITATORY, -1);

  for (Neuron *&neuron : an)
  {
    neuron = new PPNeuron(id++, (id % (A_NUM/2)) < 500 ? EXCITATORY : INHIBITORY, id >= A_NUM/2, duration);
  }
  for (Neuron *&neuron : sn)
  {
    neuron = new IFNeuron(id++, EXCITATORY, 0, duration);
  }

  std::random_device rd;
  std::mt19937 gen; // random number generator

  for (Neuron *&neuron : an)
  {
    con.add_edge(neuron, sn[0], std::uniform_real_distribution<double>{W_MIN, W_MAX}(gen));
  }
}

SNN::~SNN()
{
  for (Neuron *&neuron : an)
  {
    delete neuron;
  }
  for (Neuron *&neuron : sn)
  {
    delete neuron;
  }
}

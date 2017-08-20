#include "SNN.hpp"
#include <random>

#include <iostream>

/*
SNN::SNN(double duration) : 
  con(A_NUM+S_NUM)
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
}
*/

SNN::SNN(double duration) : 
  con(A_NUM+S_NUM)
{
  int id = 0;

  for (PPNeuron *&neuron : ppn)
  {
    neuron = new PPNeuron(id++, EXCITATORY, duration);
  }
  for (Neuron *&neuron : sn)
  {
    neuron = new IFNeuron(id++, EXCITATORY, duration);
  }

  std::random_device rd;
  std::mt19937 gen; // random number generator

  for (PPNeuron *&neuron : ppn)
  {
    con.add_edge(neuron, sn[0], std::uniform_real_distribution<double>{W_MIN, W_MAX}(gen));
//    std::cout << neuron->id << " --> " << sn[0]->id << std::endl;
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

#include <cstdio>
#include "neuron.hpp"

#define A_p   0.005
#define tau_p 0.002
#define B     1.050
#define A_n   A_p*B
#define tau_n 0.002

Neuron::Neuron(int id): id(id)
{
  this->t_last = 0.0;
  this->x = 0.0;
  this->y = 0.0;
}

PPNeuron::PPNeuron(int id): Neuron(id)
{
}

bool PPNeuron::isSpiking()
{
  return false;
}

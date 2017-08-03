#include <cstdio>
#include <random>
#include <iostream>
#include "neuron.hpp"

int main() 
{
  IFNeuron N(0, EXCITATORY);
  
  for (int i = 0; i < 2; ++i)
  {
    while (!N.is_spiking())
    {
      N.step(0.0005);
    }
    N.spike();
  }

  for (double &V : N.V_record) {
    std::cout << V << " ";
  }

  std::cout << std::endl;

  for (double &t : N.t_record) {
    std::cout << t << " ";
  }

  return 0;
}

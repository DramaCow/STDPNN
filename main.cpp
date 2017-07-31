#include <cstdio>
#include <random>
#include <iostream>
#include "neuron.hpp"

int main() 
{
  PPNeuron N(5);
  N.setNextSpikeTime(0.0);
  std::cout << N.getNextSpikeTime() << std::endl;

  return 0;
}

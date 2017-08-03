#include <cstdio>
#include <random>
#include <iostream>
#include "neuron.hpp"

int main() 
{
  PPNeuron N(5, EXCITATORY);
  std::cout << N.next_spike_time(0.0) << std::endl;

  return 0;
}

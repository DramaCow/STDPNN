#include <cstdio>
#include <random>
#include <iostream>
#include "neuron.hpp"

int main() 
{
  PPNeuron N(5);
  std::cout << N.isSpiking() << std::endl;

  return 0;
}

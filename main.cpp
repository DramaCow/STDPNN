#include <cstdio>
#include "neuron.hpp"

int main() 
{
  PPNeuron N(5);
  printf("%s\n", N.isSpiking() ? "true" : "false");
  return 0;
}

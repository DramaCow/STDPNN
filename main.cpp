#include <cstdio>
#include <random>
#include <iostream>
#include "neuron.hpp"

#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <numpy/arrayobject.h>

int main() 
{/*
  PPNeuron I[1000];
  IFNeuron N(0, EXCITATORY);
  
  double t_sim = 0.0;
  for (int i = 0; i < 100000; ++i)
  {
    while (!N.is_spiking())
    {
      t_sim += 0.0005;
      N.step(0.0005);
    }
    N.update(t_sim);
    N.spike();

    for (int a = 0; a < 1000; ++a)
    {
      //I[a].
      I[a].update(t_sim);
    }
  }

  std::cout << t_sim << std::endl;
*/
/*
  for (double &V : N.V_record) {
    std::cout << V << " ";
  }

  std::cout << std::endl;

  for (double &t : N.t_record) {
    std::cout << t << " ";
  }
*/
  std::cout << "Hello, world!" << std::endl;
  return 0;
}

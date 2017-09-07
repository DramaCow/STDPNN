#include "synapse.hpp"
#include <cmath>
#include <iostream>

Synapse::Synapse(Neuron *pre, Neuron *post, double w) : 
  pre(pre), post(post), 
  w(w), 
  d1(1.0), d2(1.0),
  x(0.0), t_x(-INFINITY),
  y(0.0), t_y(-INFINITY)
{
}

double Synapse::get_w()
{
  return w;
}

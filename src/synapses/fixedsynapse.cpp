#include "fixedsynapse.hpp"
#include <cmath>
#include <iostream>

#define A_p   0.005
#define tau_p 0.020
#define A_n   A_p*1.050
#define tau_n 0.020

FixedSynapse::FixedSynapse(Neuron *pre, Neuron *post, double w) : Synapse(pre, post, w)
{
}

void FixedSynapse::pre_spike(double t)
{
  double dt = t - t_y;
  y *= exp(-dt/tau_n);

  w += y * W_MAX;
  w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;

  y -= A_n;
  t_x = t;
}

void FixedSynapse::post_spike(double t)
{
  double dt = t - t_x;
  x *= exp(-dt/tau_p);

  w += x * W_MAX;
  w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;

  x += A_p;
  t_y = t;
}

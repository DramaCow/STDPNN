#include <cstdio>
#include <cmath>
#include <iostream>
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

void Neuron::update(double t_curr)
{
  x *= exp(-(t_curr - t_last)/tau_p);
  y *= exp(-(t_curr - t_last)/tau_n);
  t_last = t_curr;
}

void Neuron::spike()
{
  x += A_p;
  y -= A_n;
}

PPNeuron::PPNeuron(int id) : Neuron(id)
{
  std::random_device rd; // slow rng used for one-off seed (makes use of device entropy)
  gen.seed(rd()); // standard mersenne_twister_engine

  this->fr = 20;
}

bool PPNeuron::isSpiking()
{
  //std::cout << std::exponential_distribution<double>{fr}(gen) << std::endl;
  return std::uniform_real_distribution<double>{0,1}(gen) < 0.5;
}


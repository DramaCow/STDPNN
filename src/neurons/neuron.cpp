//#include <boost/numeric/odeint.hpp>
//namespace odeint = boost::numeric::odeint;

#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>
#include "neuron.hpp"
#include "../units.hpp"

#define A_p   0.005
#define tau_p 0.020
#define A_n   A_p*1.050
#define tau_n 0.020

Neuron::Neuron(int id, int type, double t_limit) : id(id), type(type), t_limit(t_limit)
{
  this->t_last = 0.0;
  this->t_next = INFINITY;

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

double Neuron::get_x()
{
  return x;
}

double Neuron::get_y()
{
  return y;
}

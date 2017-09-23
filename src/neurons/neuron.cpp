/*
  author: Sam Coward
  date: Sept 2017
*/

//#include <boost/numeric/odeint.hpp>
//namespace odeint = boost::numeric::odeint;

#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>
#include "neuron.hpp"
#include "../units.hpp"

Neuron::Neuron(int id, int type, double t_limit) : id(id), type(type), t_limit(t_limit)
{
  this->t_next = INFINITY;
}

void Neuron::spike()
{
}

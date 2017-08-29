#include "ppneuron.hpp"

PPNeuron::PPNeuron(int id, int type, double t_limit) : Neuron(id, type, t_limit)
{
  std::random_device rd; // slow rng for one-off seed (uses  device entropy)
  gen.seed(rd()); // standard mersenne_twister_engine

  this->fr = 0.0;
}

bool PPNeuron::is_spiking()
{
  return t_last == t_next;
}

double PPNeuron::next_spike_time(double t)
{
  return t_next = fr > 0 ? t + std::exponential_distribution<double>{fr}(gen) : INFINITY;
}

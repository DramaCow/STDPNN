#include "fixedsynapse.hpp"

FixedSynapse::FixedSynapse(Neuron *pre, Neuron *post, double w) : Synapse(pre, post, w)
{
}

void FixedSynapse::pre_spike(double t)
{
  w += post->get_y() * W_MAX;
  w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;
}

void FixedSynapse::post_spike(double t)
{
  w += pre->get_x() * W_MAX;
  w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;
}

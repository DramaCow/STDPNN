#include "synapse.hpp"
#include <iostream>

Synapse::Synapse(Neuron *pre, Neuron *post, double w) : pre(pre), post(post), w(w), d1(1.0), d2(1.0)
{
}

double Synapse::get_w()
{
  return w;
}

/*
void Synapse::pre_spike()
{
//  if (pre->type != INHIBITORY)
//  {
    std::cout << w << " , ";
    w += d2 * post->get_y() * W_MAX;
    w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;
    std::cout << w << std::endl;
//  }
}

void Synapse::post_spike()
{
//  if (pre->type != INHIBITORY)
//  {
    std::cout << w << " , ";
    w += d1 * pre->get_x() * W_MAX;
    w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;
    std::cout << w << std::endl;
//  }
}
*/

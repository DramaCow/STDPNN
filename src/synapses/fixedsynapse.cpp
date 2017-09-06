#include "fixedsynapse.hpp"

FixedSynapse::FixedSynapse(Neuron *pre, Neuron *post, double w) : Synapse(pre, post, w)
{
}

void FixedSynapse::pre_spike()
{
//  if (pre->type != INHIBITORY)
//  {
//    std::cout << w << " , ";
    w += d2 * post->get_y() * W_MAX;
    w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;
//    std::cout << w << std::endl;
//  }
}

void FixedSynapse::post_spike()
{
//  if (pre->type != INHIBITORY)
//  {
//    std::cout << w << " , ";
    w += d1 * pre->get_x() * W_MAX;
    w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;
//    std::cout << w << std::endl;
//  }
}

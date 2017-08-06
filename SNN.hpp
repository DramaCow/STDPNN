#ifndef SNN_H
#define SNN_H

#include "neuron.hpp"
#include "synapse.hpp"
#include <array>

class SNN
{
  public:
    SNN();

    std::array<Neuron*, 1000> an; // asynchronous neurons
    std::array<Neuron*, 1> sn;    // synchronous neurons

    SynapseNetwork con;
};

#endif

#ifndef SNN_H
#define SNN_H

#include "neuron.hpp"
#include "synapse.hpp"
#include <array>

class SNN
{
  public:
    SNN();
    ~SNN();

    std::array<PPNeuron*, 1000> an; // asynchronous neurons
    std::array<IFNeuron*, 1> sn;    // synchronous neurons

    SynapseNetwork con;
};

#endif

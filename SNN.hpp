#ifndef SNN_H
#define SNN_H

#include "neuron.hpp"
#include "synapse.hpp"
#include <array>

#define A_NUM 200
#define S_NUM 1

class SNN
{
  public:
    SNN(double duration);
    ~SNN();

    std::array<Neuron*, A_NUM> ppn; // poison process neurons
    std::array<Neuron*, S_NUM> sn; // synchronous neurons

    SynapseNetwork con;
};

#endif

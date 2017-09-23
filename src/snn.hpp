/*
  author: Sam Coward
  date: Sept 2017
*/

#ifndef SNN_H
#define SNN_H

#include "allneurons.hpp"
#include "synapses/synapse.hpp"
#include "synapsenetwork.hpp"
#include <array>

class SNN
{
  public:
    SNN(double duration, int ppn_size, int sn_size);
    ~SNN();

    std::vector<PPNeuron*> ppn; // poison process neurons
    std::vector<Neuron*> sn;    // synchronous neurons

    SynapseNetwork con;

    double d; // dopamine level
};

#endif

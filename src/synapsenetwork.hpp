#ifndef SYNAPSENETWORK_H
#define SYNAPSENETWORK_H

#include "synapses/synapse.hpp"
#include <string>

class SynapseNetwork
{
  public:
    SynapseNetwork(int num_neurons);
    ~SynapseNetwork();

    void add_synapse(Synapse *sy);

    std::vector<Synapse *> & in(Neuron *post);
    std::vector<Synapse *> & out(Neuron *pre);

    std::vector<std::vector<Synapse *>> A_post; // TODO: move
    std::vector<std::vector<Synapse *>> A_pre;

    void write(std::string fig);

  private:
    int num_neurons;
};

#endif

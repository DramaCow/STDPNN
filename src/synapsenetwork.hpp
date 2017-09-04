#ifndef SYNAPSENETWORK_H
#define SYNAPSENETWORK_H

#include "synapses/synapse.hpp"

class SynapseNetwork
{
  public:
    SynapseNetwork(int num_neurons);
    ~SynapseNetwork();

    void add_edge(Neuron *pre, Neuron *post, double w);
    void add_synapse(Synapse *sy);

    std::vector<Synapse *> & in(Neuron *post);
    std::vector<Synapse *> & out(Neuron *pre);

    std::vector<std::vector<Synapse *>> A_post; // TODO: move
    std::vector<std::vector<Synapse *>> A_pre;

  private:
    int num_neurons;
};

#endif

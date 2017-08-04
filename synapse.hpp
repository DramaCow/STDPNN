#ifndef __SYNAPSE_H
#define __SYNAPSE_H

#include "neuron.hpp"

#include <vector>

#define W_MIN 0.0   // minimum peak conductance
#define W_MAX 0.015 // maximum peak conductance

class Synapse
{
  public:
    Synapse(Neuron *pre, Neuron *post, double w);

    const Neuron *pre;
    const Neuron *post;

    double get_w();

  private:
    double w;
};

class SynapseNetwork
{
  public:
    SynapseNetwork(int num_neurons);
    ~SynapseNetwork();

    void add_edge(Neuron *pre, Neuron *post, double w);

    std::vector<Synapse *> & inputs(Neuron *post);
    std::vector<Synapse *> & outputs(Neuron *pre);

    std::vector<std::vector<Synapse *>> A_post;
    std::vector<std::vector<Synapse *>> A_pre;

  private:
    int num_neurons;

};

#endif

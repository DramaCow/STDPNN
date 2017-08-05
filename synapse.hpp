#ifndef SYNAPSE_H
#define SYNAPSE_H

#include "neuron.hpp"
#include <vector>

#define W_MIN 0.0   // minimum peak conductance
#define W_MAX 0.015 // maximum peak conductance

class Synapse
{
  public:
    Synapse(Neuron *pre, Neuron *post, double w);

    Neuron *const pre;
    Neuron *const post;

    void pre_spike();
    void post_spike();

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

    std::vector<std::vector<Synapse *>> A_post; // TODO: move
    std::vector<std::vector<Synapse *>> A_pre;

  private:
    int num_neurons;
};

#endif

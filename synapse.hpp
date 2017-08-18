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

    virtual double g_ampa() { return 0.0; }
    virtual double g_nmda() { return 0.0; }
    virtual double g_gaba() { return 0.0; }

  protected:
    double w;
};

class GlutamateSynapse : public Synapse
{
  public:
    GlutamateSynapse(Neuron *pre, Neuron *post, double w) : Synapse(pre, post, w) {}

    virtual double g_ampa() { return 1.0*w; }
    virtual double g_nmda() { return 2.0*w; }
    virtual double g_gaba() { return 0.0;   }
};

class GABASynapse : public Synapse
{
  public:
    GABASynapse(Neuron *pre, Neuron *post, double w) : Synapse(pre, post, w) {}

    virtual double g_ampa() { return 0.0;   }
    virtual double g_nmda() { return 0.0;   }
    virtual double g_gaba() { return 1.4*w; }
};

class SynapseNetwork
{
  public:
    SynapseNetwork(int num_neurons);
    ~SynapseNetwork();

    void add_edge(Neuron *pre, Neuron *post, double w);

    std::vector<Synapse *> & in(Neuron *post);
    std::vector<Synapse *> & out(Neuron *pre);

    std::vector<std::vector<Synapse *>> A_post; // TODO: move
    std::vector<std::vector<Synapse *>> A_pre;

  private:
    int num_neurons;
};

#endif

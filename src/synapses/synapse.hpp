#ifndef SYNAPSE_H
#define SYNAPSE_H

#include "../neurons/neuron.hpp"

#define W_MIN 0.0   // minimum peak conductance
#define W_MAX 0.015 // maximum peak conductance

class Synapse
{
  public:
    Synapse(Neuron *pre, Neuron *post, double w);

    Neuron *const pre;
    Neuron *const post;

    virtual void pre_spike();
    virtual void post_spike();

    double get_w();

    virtual double g_ampa() { return 0.0; }
    virtual double g_nmda() { return 0.0; }
    virtual double g_gaba() { return 0.0; }

    double d1, d2;

    double w;

  protected:
//    double w;
};

#endif

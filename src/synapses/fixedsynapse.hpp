#ifndef FIXEDSYNAPSE_H
#define FIXEDSYNAPSE_H

#include "synapse.hpp"

class FixedSynapse : public Synapse
{
  public:
    FixedSynapse(Neuron *pre, Neuron *post, double w);

    virtual void pre_spike();
    virtual void post_spike();

    virtual double g_ampa() { return 0.0; }
    virtual double g_nmda() { return 0.0; }
    virtual double g_gaba() { return 0.0; }
};

#endif

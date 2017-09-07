#ifndef FIXEDSYNAPSE_H
#define FIXEDSYNAPSE_H

#include "synapse.hpp"

class FixedSynapse : public Synapse
{
  public:
    FixedSynapse(Neuron *pre, Neuron *post, double w);

    virtual void pre_spike(double t);
    virtual void post_spike(double t);

    virtual double g_ampa() { return 0.0; }
    virtual double g_nmda() { return 0.0; }
    virtual double g_gaba() { return 0.0; }

    virtual void write(std::string fig);
};

#endif

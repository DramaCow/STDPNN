#ifndef GABASYNAPSE_H
#define GABASYNAPSE_H

#include "synapse.hpp"

class GABASynapse : public Synapse
{
  public:
    GABASynapse(Neuron *pre, Neuron *post, double w) : Synapse(pre, post, w) {}

    virtual double g_ampa() { return 0.0;   }
    virtual double g_nmda() { return 0.0;   }
    virtual double g_gaba() { return 1.4*w; }
};

#endif

#ifndef GLUTAMATESYNAPSE_H
#define GLUTAMATESYNAPSE_H

#include "synapse.hpp"

class GlutamateSynapse : public Synapse
{
  public:
    GlutamateSynapse(Neuron *pre, Neuron *post, double w) : Synapse(pre, post, w) {}

    virtual double g_ampa() { return 1.0*w; }
    virtual double g_nmda() { return 2.0*w; }
    virtual double g_gaba() { return 0.0;   }
};

#endif

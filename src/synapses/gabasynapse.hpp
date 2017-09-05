#ifndef GABASYNAPSE_H
#define GABASYNAPSE_H

#include "dopaminergicsynapse.hpp"

class GABASynapse : public DopaminergicSynapse
{
  public:
    GABASynapse(Neuron *pre, Neuron *post, double w, 
      double rho, double theta, double a0, double k_p_hi, double k_n_hi, double k_p_lo, double k_n_lo, double &d) :
      DopaminergicSynapse(pre, post, w, rho, theta, a0, k_p_hi, k_n_hi, k_p_lo, k_n_lo, d) {}

    virtual double g_ampa() { return 0.0;   }
    virtual double g_nmda() { return 0.0;   }
    virtual double g_gaba() { return 1.4*w; }
};

#endif

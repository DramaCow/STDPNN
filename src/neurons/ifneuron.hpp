#ifndef IFNEURON_H
#define IFNEURON_H

#include "neuron.hpp"
#include "../allsynapses.hpp"

class IFNeuron : public Neuron
{
  public:
    IFNeuron(int id, int type, double t_limit);

    virtual void spike();  

    virtual void step(double dt);
    virtual void receive_spike(Synapse *sy);
    virtual bool is_spiking();
    virtual double next_spike_time(double t);

  private:
    double V_rest;
    double V_thresh;
    double V_reset;
    double tau_m;

    double E_ex;
    double tau_ex;

    double E_in;
    double tau_in;

    double V;
    double g_ex;
    double g_in;

    double dVdt(double V, double g_ex, double g_in);
    double dg_exdt(double g_ex);
    double dg_indt(double g_in);
};

#endif

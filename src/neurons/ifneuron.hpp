/*
  author: Sam Coward
  date: Sept 2017
*/

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
    virtual void write(std::string fig);

    std::vector<double> t_record;
    std::vector<double> V_record;

  private:
    const double V_rest;
    const double V_thresh;
    const double V_reset;
    const double tau_m;
    const double E_ex;
    const double tau_ex;
    const double E_in;
    const double tau_in;

    double V;
    double g_ex;
    double g_in;

    double dVdt(double V, double g_ex, double g_in);
    double dg_exdt(double g_ex);
    double dg_indt(double g_in);
};

#endif

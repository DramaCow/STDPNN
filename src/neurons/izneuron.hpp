#ifndef IZNEURON_H
#define IZNEURON_H

#include "neuron.hpp"
#include "../allsynapses.hpp"
#include "../synapsenetwork.hpp"

class IzNeuron : public Neuron
{
  public:
    IzNeuron(int id, int type, double t_limit, SynapseNetwork &con);

    virtual void spike();  

    virtual void step(double dt);
    virtual void receive_spike(Synapse *sy);
    virtual bool is_spiking();
    virtual double next_spike_time(double t);

    std::vector<double> v_record;
    std::vector<double> u_record;
    std::vector<double> t_record;

    std::vector<Synapse*> &in;
    std::vector<Synapse*> &out;

  private:
    const double a;      // dominant ion channel time const
    const double b;      // arbitrary scaling const
    const double c;      // reset potential (mV)
    const double k;      // arbitrary scaling const
    const double v_r;    // rest potential (mV)
    const double v_peak; // peak potential (mV)
    const double C;      // capacitance (pF)
    const double v_t;    // threshold potential (mV)
    const double d;      // after spike reset of u

    const double E_ampa;
    const double E_nmda;
    const double E_gaba;
    const double tau_ampa;
    const double tau_nmda;
    const double tau_gaba;
    const double Mg2p0; // magnesium 2+ init

    double B(double v);

    double v; // membrane potential (mV)
    double u; // recovery variable (contribution of dominant ion channel)

    double h_ampa;
    double h_nmda;
    double h_gaba;

    double g_ampa;
    double g_nmda;
    double g_gaba;

//    std::vector<Synapse*> &in;
//    std::vector<Synapse*> &out;
};

#endif

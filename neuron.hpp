#ifndef NEURON_H
#define NEURON_H

#include <random>

#define INHIBITORY 0
#define EXCITATORY 1

class Synapse;

class Neuron
{
  public:
    Neuron(int id, int type, double t_limit);

    const int id;
    const int type;
    double t_limit;

    virtual void update(double t_curr);
    virtual void spike();  

    virtual void step(double dt) = 0;
    virtual void receive_spike(Synapse *sy) = 0;
    virtual bool is_spiking() = 0;
    virtual double next_spike_time(double t) = 0;

    double get_x();
    double get_y();

    std::vector<double> spikes;

  protected:
    double t_last;
    double t_next;

    double x;
    double y;
};

#include "synapse.hpp"

class PPNeuron : public Neuron
{
  public:
    PPNeuron(int id, int type, double t_limit);

    virtual void step(double dt) {}
    virtual void receive_spike(Synapse *sy) {}
    virtual bool is_spiking();
    virtual double next_spike_time(double t);

    double fr;        // fire rate

  private:
    std::mt19937 gen; // random number generator
    double t_next;    // time of next spike
};

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

class IzNeuron : public Neuron
{
  public:
    IzNeuron(int id, int type, double t_limit);

    virtual void spike();  

    virtual void step(double dt);
    virtual void receive_spike(Synapse *sy);
    virtual bool is_spiking();
    virtual double next_spike_time(double t);

    std::vector<double> v_record;
    std::vector<double> u_record;
    std::vector<double> t_record;

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
};

#endif

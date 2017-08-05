#ifndef NEURON_H
#define NEURON_H

#include <random>

#define INHIBITORY 0
#define EXCITATORY 1

class Neuron
{
  public:
    Neuron(int id, int type);

    const int id;
    const int type;

    virtual void update(double t_curr);
    virtual void spike();  

    virtual void step(double dt) = 0;
    virtual void receive_spike(int source_type, double strength) = 0;
    virtual bool is_spiking() = 0;
    virtual double next_spike_time(double t) = 0;

    double get_x();
    double get_y();

  protected:
    double t_last;
    double t_next;

    double x;
    double y;
};

class PPNeuron : public Neuron
{
  public:
    PPNeuron(int id, int type);

    virtual void step(double dt) {}
    virtual void receive_spike(int source_type, double strength) {}
    virtual bool is_spiking();
    virtual double next_spike_time(double t);

  private:
    std::mt19937 gen; // random number generator
    double fr;        // fire rate
    double t_next;    // time of next spike
};

class IFNeuron : public Neuron
{
  public:
    IFNeuron(int id, int type);

    virtual void spike();  

    virtual void step(double dt);
    virtual void receive_spike(int source_type, double strength);
    virtual bool is_spiking();
    virtual double next_spike_time(double t);

    std::vector<double> V_record;
    std::vector<double> t_record;

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

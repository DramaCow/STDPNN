#include "ifneuron.hpp"
#include "../units.hpp"

IFNeuron::IFNeuron(int id, int type, double t_limit) : Neuron(id, type, t_limit)
{
  tau_m    =  20.0 * ms;
  V_rest   = -74.0 * mV;
  V_thresh = -54.0 * mV;
  V_reset  = -60.0 * mV;

  E_ex     =   0.0 * mV;
  tau_ex   =   5.0 * ms;

  E_in     = -88.0 * mV;
  tau_in   =   5.0 * ms;

  V = V_rest;
  g_ex = g_in = 0;
}

void IFNeuron::spike()
{
  Neuron::spike();
  V = V_reset;
}

void IFNeuron::step(double dt)
{
  V    += dt * dVdt(V, g_ex, g_in);
  g_ex += dt * dg_exdt(g_ex);
  g_in += dt * dg_indt(g_in);
}

void IFNeuron::receive_spike(Synapse *sy)
{
  if (sy->pre->type == INHIBITORY)
  {
    g_in += 1.0*sy->get_w();
  }
  else if (sy->pre->type == EXCITATORY)
  {
//		g_ex += 1.1*sy->get_w();
    g_ex += 1.0*sy->get_w();
  }
}

bool IFNeuron::is_spiking()
{
  return V >= V_thresh;
}

double IFNeuron::next_spike_time(double t)
{
  return INFINITY;
}

double IFNeuron::dVdt(double V, double g_ex, double g_in)
{
  return (1.0/tau_m)*(V_rest - V + g_ex*(E_ex - V) + g_in*(E_in - V));
}
double IFNeuron::dg_exdt(double g_ex)
{
  return (1.0/tau_ex)*(-g_ex);
}
double IFNeuron::dg_indt(double g_in)
{
  return (1.0/tau_in)*(-g_in);
}

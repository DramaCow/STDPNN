#include "ifneuron.hpp"
#include "../units.hpp"
#include <cmath>
#include <algorithm>

IFNeuron::IFNeuron(int id, int type, double t_limit) : Neuron(id, type, t_limit)
{
  // TODO: use initialization lists, not this shit
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

  t_record.push_back(0.0);
  V_record.push_back(V);
}

void IFNeuron::spike()
{
  Neuron::spike();
  V = V_reset;

  t_record.push_back(t_record.back());
  V_record.push_back(V);
}

void IFNeuron::step(double dt)
{
  V    += dt * dVdt(V, g_ex, g_in);
  g_ex += dt * dg_exdt(g_ex);
  g_in += dt * dg_indt(g_in);

  t_record.push_back(t_record.back() + dt);
  V_record.push_back(V);
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

void IFNeuron::write(std::string fig)
{
  FILE* file = fopen((fig + "_if_" + std::to_string(id) + ".dat").c_str(), "wb");

  int count = V_record.size();
  int num_plots = 1;
  double ymin = *min_element(std::begin(V_record), std::end(V_record)); 
  double ymax = *max_element(std::begin(V_record), std::end(V_record));

  fwrite(&count, sizeof(int), 1, file);
  fwrite(&num_plots, sizeof(int), 1, file);
  fwrite(&ymin, sizeof(double), 1, file);
  fwrite(&ymax, sizeof(double), 1, file);
  fwrite(&t_record[0], sizeof(double), count, file);
  fwrite(&V_record[0], sizeof(double), count, file);

  fclose(file);
}

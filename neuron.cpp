#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>
#include "neuron.hpp"
#include "units.hpp"

#define A_p   0.005
#define tau_p 0.020
#define B     1.050
#define A_n   A_p*B
#define tau_n 0.020

Neuron::Neuron(int id, int type, int group_id): id(id), type(type), group_id(group_id)
{
  this->t_last = 0.0;
  this->t_next = INFINITY;

  this->x = 0.0;
  this->y = 0.0;
}

void Neuron::update(double t_curr)
{
  x *= exp(-(t_curr - t_last)/tau_p);
  y *= exp(-(t_curr - t_last)/tau_n);
  t_last = t_curr;
}

void Neuron::spike()
{
  x += A_p;
  y -= A_n;
}

double Neuron::get_x()
{
  return x;
}

double Neuron::get_y()
{
  return y;
}

PPNeuron::PPNeuron(int id, int type, int group_id) : Neuron(id, type, group_id)
{
  std::random_device rd; // slow rng for one-off seed (uses  device entropy)
  gen.seed(rd()); // standard mersenne_twister_engine

  this->fr = 0.0;
}

bool PPNeuron::is_spiking()
{
  return t_last == t_next;
}

double PPNeuron::next_spike_time(double t)
{
  return t_next = fr > 0 ? t + std::exponential_distribution<double>{fr}(gen) : INFINITY;
}

IFNeuron::IFNeuron(int id, int type, int group_id) : Neuron(id, type, group_id)
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

#ifdef DEBUG
  V_record.push_back(V);
  g_record.push_back(g_ex);
  t_record.push_back(0.0);
  y_record.push_back(y);
#endif
}

void IFNeuron::spike()
{
  Neuron::spike();
  V = V_reset;

#ifdef DEBUG
  V_record.push_back(V);
  g_record.push_back(g_ex);
  t_record.push_back(t_record.back());
  y_record.push_back(y);
#endif
}

void IFNeuron::step(double dt)
{
  V    += dt * dVdt(V, g_ex, g_in);
  g_ex += dt * dg_exdt(g_ex);
  g_in += dt * dg_indt(g_in);

#ifdef DEBUG
  V_record.push_back(V);
  g_record.push_back(g_ex);
  t_record.push_back(t_record.back() + dt);
  y_record.push_back(y);
#endif
}

void IFNeuron::receive_spike(Synapse *sy)
{
  if (sy->pre->type == INHIBITORY)
  {
    g_in += 1.0*sy->get_w();
  }
  else if (sy->pre->type == EXCITATORY)
  {
    g_ex += 1.1*sy->get_w();
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

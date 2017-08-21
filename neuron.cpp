//#include <boost/numeric/odeint.hpp>
//namespace odeint = boost::numeric::odeint;

#include <cstdio>
#include <cmath>
#include <iostream>
#include <vector>
#include "neuron.hpp"
#include "units.hpp"

#define A_p   0.005
#define tau_p 0.020
#define A_n   A_p*1.050
#define tau_n 0.020

Neuron::Neuron(int id, int type, double t_limit) : id(id), type(type), t_limit(t_limit), d1(1.0), d2(1.0)
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
  x += d1*A_p;
  y -= d2*A_n;
}

double Neuron::get_x()
{
  return x;
}

double Neuron::get_y()
{
  return y;
}

PPNeuron::PPNeuron(int id, int type, double t_limit) : Neuron(id, type, t_limit)
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

IzNeuron::IzNeuron(int id, int type, double t_limit) : 
  Neuron(id, type, t_limit),

  a      (  0.01 * 1          ), // dominant ion channel time const
  b      (   -20 * pF/ms      ), // arbitrary scaling const
  c      (   -55 * mV         ), // reset potential (mV),
  k      (     1 * pF/(mV*mV) ), // arbitrary scaling const
  v_r    (   -80 * mV         ), // rest potential (mV),
  v_peak (    40 * mV         ), // peak potential (mV),
  C      (  15.2 * pF         ), // capacitance (pF),
  v_t    ( -29.7 * mV         ), // threshold potential (mV),
  d      (    91 * (pF*mV)/ms ), // after spike reset of u

  E_ampa   (   0 * mV ), // ampa recovery potential
  E_nmda   (   0 * mV ), // nmda recovery potential
  E_gaba   (   0 * mV ), // gaba recovery potential
  tau_ampa (   6 * ms ), // ampa time constant
  tau_nmda ( 160 * ms ), // nmda time constant
  tau_gaba (   4 * ms ), // gaba time constant
  Mg2p0    (   1 * mM ), // for magnesium block function

  v(c),   // membrane potential (mV),
  u(0.0), // recovery variable (contribution of dominant ion channel),

  h_ampa(0.0),
  h_nmda(0.0),
  h_gaba(0.0)
{
  v_record.push_back(v);
  u_record.push_back(u);
  t_record.push_back(0.0);
}

double IzNeuron::B(double v)
{
  return 1.0 / (1.0 + (Mg2p0/3.57)*exp(-0.062*v));
}

void IzNeuron::spike()
{
  Neuron::spike();
  v = c;
  u += d;

  v_record.push_back(v);
  u_record.push_back(u);
  t_record.push_back(t_record.back());
}

void IzNeuron::step(double dt)
{
  double I = /*g_ampa* */h_ampa*(E_ampa - v) +
             /*g_nmda* */h_nmda*(E_nmda - v) +
             B(v)* /*g_gaba* */h_gaba*(E_gaba - v);

  // TODO: use 4th order runge-kutta, or whatever boost odeint implements
  v += dt * (k*(v - v_r)*(v - v_t) - u + 2000*pA/*+ I*/)/C;
  u += dt * a*(b*(v - v_r) - u);

  h_ampa += dt * -(h_ampa/tau_ampa);
  h_nmda += dt * -(h_nmda/tau_nmda);
  h_gaba += dt * -(h_gaba/tau_gaba);

  v_record.push_back(v);
  u_record.push_back(u);
  t_record.push_back(t_record.back() + dt);
}

void IzNeuron::receive_spike(Synapse *sy)
{
  // TODO
}

bool IzNeuron::is_spiking()
{
  return v > v_peak;
}

double IzNeuron::next_spike_time(double t)
{
  // REDUNDANT
  return INFINITY;
}

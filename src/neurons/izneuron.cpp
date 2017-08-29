#include "izneuron.hpp"
#include "../units.hpp"

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

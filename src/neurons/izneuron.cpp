/*
  author: Sam Coward
  date: Sept 2017
*/

#include "izneuron.hpp"
#include "../units.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

IzNeuron::IzNeuron(int id, int type, double t_limit, SynapseNetwork &con) :
  Neuron(id, type, t_limit),
  in(con.in(this)), out(con.out(this)),

  a      (  0.01 * 1/ms       ), // dominant ion channel time const
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
  t_record.push_back(0.0);
  v_record.push_back(v);
  u_record.push_back(u);
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
  double g_ampa, g_nmda, g_gaba;
  g_ampa = g_nmda = g_gaba = 0.0;
  for (Synapse *&sy : in)
  {
    g_ampa += sy->g_ampa();
    g_nmda += sy->g_nmda();
    g_gaba += sy->g_gaba();
  }

  double I = g_ampa* h_ampa*(E_ampa - v) +
             g_nmda* h_nmda*(E_nmda - v) +
             B(v)* g_gaba* h_gaba*(E_gaba - v);

  // TODO: use 4th order runge-kutta, or whatever boost odeint implements
  //v += dt * (k*(v - v_r)*(v - v_t) - u + 2000*pA/*+ I*/)/C;
  v += dt * (k*(v - v_r)*(v - v_t) - u + 400*pA/*I*/)/C;
  u += dt * a*(b*(v - v_r) - u);

  h_ampa += dt * -(h_ampa/tau_ampa);
  h_nmda += dt * -(h_nmda/tau_nmda);
  h_gaba += dt * -(h_gaba/tau_gaba);

  t_record.push_back(t_record.back() + dt);
  v_record.push_back(v);
  u_record.push_back(u);
}

void IzNeuron::receive_spike(Synapse *sy)
{
  //std::cout << h_ampa << " " << h_nmda << " " << h_gaba << std::endl;
  //std::cout << sy->get_w() << std::endl;
  //std::cout << sy->g_ampa() << " " << sy->g_nmda() << " " << sy->g_gaba() << std::endl;

  h_ampa += sy->g_ampa() > 0;
  h_nmda += sy->g_nmda() > 0;
  h_gaba += sy->g_gaba() > 0;
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

void IzNeuron::write(std::string fig)
{
  FILE* file = fopen((fig + "_iz_" + std::to_string(id) + ".dat").c_str(), "wb");

  int count = v_record.size();
  int num_plots = 1;
  double ymin = *min_element(std::begin(v_record), std::end(v_record)); 
  double ymax = *max_element(std::begin(v_record), std::end(v_record));

  fwrite(&count, sizeof(int), 1, file);
  fwrite(&num_plots, sizeof(int), 1, file);
  fwrite(&ymin, sizeof(double), 1, file);
  fwrite(&ymax, sizeof(double), 1, file);
  fwrite(&t_record[0], sizeof(double), count, file);
  fwrite(&v_record[0], sizeof(double), count, file);

  fclose(file);
}

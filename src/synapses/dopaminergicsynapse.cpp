#include "dopaminergicsynapse.hpp"
#include <cmath>
#include "../units.hpp"

#include <iostream>

DopaminergicSynapse::DopaminergicSynapse(Neuron *pre, Neuron *post, double w, 
  double rho, double theta, double a0, double k_p_hi, double k_n_hi, double k_p_lo, double k_n_lo, double &d) : 
  Synapse(pre, post, w),

  x(0.0), t_x(0.0),
  y(0.0), t_y(0.0),

  rho    (rho   ) , // Naka-Rushton exponent
  theta  (theta ) , // Nake-Rushton threshold
  a0     (a0    ) , // maximum value
  tau_p  (0.02  ) , // kernel p time const.
  tau_n  (0.02  ) , // kernel n time const.
  tau_g  (0.3   ) , // eligibility time const.
  mu     (0.65  ) , // learn rate

  k_p_hi (k_p_hi) ,
  k_n_hi (k_n_hi) ,
  k_p_lo (k_p_lo) ,
  k_n_lo (k_n_lo) ,

  d(d)
{
}

void DopaminergicSynapse::pre_spike(double t)
{
  double dt = t - t_x;
  y *= exp(-dt/tau_n);
  w += mu * y;
  w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;

  y += z_n(d, 0); // 0 is for laziness
  t_y = t;
}

void DopaminergicSynapse::post_spike(double t)
{
  double dt = t - t_x;
  x *= exp(-dt/tau_p);
  w += mu * x;
  w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;

  x += z_p(d, 0); // 0 is for laziness
  t_x = t;
}

double DopaminergicSynapse::alpha(double d)
{
  return (a0*pow(d,rho)) / (pow(d,rho)+pow(theta,rho));
}

double DopaminergicSynapse::z_p(double d, double dt)
{
  double K = alpha(d)*k_p_hi + (1 - alpha(d))*k_p_lo;
  return K*exp(-dt/tau_p);
}

double DopaminergicSynapse::z_n(double d, double dt)
{
  double K = alpha(d)*k_n_hi + (1 - alpha(d))*k_n_lo;
  return K*exp(-dt/tau_n);
}

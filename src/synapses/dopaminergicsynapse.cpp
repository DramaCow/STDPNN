#include "dopaminergicsynapse.hpp"
#include <cmath>

DopaminergicSynapse::DopaminergicSynapse(Neuron *pre, Neuron *post, double w, 
  double rho, double theta, double a0, double k_p_hi, double k_n_hi, double k_p_lo, double k_n_lo, double &d) : 
  Synapse(pre, post, w),

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

void DopaminergicSynapse::pre_spike()
{
  double dt = post->get_y();
  w += mu * z_p(d, dt);
  w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;
}

void DopaminergicSynapse::post_spike()
{
  double dt = pre->get_x();
  w += mu * z_n(d, dt);
  w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;
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

/*
  author: Sam Coward
  date: Sept 2017
*/

#include "dopaminergicsynapse.hpp"
#include <cmath>
#include <algorithm>
#include "../units.hpp"

#include <iostream>

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

void DopaminergicSynapse::write(std::string fig)
{
  std::vector<double> time;
  std::vector<double> stdp;

  double t;
  t = -0.1;
  for (int i = 0; i < 101; ++i)
  {
    time.push_back(t);
		stdp.push_back(mu*z_n(d,-t));
    t += 0.001;
  }
  t = 0.0;
  for (int i = 0; i < 101; ++i)
  {
    time.push_back(t);
    stdp.push_back(mu*z_p(d,t));
    t += 0.001;
  }

  FILE* file; 
  if (pre == nullptr || post == nullptr)
  {
    file = fopen((fig + "_dsy_example.dat").c_str(), "wb");
  }
  else
  {
    file = fopen((fig + "_dsy_" + std::to_string(pre->id) + "_" + std::to_string(post->id) + ".dat").c_str(), "wb");
  }

  int count = time.size();
  int num_plots = 1;
  double ymin = *min_element(std::begin(stdp), std::end(stdp)); 
  double ymax = *max_element(std::begin(stdp), std::end(stdp));

  fwrite(&count, sizeof(int), 1, file);
  fwrite(&num_plots, sizeof(int), 1, file);
  fwrite(&ymin, sizeof(double), 1, file);
  fwrite(&ymax, sizeof(double), 1, file);
  fwrite(&time[0], sizeof(double), count, file);
  fwrite(&stdp[0], sizeof(double), count, file);

  fclose(file);
}

#include "fixedsynapse.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>

#define A_p   0.005
#define tau_p 0.020
#define A_n   A_p*1.050
#define tau_n 0.020

FixedSynapse::FixedSynapse(Neuron *pre, Neuron *post, double w) : Synapse(pre, post, w)
{
}

void FixedSynapse::pre_spike(double t)
{
  double dt = t - t_y;
  y *= exp(-dt/tau_n);

  w += y * W_MAX;
  w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;

  y -= A_n;
  t_x = t;
}

void FixedSynapse::post_spike(double t)
{
  double dt = t - t_x;
  x *= exp(-dt/tau_p);

  w += x * W_MAX;
  w = w < W_MIN ? W_MIN : w > W_MAX ? W_MAX : w;

  x += A_p;
  t_y = t;
}

void FixedSynapse::write(std::string fig)
{
  std::vector<double> time;
  std::vector<double> stdp;

  double t;
  t = -0.1;
  for (int i = 0; i < 100; ++i)
  {
    time.push_back(t);
    stdp.push_back(-A_n*exp(t/tau_n));
    t += 0.001;
  }
  t = 0.0;
  for (int i = 0; i < 100; ++i)
  {
    time.push_back(t);
    stdp.push_back(A_p*exp(-t/tau_p));
    t += 0.001;
  }

  FILE* file; 
  if (pre == nullptr || post == nullptr)
  {
    file = fopen((fig + "_fsy_example.dat").c_str(), "wb");
  }
  else
  {
    file = fopen((fig + "_fsy_" + std::to_string(pre->id) + "_" + std::to_string(post->id) + ".dat").c_str(), "wb");
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

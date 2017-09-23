/*
  author: Sam Coward
  date: Sept 2017
*/

#include "synapsenetwork.hpp"
#include <iostream>

SynapseNetwork::SynapseNetwork(int num_neurons) :
  A_post(num_neurons, std::vector<Synapse *>()),
  A_pre(num_neurons, std::vector<Synapse *>()),
  num_neurons(num_neurons)
{
}

SynapseNetwork::~SynapseNetwork()
{
  for (std::vector<Synapse *> &node : A_post)
  {
    for (Synapse *&sy : node)
    {
      delete sy;
    }
  }
}

void SynapseNetwork::add_synapse(Synapse *sy)
{
  A_post[sy->pre->id].push_back(sy);
  A_pre[sy->post->id].push_back(sy);
}

std::vector<Synapse *> & SynapseNetwork::in(Neuron *post)
{
  return A_pre[post->id];
}

std::vector<Synapse *> & SynapseNetwork::out(Neuron *pre)
{
  return A_post[pre->id];
}

void SynapseNetwork::write(std::string fig)
{
  FILE* file = fopen((fig + "_sy_str.dat").c_str(), "wb");

  int count = 0;
  for (std::size_t id = 0; id < A_post.size(); ++id)
  {
    for (Synapse *&sy : A_post[id])
    {
      count++;
    }
  }
  int num_plots = 1;
  double ymin = 0.0, ymax = 1.0;

  fwrite(&count, sizeof(int), 1, file);
  fwrite(&num_plots, sizeof(int), 1, file);
  fwrite(&ymin, sizeof(double), 1, file);
  fwrite(&ymax, sizeof(double), 1, file);

  for (double snum = 0.0; snum < double(count); snum += 1.0)
  {
    fwrite(&snum, sizeof(double), 1, file);
  }
  for (std::size_t id = 0; id < A_post.size(); ++id)
  {
    for (Synapse *&sy : A_post[id])
    {
      double w = sy->get_w()/W_MAX;
      fwrite(&w, sizeof(double), 1, file);
    }
  }

  fclose(file);
}

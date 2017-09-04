#include "synapsenetwork.hpp"

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

void SynapseNetwork::add_edge(Neuron *pre, Neuron *post, double w)
{
  Synapse *sy = new Synapse(pre, post, w);
  A_post[pre->id].push_back(sy);
  A_pre[post->id].push_back(sy);
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

/*
  author: Sam Coward
  date: Sept 2017
*/

#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <string>

#define INHIBITORY 0
#define EXCITATORY 1

class Synapse;

class Neuron
{
  public:
    Neuron(int id, int type, double t_limit);

    const int id;
    const int type;
    double t_limit;

    virtual void spike();  

    virtual void step(double dt) = 0;
    virtual void receive_spike(Synapse *sy) = 0;
    virtual bool is_spiking() = 0;
    virtual double next_spike_time(double t) = 0;

    std::vector<double> spikes;

    virtual void write(std::string fig) = 0;

  protected:
    double t_next;
};

#endif

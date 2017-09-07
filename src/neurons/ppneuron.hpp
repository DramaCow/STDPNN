#ifndef PPNEURON_H
#define PPNEURON_H

#include <random>
#include "neuron.hpp"
#include "../allsynapses.hpp"

class PPNeuron : public Neuron
{
  public:
    PPNeuron(int id, int type, double t_limit);

    virtual void step(double dt) {}
    virtual void receive_spike(Synapse *sy) {}
    virtual bool is_spiking();
    virtual double next_spike_time(double t);

    double fr;        // fire rate

    virtual void write(std::string fig) {}

  private:
    std::mt19937 gen; // random number generator
    double t_next;    // time of next spike
};

#endif

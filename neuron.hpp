#ifndef NEURON_H
#define NEURON_H

class Neuron
{
  public:
    Neuron(int id);
    virtual bool isSpiking() = 0;

  protected:
    int id;
    double t_last;
    double x;
    double y;
};

class PPNeuron : public Neuron
{
  public:
    PPNeuron(int id);
    virtual bool isSpiking();
};

#endif

#ifndef __NEURON_H
#define __NEURON_H

#include <random>

class Neuron
{
  public:
    Neuron(int id);

    virtual void update(double t_curr);
    virtual void spike();    

    virtual void receiveAP() = 0;
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

    virtual void receiveAP() {}
    virtual bool isSpiking();

    double getNextSpikeTime();
    void   setNextSpikeTime(double t_curr);

  private:
    std::mt19937 gen; // random number generator
    double fr;        // fire rate
    double t_next;    // time of next spike
};

#endif

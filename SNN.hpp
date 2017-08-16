#ifndef SNN_H
#define SNN_H

#include "neuron.hpp"
#include "synapse.hpp"
#include <array>

#define A_NUM 1000
#define S_NUM 1

class Group
{
  public:
    Group(std::array<Neuron*,A_NUM>::iterator b, std::array<Neuron*,A_NUM>::iterator e) : b(b), e(e), size(std::distance(b, e)) {}
    std::array<Neuron*,A_NUM>::iterator begin() { return b; }
    std::array<Neuron*,A_NUM>::iterator end() { return e; }
    const int size;

  private:
    std::array<Neuron*,A_NUM>::iterator b;
    std::array<Neuron*,A_NUM>::iterator e;
};

class SNN
{
  public:
    SNN();
    ~SNN();

    std::array<Neuron*, A_NUM> an; // asynchronous neurons
    std::array<Neuron*, S_NUM> sn; // synchronous neurons

    SynapseNetwork con;

    std::array<Group, 2> group;
};

#endif

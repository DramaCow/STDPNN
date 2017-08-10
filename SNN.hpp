#ifndef SNN_H
#define SNN_H

#include "neuron.hpp"
#include "synapse.hpp"
#include <array>

#define A_NUM 1100
#define S_NUM 1

class Group
{
  public:
    Group(std::array<PPNeuron*,A_NUM>::iterator b, std::array<PPNeuron*,A_NUM>::iterator e) : b(b), e(e), size(std::distance(b, e)) {}
    std::array<PPNeuron*,A_NUM>::iterator begin() { return b; }
    std::array<PPNeuron*,A_NUM>::iterator end() { return e; }
    const int size;

  private:
    std::array<PPNeuron*,A_NUM>::iterator b;
    std::array<PPNeuron*,A_NUM>::iterator e;
};

class SNN
{
  public:
    SNN();
    ~SNN();

    std::array<PPNeuron*, A_NUM> an; // asynchronous neurons
    std::array<IFNeuron*, S_NUM> sn; // synchronous neurons

    SynapseNetwork con;

    std::array<Group, 2> group;
};

#endif

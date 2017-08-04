#include <iostream>
#include <random>
#include "neuron.hpp"
#include "event.hpp"

int main(int argc, char *argv[]) 
{
  if (argc < 2)
  {
    printf("usage: %s <output_filename>\n", argv[0]);
    return 1;
  }
/*
  PPNeuron I[1000];
  IFNeuron N(0, EXCITATORY);
  double t_sim = 0.0;
  for (int i = 0; i < 10; ++i)
  {
    while (!N.is_spiking())
    {
      t_sim += 0.0005;
      N.step(0.0005);
    }
    N.update(t_sim);
    N.spike();

    for (int a = 0; a < 1000; ++a)
    {
      I[a].update(t_sim);
    }
  }
*/

  EventQueue EQ;
  EQ.insert(new Event(0.5));

/*
  // export results to binary file
  FILE* file = fopen(argv[1], "wb");
  int entries = N.t_record.size();
  fwrite(&entries, sizeof(int), 1, file);
  fwrite(&N.t_record[0], sizeof(double), entries, file);
  fwrite(&N.V_record[0], sizeof(double), entries, file);
  fclose(file);
*/
  return 0;
}
/*
  for (auto iter = EQ.event_list.begin()+1; iter != EQ.event_list.end(); ++iter)
  {
    std::cout << (*iter)->time << std::endl;
  }
*/

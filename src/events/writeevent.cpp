/*
  author: Sam Coward
  date: Sept 2017
*/

#include "writeevent.hpp"
#include "../eventmanager.hpp"

void WriteEvent::process(EventManager &EM, SNN &snn)
{
    FILE* file = fopen(figure.c_str(), "wb");
    int count = snn.ppn.size();
    int num_plots = 1;
    double ymin = 0.0, ymax = 1.0;
    fwrite(&count, sizeof(int), 1, file);
    fwrite(&num_plots, sizeof(int), 1, file);
    fwrite(&ymin, sizeof(double), 1, file);
    fwrite(&ymax, sizeof(double), 1, file);
    for (double id = 0.0; id < snn.ppn.size(); id+=1.0)
    {
      fwrite(&id, sizeof(double), 1, file); 
    }
    for (std::size_t id = 0; id < snn.ppn.size(); ++id)
    {
      for (Synapse *&sy : snn.con.out(snn.ppn[id]))
      {
        double w = sy->get_w()/W_MAX;
        fwrite(&w, sizeof(double), 1, file);
      }
    }
    fclose(file);
}

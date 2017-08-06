#include <iostream>
#include <cstdio>
#include <random>
#include <cmath>
#include <string>
#include "neuron.hpp"
#include "synapse.hpp"
#include "event.hpp"

#include "SNN.hpp"

int main(int argc, char *argv[]) 
{
  if (argc < 2)
  {
    printf("usage: %s <fig_num>\n", argv[0]);
    return 1;
  }

  // global config
  const double dt_max = 0.0005;

  // initialise simulation state
  SNN snn;

  // initialise event queue
  EventManager EM;
  EM.insert(new EpochEvent(0.0, 0));
  EM.insert(new EpochEvent(0.0, 1));
  EM.insert(new RecordEvent(0.0, 0));

  // main loop
  while (EM.t_sim <= EM.duration && EM.size() > 0)
  {
    Event *e = EM.get_min();

    // synchronous update
    bool synch_event_inserted = false;
    while (EM.t_sim < e->time && !synch_event_inserted)
    {
      double dt = (dt_max <= (e->time-EM.t_sim)) ? dt_max : (e->time-EM.t_sim);
      EM.t_sim += dt;

      for (IFNeuron *&neuron : snn.sn)
      {
        neuron->step(dt);
        if (neuron->is_spiking())
        {
          EM.insert(new SpikeEvent(EM.t_sim, neuron));
          synch_event_inserted = true;
        }
      }
    }

    // event (asynchronous) update
    if (!synch_event_inserted)
    {
      e->process(EM, snn);
      EM.del_min();
    }
  }

  // export results to binary files
  std::string fig_num(argv[1]);
  {
    IFNeuron *N = snn.sn[0];
    FILE* file = fopen((fig_num + "A.dat").c_str(), "wb");
    int entries = N->t_record.size();
    fwrite(&entries, sizeof(int), 1, file);
    fwrite(&N->t_record[0], sizeof(double), entries, file);
    fwrite(&N->V_record[0], sizeof(double), entries, file);
    fclose(file);
  }
  {
    IFNeuron *N = snn.sn[0];
    FILE* file = fopen((fig_num + "B.dat").c_str(), "wb");
    int entries = N->t_record.size();
    fwrite(&entries, sizeof(int), 1, file);
    fwrite(&N->t_record[0], sizeof(double), entries, file);
    fwrite(&N->g_record[0], sizeof(double), entries, file);
    fclose(file);
  }
  {
    FILE* file = fopen((fig_num + "C.dat").c_str(), "wb");
    int entries = snn.an.size();
    fwrite(&entries, sizeof(int), 1, file);
    for (double id = 0.0; id < snn.an.size(); id+=1.0)
    {
      fwrite(&id, sizeof(double), 1, file); 
    }
    for (std::size_t id = 0; id < snn.an.size(); ++id)
    {
      for (Synapse *&sy : snn.con.out(snn.an[id]))
      {
        double w = sy->get_w()/W_MAX;
        fwrite(&w, sizeof(double), 1, file);
      }
    }
    fclose(file);
  }
  {
    FILE* file = fopen((fig_num + "D.dat").c_str(), "wb");
    fwrite(&EM.rec_entries, sizeof(int), 1, file);
    fwrite(&EM.t_record[0], sizeof(double), EM.rec_entries, file);
    fwrite(&EM.w1_record[0], sizeof(double), EM.rec_entries, file);
    fwrite(&EM.w2_record[0], sizeof(double), EM.rec_entries, file);
    fclose(file);
  }

  return 0;
}

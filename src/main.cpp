#include <iostream>
#include <iomanip>
#include <cstdio>
#include <random>
#include <cmath>
#include <string>
#include <algorithm>

#include "snn.hpp"
#include "eventmanager.hpp"
#include "allevents.hpp"
#include "units.hpp"

#include <typeinfo>

int main(int argc, char *argv[]) 
{
  if (argc < 2)
  {
    printf("usage: %s <fig_num>\n", argv[0]);
    return 1;
  }

  // global config
  const double duration = 2.4;
  const double dt_max = 0.00005;
  double t_sim = 0.0;

  // figure number as string
  std::string fig_num(argv[1]);

  // example outputting dopamine controlled STDP rules for d1/d2 dopaminergic neurons
  double d = 0.0; // level of dopamine
  GlutamateSynapse d1(nullptr, nullptr, 0.5*W_MAX, 1.20, 6.00, 1.20, 1.30,  0.00, -0.40, -0.50, d);
  GlutamateSynapse d2(nullptr, nullptr, 0.5*W_MAX, 1.40, 1.80, 1.00, 0.35, -0.85,  0.30,  0.30, d);
  std::vector<double> time;
  std::vector<double> stdp1;
  std::vector<double> stdp2;
  double t = -0.1;
  for (int i = 0; i < 100; ++i)
  {
    time.push_back(t);
    stdp1.push_back(d1.mu*d1.z_n(d,-t));
    stdp2.push_back(d2.mu*d2.z_n(d,-t));
    t += 0.001;
  }
  t = 0.0;
  for (int i = 0; i < 100; ++i)
  {
    time.push_back(t);
    stdp1.push_back(d1.mu*d1.z_p(d,t));
    stdp2.push_back(d2.mu*d2.z_p(d,t));
    t += 0.001;
  }
  FILE* f = fopen((fig_num + "BB.dat").c_str(), "wb");
  int count = time.size();
  int num_plots = 2;
  double ymin1 = *min_element(std::begin(stdp1), std::end(stdp1)); 
  double ymax1 = *max_element(std::begin(stdp1), std::end(stdp1));
  double ymin2 = *min_element(std::begin(stdp2), std::end(stdp2)); 
  double ymax2 = *max_element(std::begin(stdp2), std::end(stdp2));
  double ymin = ymin1 < ymin2 ? ymin1 : ymin2;
  double ymax = ymax1 > ymax2 ? ymax1 : ymax2;
  fwrite(&count, sizeof(int), 1, f);
  fwrite(&num_plots, sizeof(int), 1, f);
  fwrite(&ymin, sizeof(double), 1, f);
  fwrite(&ymax, sizeof(double), 1, f);
  fwrite(&time[0], sizeof(double), count, f);
  fwrite(&stdp1[0], sizeof(double), count, f);
  fwrite(&stdp2[0], sizeof(double), count, f);
  fclose(f);

  // initialise simulation state
  SNN snn(duration);

  // initialise event queue
  EventManager EM(duration);
  EM.insert(new RandomActionTrialEvent(0.0, 1));
  EM.insert(new RecordEvent(0.0, 0));

  // initialise some global recorders
  std::vector<double> s_record;

  std::cout << "SIM " << argv[1] << " (duration=" << EM.duration << "s) :" << std::endl;

  // main loop
  while (t_sim <= EM.duration && EM.size() > 0)
  {
    Event *e = EM.get_min();

    // synchronous update
    bool synch_event_inserted = false;
    while (t_sim < e->time && !synch_event_inserted)
    {
      double dt = (dt_max <= (e->time-t_sim)) ? dt_max : (e->time-t_sim);
      t_sim += dt;

      for (Neuron *&neuron : snn.sn)
      {
        neuron->step(dt);
        if (neuron->is_spiking())
        {
          EM.insert(new SpikeEvent(t_sim, neuron)); // TODO: why bother inserting, just process it here
          synch_event_inserted = true;

          s_record.push_back(t_sim);
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
  
  // TODO: uhh...
  std::sort(std::begin(snn.ppn), std::end(snn.ppn), [](Neuron *n1, Neuron *n2) { return n1->id < n2->id; });

  std::cout << "\r COMPLETE [" << std::string(32, '#') << "] " << /*std::setprecision(2) << std::fixed <<*/ EM.duration << "s " << std::endl;
  std::cout << " writing results to file...";

  // export results to binary files
  {
    IzNeuron *iz = dynamic_cast<IzNeuron*>(snn.sn[0]);
    FILE* file = fopen((fig_num + "B.dat").c_str(), "wb");
    int count = iz->u_record.size();
    int num_plots = 1;
    //double ymin = -80*mV, ymax = 40*mV;
    double ymin = *min_element(std::begin(iz->u_record), std::end(iz->u_record)); 
    double ymax = *max_element(std::begin(iz->u_record), std::end(iz->u_record));
    fwrite(&count, sizeof(int), 1, file);
    fwrite(&num_plots, sizeof(int), 1, file);
    fwrite(&ymin, sizeof(double), 1, file);
    fwrite(&ymax, sizeof(double), 1, file);
    fwrite(&iz->t_record[0], sizeof(double), count, file);
    fwrite(&iz->u_record[0], sizeof(double), count, file);
    fclose(file);
  }
  {
    FILE* file = fopen((fig_num + "S.dat").c_str(), "wb");

    int count = snn.ppn.size() + snn.sn.size();
    fwrite(&count, sizeof(int), 1, file);

    for (PPNeuron *&neuron : snn.ppn)
    {
      count = neuron->spikes.size();
      fwrite(&count, sizeof(int), 1, file);
      fwrite(&neuron->spikes[0], sizeof(double), count, file);
    }

    for (Neuron *&neuron : snn.sn)
    {
      count = neuron->spikes.size();
      fwrite(&count, sizeof(int), 1, file);
      fwrite(&neuron->spikes[0], sizeof(double), count, file);
    }
  }
  /*
  {
    FILE* file = fopen((fig_num + "A.dat").c_str(), "wb");
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
  {
    FILE* file = fopen((fig_num + "B.dat").c_str(), "wb");
    int count = EM.rec_entries;
    int num_plots = 2;
    double ymin = 0.0, ymax = 1.0;
    fwrite(&count, sizeof(int), 1, file);
    fwrite(&num_plots, sizeof(int), 1, file);
    fwrite(&ymin, sizeof(double), 1, file);
    fwrite(&ymax, sizeof(double), 1, file);
    fwrite(&EM.t_record[0], sizeof(double), count, file);
    fwrite(&EM.w1_record[0], sizeof(double), count, file);
    fwrite(&EM.w2_record[0], sizeof(double), count, file);
    fclose(file);
  }
  */
  /*
  {
    FILE* file = fopen((fig_num + "A.dat").c_str(), "wb");

    IzNeuron *iz = dynamic_cast<IzNeuron*>(snn.sn[0]);

    int count = iz->t_record.size();
    int num_plots = 1;
    double ymin = -80*mV, 
           ymax = 40*mV;

    fwrite(&count, sizeof(int), 1, file);
    fwrite(&num_plots, sizeof(int), 1, file);
    fwrite(&ymin, sizeof(double), 1, file);
    fwrite(&ymax, sizeof(double), 1, file);

    fwrite(&iz->t_record[0], sizeof(double), count, file);
    fwrite(&iz->v_record[0], sizeof(double), count, file);

    fclose(file);
  }
  {
    FILE* file = fopen((fig_num + "B.dat").c_str(), "wb");

    IzNeuron *iz = dynamic_cast<IzNeuron*>(snn.sn[0]);

    int count = iz->t_record.size();
    int num_plots = 1;
    double ymin = 0, 
           ymax = 5e-10;

    fwrite(&count, sizeof(int), 1, file);
    fwrite(&num_plots, sizeof(int), 1, file);
    fwrite(&ymin, sizeof(double), 1, file);
    fwrite(&ymax, sizeof(double), 1, file);

    fwrite(&iz->t_record[0], sizeof(double), count, file);
    fwrite(&iz->u_record[0], sizeof(double), count, file);

    fclose(file);
  }
  */

  std::cout << "\r results written to file!  " << std::endl;

  return 0;
}

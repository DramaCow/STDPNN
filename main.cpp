#include <iostream>
#include <cstdio>
#include <random>
#include <cmath>
#include <string>
#include "neuron.hpp"
#include "synapse.hpp"
#include "event.hpp"

#include "SNN.hpp"

double corr_fr(double x, double y);
double uncorr_fr(double x);

int main(int argc, char *argv[]) 
{
  if (argc < 2)
  {
    printf("usage: %s <fig_num>\n", argv[0]);
    return 1;
  }

  // global config
  const double duration = 50.0;
  const double ave_epoch_period = 0.02;

  // neuron config
  const int a_num = 1000;
  const int s_num = 1;
  PPNeuron *a_neuron[a_num];
  for (int id = 0; id < a_num; ++id)
  {
    a_neuron[id] = new PPNeuron(id, EXCITATORY);
  }
  IFNeuron *s_neuron[1];
  s_neuron[0] = new IFNeuron(a_num, EXCITATORY);

  // network config
  SynapseNetwork sn(a_num + s_num);
  for (int id = 0; id < a_num; ++id)
  {
    sn.add_edge(a_neuron[id], s_neuron[0], (W_MAX + W_MIN)/2.0);
  }

  // important variables and constants
  const double dt_max = 0.000005;
  double t_sim = 0.0;
  double t_epoch[2] = { 0.0, 0.0 };

  // initialise event queue
  EventQueue EQ;
  EQ.insert(new EpochEvent(0.0, 1, 0));
  EQ.insert(new EpochEvent(0.0, 1, 1));

  // set up recording
  const double rec_period = 0.5;
  const int rec_entries = int(duration/rec_period) + 1;
  double w1_record[rec_entries];
  double w2_record[rec_entries];
  double t_record[rec_entries];
  for (int i = 0; i < rec_entries; ++i)
  {
    t_record[i] = i*rec_period;
  }
  EQ.insert(new RecordEvent(t_sim, 2, 0));

  // random number generator engine
  std::mt19937 gen;
  {
    std::random_device rd; // slow rng for one-off seed (uses  device entropy)
    gen.seed(rd()); // standard mersenne_twister_engine
  }

  // main loop
  while (t_sim <= duration && EQ.size() > 0)
  {
    Event *e = EQ.get_min();

    // synchronous update
    bool synch_event_inserted = false;
    while (t_sim < e->time && !synch_event_inserted)
    {
      double dt = dt_max <= (e->time - t_sim) ? dt_max : (e->time - t_sim);
      t_sim += dt;
      for (int id = 0; id < s_num; ++id)
      {
        s_neuron[id]->step(dt);
        if (s_neuron[id]->is_spiking())
        {
          EQ.insert(new SpikeEvent(t_sim, 0, s_neuron[id]));
          synch_event_inserted = true;
        }
      }
    }

    // event (asynchronous) update
    if (!synch_event_inserted)
    {
      // TODO: use e->process polymorphism
      switch (e->type)
      {
        case 0: {
          SpikeEvent *se = dynamic_cast<SpikeEvent*>(e);

          se->neuron->update(t_sim);

          if (se->neuron->is_spiking())
          {
            for (Synapse *&sy : sn.outputs(se->neuron))
            {
              sy->post->update(t_sim);
              sy->pre_spike();
              sy->post->receive_spike(sy->post->type, sy->get_w());

              double t_next = sy->post->next_spike_time(t_sim);
              if (t_next <= duration)
              {
                EQ.insert(new SpikeEvent(t_next, 0, sy->post));
              }
            }

            for (Synapse *&sy : sn.inputs(se->neuron))
            {
              sy->pre->update(t_sim);
              sy->post_spike();
            }

            se->neuron->spike();
          }
          
          double t_next = se->neuron->next_spike_time(t_sim);
          if (t_next <= duration)
          {
            EQ.insert(new SpikeEvent(t_next, 0, se->neuron));
          }

          break;
        }

        case 1: {
          EpochEvent *ee = dynamic_cast<EpochEvent*>(e);

          if (ee->group_id == 0)
          {
            double norm_var_y = std::normal_distribution<double>{0.0, 1.0}(gen);
            for (int id = 0; id < a_num/2 ; ++id)
            {
              double norm_var_x = std::normal_distribution<double>{0.0, 1.0}(gen);
              a_neuron[id]->fr = corr_fr(norm_var_x, norm_var_y);
              //a_neuron[id]->fr = uncorr_fr(norm_var_x);

              double t_next = a_neuron[id]->next_spike_time(t_sim);
              if (t_next <= duration)
              {
                EQ.insert(new SpikeEvent(t_next, 0, a_neuron[id]));
              } 
            }
          }
          else if (ee->group_id == 1)
          {
            //double norm_var_y = std::normal_distribution<double>{0.0, 1.0}(gen);
            for (int id = a_num/2; id < a_num; ++id)
            {
              double norm_var_x = std::normal_distribution<double>{0.0, 1.0}(gen);
              //a_neuron[id]->fr = corr_fr(norm_var_x, norm_var_y);
              a_neuron[id]->fr = uncorr_fr(norm_var_x);

              double t_next = a_neuron[id]->next_spike_time(t_sim);
              if (t_next <= duration)
              {
                EQ.insert(new SpikeEvent(t_next, 0, a_neuron[id]));
              } 
            }
          }

          break;
        }

        case 2: {
          RecordEvent *re = dynamic_cast<RecordEvent*>(e);

          printf("[%.2f s]\n", t_sim);

          double ave_w_1 = 0.0;
          for (int id = 0; id < a_num/2; ++id)
          {
            for (Synapse *&sy : sn.outputs(a_neuron[id]))
            {
              ave_w_1 += sy->get_w();
            }
          }
          ave_w_1 /= (a_num/2)*W_MAX;

          double ave_w_2 = 0.0;
          for (int id = a_num/2; id < a_num; ++id)
          {
            for (Synapse *&sy : sn.outputs(a_neuron[id]))
            {
              ave_w_2 += sy->get_w();
            }
          }
          ave_w_2 /= (a_num/2)*W_MAX;

          w1_record[re->idx] = ave_w_1; 
          w2_record[re->idx] = ave_w_2; 

          double t_delay = rec_period < (duration-t_sim) ? rec_period : (duration-t_sim);
          if (t_delay > 0)
          {
            EQ.insert(new RecordEvent(t_sim + t_delay, 2, re->idx + 1));
          }

          break;
        }

        default: {
          std::cout << "ERROR - INVALID EVENT!" << std::endl;
          return 1;
        }
      }
      EQ.del_min();
    }
  }

  // export results to binary files
  std::string fig_num(argv[1]);
  {
    IFNeuron *N = dynamic_cast<IFNeuron*>(s_neuron[0]);
    FILE* file = fopen((fig_num + "A.dat").c_str(), "wb");
    int entries = N->t_record.size();
    fwrite(&entries, sizeof(int), 1, file);
    fwrite(&N->t_record[0], sizeof(double), entries, file);
    fwrite(&N->V_record[0], sizeof(double), entries, file);
    fclose(file);
  }
  {
    IFNeuron *N = dynamic_cast<IFNeuron*>(s_neuron[0]);
    FILE* file = fopen((fig_num + "B.dat").c_str(), "wb");
    int entries = N->t_record.size();
    fwrite(&entries, sizeof(int), 1, file);
    fwrite(&N->t_record[0], sizeof(double), entries, file);
    fwrite(&N->g_record[0], sizeof(double), entries, file);
    fclose(file);
  }
  {
    FILE* file = fopen((fig_num + "C.dat").c_str(), "wb");
    fwrite(&a_num, sizeof(int), 1, file);
    for (double id = 0.0; id < a_num; id+=1.0)
    {
      fwrite(&id, sizeof(double), 1, file); 
    }
    for (int id = 0; id < a_num; ++id)
    {
      for (Synapse *&sy : sn.outputs(a_neuron[id]))
      {
        double w = sy->get_w();
        fwrite(&w, sizeof(double), 1, file);
      }
    }
    fclose(file);
  }
  {
    FILE* file = fopen((fig_num + "D.dat").c_str(), "wb");
    fwrite(&rec_entries, sizeof(int), 1, file);
    fwrite(&t_record[0], sizeof(double), rec_entries, file);
    fwrite(&w1_record[0], sizeof(double), rec_entries, file);
    fwrite(&w2_record[0], sizeof(double), rec_entries, file);
    fclose(file);
  }

  return 0;
}

#define FRF 0.3
double corr_fr(double x, double y)
{
  double fr = 10.0*(1.0 + FRF*x + FRF*y);
  return fr < 0.0 ? 0.0 : fr;
}

double uncorr_fr(double x)
{
  double fr = 10.0*(1.0 + FRF*sqrt(2.0)*x);
  return fr < 0.0 ? 0.0 : fr;
}
/*
  for (auto iter = EQ.event_list.begin()+1; iter != EQ.event_list.end(); ++iter)
  {
    std::cout << (*iter)->time << std::endl;
  }
*/

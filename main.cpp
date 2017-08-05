#include <iostream>
#include <random>
#include "neuron.hpp"
#include "synapse.hpp"
#include "event.hpp"

int main(int argc, char *argv[]) 
{
/*
  if (argc < 2)
  {
    printf("usage: %s <output_filename>\n", argv[0]);
    return 1;
  }
*/

  // global config
  double duration = 10000.0;

  // neuron config
  const int a_num = 1000;
  const int s_num = 1;
  Neuron *a_neuron[a_num];
  for (int id = 0; id < a_num; ++id)
  {
    a_neuron[id] = new PPNeuron(id, EXCITATORY);
  }
  Neuron *s_neuron[1];
  s_neuron[0] = new IFNeuron(a_num, EXCITATORY);

  // network config
  SynapseNetwork sn(a_num + s_num);
  for (int id = 0; id < a_num; ++id)
  {
    sn.add_edge(a_neuron[id], s_neuron[0], (W_MAX + W_MIN)/2.0);
  }

  // important variables and constants
  double dt_max = 0.0005;
  double t_sim = 0.0;
  double t_epoch[2] = { 0.0, 0.0 };

  // initialise event queue
  EventQueue EQ;
  EQ.insert(new EpochEvent(0.0, 1, 0));
  EQ.insert(new EpochEvent(0.0, 1, 1));

  // set up recording
  double rec_period = 0.5;
  EQ.insert(new RecordEvent(0.0, 2));
  EQ.insert(new RecordEvent(duration, 2));

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
          // INSERT EVENT HERE
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
          break;
        }

        case 2: {
          RecordEvent *re = dynamic_cast<RecordEvent*>(e);
          break;
        }

        default: {
          std::cout << "ERROR!" << std::endl;
          return 1;
        }
      }
      EQ.del_min();
    }
  }

  // export results to binary file
  IFNeuron *N = dynamic_cast<IFNeuron*>(s_neuron[0]);
  FILE* file = fopen(argv[1], "wb");
  int entries = N->t_record.size();
  fwrite(&entries, sizeof(int), 1, file);
  fwrite(&N->t_record[0], sizeof(double), entries, file);
  fwrite(&N->V_record[0], sizeof(double), entries, file);
  fclose(file);

  return 0;
}
/*
  for (Synapse *&sy : sn.outputs(n1))
  {
    std::cout << sy->pre->id << " --> " << sy->post->id << std::endl;
  }
  for (Synapse *&sy : sn.inputs(n1))
  {
    std::cout << sy->post->id << " <-- " << sy->pre->id << std::endl;
  }

  for (auto iter = EQ.event_list.begin()+1; iter != EQ.event_list.end(); ++iter)
  {
    std::cout << (*iter)->time << std::endl;
  }
*/

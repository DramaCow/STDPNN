#include "recordevent.hpp"
#include "../eventmanager.hpp"

RecordEvent::RecordEvent(double time, int idx, std::string fig) : Event(time), idx(idx), fig(fig)
{
}

void RecordEvent::process(EventManager &EM, SNN &snn)
{
  auto begin  = std::begin(snn.ppn);
  auto middle = std::begin(snn.ppn) + 500;
  auto end    = std::end(snn.ppn);
  double sum_w;

  sum_w = 0.0;
  for (auto it = begin; it < middle; ++it)
  {
    for (Synapse *&sy : snn.con.out(*it))
    {
      sum_w += sy->get_w();
    }
  }
  EM.w1_record[idx] = (sum_w / (snn.ppn.size()/2.0)) / W_MAX;

  sum_w = 0.0;
  for (auto it = middle; it < end; ++it)
  {
    for (Synapse *&sy : snn.con.out(*it))
    {
      sum_w += sy->get_w();
    }
  }
  EM.w2_record[idx] = (sum_w / (snn.ppn.size()/2.0)) / W_MAX;

  double t_delay = EM.rec_period < (EM.duration-time) ? EM.rec_period : (EM.duration-time);
  if (t_delay > 0)
  {
    EM.insert(new RecordEvent(time + t_delay, idx+1, fig));
  }
  else
  {
    FILE* file = fopen((fig + "_ave.dat").c_str(), "wb");

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
}

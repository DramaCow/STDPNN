#include "dopamineevent.hpp"
#include "../eventmanager.hpp"

void DopamineEvent::process(EventManager &EM, SNN &snn)
{
  auto begin = group_id == 0 ? std::begin(snn.ppn)       : std::begin(snn.ppn) + 500;
  auto end   = group_id == 0 ? std::begin(snn.ppn) + 500 : std::end(snn.ppn);

  for (auto it = begin; it < end; ++it)
  {
    for (Synapse *&sy : snn.con.out(*it))
    {
      sy->d1 = d1;
      sy->d2 = d2;
    }
  }
}

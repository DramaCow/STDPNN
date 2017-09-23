import sys
import numpy as np
import matplotlib.pyplot as plt

def raster(event_times_list, **kwargs):
    """
    Creates a raster plot
    Parameters
    ----------
    event_times_list : iterable
                       a list of event time iterables
    color : string
            color of vlines
    Returns
    -------
    ax : an axis containing the raster plot
    """
    ax = plt.gca()
    for ith, trial in enumerate(event_times_list):
        plt.vlines(trial, ith - 0.475, ith + 0.475, **kwargs)
    plt.ylim(-0.5, len(event_times_list) - 0.5)
    plt.yticks(range(0, len(event_times_list)))
    return ax

with open(sys.argv[1], "rb") as f:
  spikes = []

  count, = np.fromfile(f, dtype=np.int32, count=1)
  print(count)
  for _ in range(count):
    count, = np.fromfile(f, dtype=np.int32, count=1)
    trial = np.fromfile(f, dtype=np.float64, count=count)
    spikes.append(trial)

  fig = plt.figure()
  ax = raster(spikes)
  plt.title('Spike Train')
  plt.xlabel('Time (s)')
  plt.ylabel('Neuron ID')
  plt.show()

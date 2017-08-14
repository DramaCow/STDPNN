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

def spikecount(N, width=50):
  t = np.arange(0, 20000, width)
  S = np.zeros(len(t), dtype=int)

  for n in N:
    S[int(n / width)] += 1

  return t, S/width, width

'''
def correlogram(X, Y, maxDelta=10, bins=200):
	# 2x to account negative direction
	# +1 to allow max-min being considered a valid time, 
	#    without causing array-out-of-bounds
	dt = 2 * ((maxDelta + 1) / bins) 

	extents = int(bins/2)

	# +1 accounts for index at 0 -> not considered positive or negative
	D = np.array([b*dt for b in range(-extents, extents + (1 if bins % 2 == 1 else 0))])
	B = np.zeros(bins, dtype=int)

	for x in X:
		for y in Y:
			n = floor((x - y) / dt)
			if abs(n) < extents:
				B[n + extents] += 1

  return D, B, dt
'''

spikes = []
for i in range(5):
  with open(str(i)+"_"+sys.argv[1]+"S.dat", "rb") as f:
    spikes.append(np.fromfile(f, dtype=np.float64))

    #fig = plt.figure()
    #ax = raster(spikes)
    #plt.title('Spike Train')
    #plt.xlabel('Time (s)')
    #plt.ylabel('Neuron ID')
    #plt.show()

    TT, S, width = spikecount(spikes[i])
    plt.bar(TT, S, width, color="k")
    plt.show()

print(spikes)

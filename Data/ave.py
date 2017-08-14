import numpy as np
import matplotlib.pyplot as plt
import sys

if len(sys.argv) > 1:
  x = []
  y1 = []
  y2 = []

  for i in range(10):
    with open(sys.argv[1]+"_"+str(i)+"B.dat", "rb") as f:
      count, = np.fromfile(f, dtype=np.int32, count=1)
      num_plots, = np.fromfile(f, dtype=np.int32, count=1)
      ymin, = np.fromfile(f, dtype=np.float64, count=1)
      ymax, = np.fromfile(f, dtype=np.float64, count=1)
      x_read = np.fromfile(f, dtype=np.float64, count=count)
      y1_read = np.fromfile(f, dtype=np.float64, count=count)
      y2_read = np.fromfile(f, dtype=np.float64, count=count)

      if y1_read[-1] < y2_read[-1]:
        y1_read, y2_read = y2_read, y1_read

      x.append(x_read)
      y1.append(y1_read)
      y2.append(y2_read)

  x = np.mean(x, axis=0)
  y1 = np.mean(y1, axis=0)
  y2 = np.mean(y2, axis=0)

  plt.plot(x, y1, markersize=2)
  plt.plot(x, y2, markersize=2)
  plt.ylim((0, 1))
  plt.show()

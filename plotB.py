import numpy as np
import matplotlib.pyplot as plt
import sys

if len(sys.argv) <= 1:
  print("usage: python plot.py <input_filename>")
else:
  with open(sys.argv[1], "rb") as f:
    count, = np.fromfile(f, dtype=np.int32, count=1)
    num_plots, = np.fromfile(f, dtype=np.int32, count=1)
    ymin, = np.fromfile(f, dtype=np.float64, count=1)
    ymax, = np.fromfile(f, dtype=np.float64, count=1)
    x = np.fromfile(f, dtype=np.float64, count=count)
    Y = [np.fromfile(f, dtype=np.float64, count=count) for _ in range(num_plots)]

    print("count =", count)

    if len(sys.argv) > 2:
      for y in Y:
        plt.plot(x, y, sys.argv[2], markersize=2)
    else:
      for y in Y:
        plt.plot(x, y, markersize=2)
    plt.ylim((ymin, ymax))
    plt.show()

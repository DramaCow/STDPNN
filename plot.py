import numpy as np
import matplotlib.pyplot as plt
import sys

if len(sys.argv) <= 1:
  print("usage: python plot.py <input_filename>")
else:
  with open(sys.argv[1], "rb") as f:
    count, = np.fromfile(f, dtype=np.int32, count=1)
    x = np.fromfile(f, dtype=np.float64, count=count)
    y1 = np.fromfile(f, dtype=np.float64, count=count)
    y2 = np.fromfile(f, dtype=np.float64, count=count)

    print("count =", count)

    #plt.plot(x, y1, "k.", markersize=2)
    plt.plot(x, y1, "b-", markersize=2)
    plt.plot(x, y2, "r-", markersize=2)
    plt.ylim((0, 1))
    plt.show()

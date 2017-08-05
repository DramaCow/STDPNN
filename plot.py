import numpy as np
import matplotlib.pyplot as plt
import sys

if len(sys.argv) <= 1:
  print("usage: python plot.py <input_filename>")
else:
  with open(sys.argv[1], "rb") as f:
    count, = np.fromfile(f, dtype=np.int32, count=1)
    x = np.fromfile(f, dtype=np.float64, count=count)
    y = np.fromfile(f, dtype=np.float64, count=count)

    print("count =", count)
    print("x =", min(x), ":", max(x))
    print("y =", min(y), ":", max(y))

    plt.plot(x, y)
    plt.show()

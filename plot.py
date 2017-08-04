import numpy as np
import matplotlib.pyplot as plt
import sys

if len(sys.argv) <= 1:
  print("usage: python plot.py <input_filename>")
else:
  with open(sys.argv[1], "rb") as f:
    count, = np.fromfile(f, dtype=np.int32, count=1)
    t_record = np.fromfile(f, dtype=np.float64, count=count)
    V_record = np.fromfile(f, dtype=np.float64, count=count)

    plt.plot(t_record, V_record)
    plt.show()

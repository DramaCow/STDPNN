import numpy as np
import matplotlib.pyplot as plt

def corr_fr(x, y):
  return max((10*(1 + 0.3*x + 0.3*y), 0.0))
def uncorr_fr(x):
  return max((10*(1 + 0.3*np.sqrt(2)*x), 0.0))

def count(w):
  above = 0
  for i in range(len(w)):
    if w[i] > 0.0075:
      above = above + 1
  below = len(w) - above
  print("above =", above)
  print("below =", below)

def plot(w, fignum=None):
  plt.plot(range(len(w)), [w[i]/0.015 for i in range(len(w))], 'k.', markersize=2)
  plt.ylim((-0.025,1.025))
  if not fignum == None:
    plt.savefig(str(fignum)+".png")
    plt.close()
  else:
    plt.show()

w1 = np.clip(np.array([0.015*(0.7+0.10*np.random.normal(0,1)) for _ in range(500)]), 0, 0.015)
w2 = np.clip(np.array([0.015*(0.3+0.10*np.random.normal(0,1)) for _ in range(500)]), 0, 0.015)
w  = np.concatenate((w1, w2))
w_ = np.empty(len(w))

norm_var_y = np.random.normal(0,1)
f1 = np.array([corr_fr(np.random.normal(0,1), norm_var_y) for _ in range(500)])
norm_var_y = np.random.normal(0,1)
f2 = np.array([corr_fr(np.random.normal(0,1), norm_var_y) for _ in range(500)])
f  = np.concatenate((f1, f2))

eta =  -0.00000005

dw_idt = lambda i : eta*(f[i] - np.mean(f))*np.dot(w,f)
dwdt = lambda : sum([dw_idt(i) for i in range(len(w))])

iters = 20000

for t in range(iters):
  #count(w[0:int(len(w)/2)])
  #count(w[int(len(w)/2):len(w)])
  #print("===")
  print(t, "/", iters, end="\r")

  if t%100==0:
    plot(w, t)

  for i in range(len(w)):
    w_[i] = w[i] + dw_idt(i)
    w_[i] = 0.015 if w[i] > 0.015 else 0.0 if w[i] < 0.0 else w[i]

  w = w_

  if t%2==0:
    norm_var_y = np.random.normal(0,1)
    f1 = np.array([corr_fr(np.random.normal(0,1), norm_var_y) for _ in range(500)])
    norm_var_y = np.random.normal(0,1)
    f2 = np.array([corr_fr(np.random.normal(0,1), norm_var_y) for _ in range(500)])
    f  = np.concatenate((f1, f2))

count(w[0:int(len(w)/2)])
count(w[int(len(w)/2):len(w)])

plot(w, iters)

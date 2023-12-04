import matplotlib.pyplot as plt
import numpy as np

x0 = 1
y0 = 20
x1 = 100
y1 = 200

xvalues = np.array([])
yvalues = np.array([])

x = x0
while x <= x1:
    t = (x - x0)/(x1 - x0)
    y = y0 * (1 - t) + y1 * t
    xvalues = np.append(xvalues, int(x))
    yvalues = np.append(yvalues, int(y))
    x+=1

print(xvalues)
print(yvalues)

plt.plot(xvalues, yvalues, 'o')
plt.show()

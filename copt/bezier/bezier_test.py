import random
import sys
import numpy as np
import matplotlib.pyplot as plt
from color import Color
from bezier_matplotlib import bezier_curve

def quadraticVertex(x0,y0,x1,y1,x2,y2):
    return np.array([(x0,y0),(x1,y1),(x2,y2)])

#some color definitions
color=Color()
color_points=color.black
color_curve=color.blue
color_moving=color.red

fig, ax = plt.subplots(nrows=1, ncols=1, figsize=(9,6))


w=0.3
h=0.5
r=0.25
"""
r=0.25
vertex(0, -r)
quadraticVertex(r, -r, r, 0);
quadraticVertex(r, r, 0, r);
quadraticVertex(-r, r, -r, 0);
quadraticVertex(-r, -r, 0, -r);
"""
bezierPoints=[]
bezierPoints.append(quadraticVertex(0,-r, r,-r, r, 0))
bezierPoints.append(quadraticVertex(0,-r, r, r, 0, r))
bezierPoints.append(quadraticVertex(0,-r,-r, r,-r, 0))
bezierPoints.append(quadraticVertex(0,-r,-r,-r, 0,-r))
for serie in bezierPoints:
    curve=np.array(bezier_curve(serie))
    #plot points
    path = ax.scatter(serie[:, 0], serie[:, 1], s=30, color=color_points)
    #plot curve
    graph,= ax.plot(curve[:, 0], curve[:, 1], linewidth=1.5, c=color_curve)

    plt.fill(curve[:, 0], curve[:, 1])
plt.show()

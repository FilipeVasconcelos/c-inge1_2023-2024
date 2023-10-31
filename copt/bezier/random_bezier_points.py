import random
#import sys
import numpy as np
import matplotlib.pyplot as plt
from color import Color
from bezier_matplotlib import bezier_curve,MoveBezierCurve,getlimXY 

#some color definitions
color=Color()
color_points=color.black
color_curve=color.blue
color_moving=color.red

#set points
nseries=4
npoints=5
series=[]
for s in range(nseries):
    bezierPoints=[]
    for k in range(npoints):
        bezierPoints.append((random.random(),random.random()))
    series.append(np.array(bezierPoints))
fig, ax = plt.subplots(nrows=1, ncols=1, figsize=(8,6))
xlim,ylim=getlimXY(series)
ax.set_xlim(xlim)
ax.set_ylim(ylim)

#get bezier curve
moveSerie=[]
for serie in series: 
    curve=np.array(bezier_curve(serie))
    #plot points
    path = ax.scatter(serie[:, 0], serie[:, 1], s=30, color=[color_points]*len(serie))
    #plot curve
    graph,= ax.plot(curve[:, 0], curve[:, 1], linewidth=1.5, c=color_curve)
    # moving class
    moveSerie.append(MoveBezierCurve(ax, path, graph, serie,color_points=color_points,color_curve=color_curve,color_moving=color_moving))

plt.show()

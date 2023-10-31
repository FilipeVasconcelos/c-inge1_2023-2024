import random
import sys
import numpy as np
import matplotlib.pyplot as plt
from color import Color
from bezier_matplotlib import bezier_curve,MoveBezierCurve,getlimXY 

#some color definitions
color=Color()
color_points=color.black
color_curve=color.blue
color_moving=color.red

nnseries=[3,4]
nnpoints=[2,3,4,5]
fig, ax = plt.subplots(nrows=len(nnseries), ncols=len(nnpoints), figsize=(9,6))
moveSerie=[]
for i,nseries in enumerate(nnseries):
    for j,npoints in enumerate(nnpoints):
        param=1
        series=[]
        freq=2*np.pi/nseries
        for s in range(nseries):
            bezierPoints=[]
            for k in range(npoints):
                bezierPoints.append((np.cos((param*s+k)%nseries*freq),np.sin((param*s+k)%nseries*freq)))
            #random.shuffle(bezierPoints)
            series.append(np.array(bezierPoints))
            #random.shuffle(series)
        ax[i,j].set_xlim(-2,2)
        ax[i,j].set_ylim(-2,2)
        ax[i,j].set_aspect('equal')
        ax[i,j].set_xticks([-1, 0, 1])
        ax[i,j].set_yticks([-1, 0, 1])
        ax[i,j].set_axis_off()
        ax[i,j].title.set_text(str(nseries)+'x'+str(npoints))

        #get bezier curve
        for serie in series: 
            curve=np.array(bezier_curve(serie))
            #plot points
            path = ax[i,j].scatter(serie[:, 0], serie[:, 1], s=30, color=[color_points]*len(serie))
            #plot curve
            graph,= ax[i,j].plot(curve[:, 0], curve[:, 1], linewidth=1.5, c=color_curve)
#            # moving class
#            moveSerie.append(MoveBezierCurve(ax[i,j], path, graph, serie,color_points=color_points,color_curve=color_curve,color_moving=color_moving))

plt.show()

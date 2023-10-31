import sys
import numpy as np
import matplotlib.pyplot as plt
from color import Color

# ------------------------------------------------------------------------------
# Linear interpolation entre deux valeurs 
# a et b paramétrée par t. 
# Si t=0 lerp(a,b,t) = a
# si t=1 lerp(a,b,t) = b
# ------------------------------------------------------------------------------
def lerp(a,b,t):
  return (1 - t) * a + t * b;
# ------------------------------------------------------------------------------
# Lerp entre deux points p et q (un point est un tuple de deux valeurs "x et y" )
# ------------------------------------------------------------------------------
def Plerp(p,q,t):
    return ( lerp(p[0],q[0],t), lerp(p[1],q[1],t) )
# ------------------------------------------------------------------------------
# Courbe de bezier d'une liste pts de n points
# Pour n=2 :
#       Lerp des points pts[0]et pts[1]<=> Plerp( pts[0], pts[1], t ) 
# Pour n=3 :
#       PLerp de PLerp(pts[0],pts[1]) et PLerp(pts[1],pts[0])
#       Plerp(du bezier des deux premiers et du bezier des deux derniers)
# Pour n :
#       Plerp ( du bezier des n-1 premiers et bezier des n-1 derniers )
#       PLerp ( bezier(pts[:-1],t ) , Bezier(pts[1:] , t )
# ------------------------------------------------------------------------------
def bezier(pts,t) :
    if len(pts) == 2 :
        return Plerp(pts[0], pts[1], t) 
    else :
        return Plerp(bezier(pts[:-1],t), bezier(pts[1:],t)  , t)
# ------------------------------------------------------------------------------
# bezier_curve génère un nombre npts de points de la courbe de bezier pour 
# une liste de points pts
# ------------------------------------------------------------------------------
def bezier_curve(pts,npts_curve=512):
    return [bezier(pts,t) for t in np.linspace (0, 1.0,npts_curve) ]
# ------------------------------------------------------------------------------
# Cette classe permet de gérer les évenements souris sur les figures matpltlib
# adapté de https://stackoverflow.com/a/61457441
# ------------------------------------------------------------------------------
class MoveBezierCurve:
    def __init__(self, ax, path, graph, points,color_points,color_curve,color_moving,filled=[]):
        self.ax = ax
        self.figcanvas = self.ax.figure.canvas
        self.graph = graph
        self.path = path
        self.points = points
        self.moved = points 
        self.moving = [False for pt in self.points]
        self.pressed = False
        self.start = False
        self.ind = None
        self.color_curve=color_curve
        self.color_points=color_points
        self.color_moving=color_moving
        self.filled=filled

        self.figcanvas.mpl_connect('button_press_event', self.mouse_press)
        self.figcanvas.mpl_connect('button_release_event', self.mouse_release)
        self.figcanvas.mpl_connect('motion_notify_event', self.mouse_move)

    def mouse_release(self, event):
        if self.ax.get_navigate_mode()!= None: return
        if not event.inaxes: return
        if event.inaxes != self.ax: return
        if event.inaxes!=self.path.axes: return
        if self.pressed: 
            self.ind= None
            self.pressed = False
            self.start = False
            self.moving = [False for pt in self.points]
            self.points = self.moved
            self.graph.set_color(self.color_curve)
            self.path.set_facecolors(self.color_points)
            self.path.set_edgecolors(self.color_points)
            self.figcanvas.draw()
            return

    def mouse_press(self, event):
        if self.ax.get_navigate_mode()!= None: return
        if not event.inaxes: return
        if event.inaxes != self.ax: return
        if self.start: return
        self.moving= np.where(np.linalg.norm(self.points- np.array([(event.xdata,event.ydata)])  ,axis=1) < 0.05,True,False)
        self.ind=[i for i, x in enumerate(self.moving) if x]
        self.pressed = True
        #for f in self.filled:
        #    f.remove()

    def mouse_move(self, event):
        if not self.ind : return 
        if self.ax.get_navigate_mode()!= None: return
        if not event.inaxes: return
        if event.inaxes != self.ax: return
        if not self.pressed: return
        self.start = True

        self.moved=[]
        for k,move in enumerate(self.moving):
            if move :
                self.moved.append((event.xdata,event.ydata))
            else:
                self.moved.append(self.points[k])
        self.moved=np.array(self.moved)
        ptx,pty=self.moved[:, 0], self.moved[:, 1]

        tmpcolor=[self.color_points]*len(self.points)
        tmpcolor[self.ind[0]]=self.color_moving

        self.path.remove()
        self.path = self.ax.scatter(ptx, pty, s=20, color=tmpcolor)
        curve=np.array(bezier_curve(self.moved))
        self.graph.remove()
        self.graph, = self.ax.plot(curve[:, 0], curve[:, 1], linewidth=1.5,color=self.color_moving)
        for f in self.filled:
            f.remove()
        self.filled=self.ax.fill(curve[:, 0], curve[:, 1],self.color_moving,alpha=0.25)
        self.figcanvas.draw()

# ------------------------------------------------------------------------------
# Tracer une courbe de Bézier définie par une liste de points
# points sur des axis matplotlib ax
# ------------------------------------------------------------------------------
def plotBezierCurve(points,fig,ax,color_points='k',color_curve='blue',color_moving='red'):
    curve=np.array(bezier_curve(points))
    ptx,pty=points[:, 0], points[:, 1]
    path = ax.scatter(ptx, pty, s=20, color=[color_points]*len(points))
    graph,= ax.plot(curve[:, 0], curve[:, 1], linewidth=1.5, c=color_curve)
    filled=ax.fill(curve[:, 0], curve[:, 1],color=color_curve,alpha=0.25)
    return MoveBezierCurve(ax, path, graph, points,color_points=color_points,color_curve=color_curve,color_moving=color_moving,filled=filled)

# ------------------------------------------------------------------------------
# récupéer les limites xlim et ylim d'un ensemble de points de beziers
# ------------------------------------------------------------------------------
def getlimXY(setPoints):
    offpct=0.45
    minx,maxx=1e100,-1e100
    miny,maxy=1e100,-1e100
    for points in setPoints:
        minx=min(minx,min(points[:,0]))
        maxx=max(maxx,max(points[:,0]))
        miny=min(miny,min(points[:,1]))
        maxy=max(maxy,max(points[:,1]))
    offx,offy=(maxx-minx)*offpct,(maxy-miny)*offpct
    return (minx-offx,maxx+offx),(miny-offy,maxy+offy)

if __name__ == "__main__" :

    # ----------------------------------------------------------------------
    # Approximation du cercle par des courbes de Bezier
    # ----------------------------------------------------------------------
    a=1.00005519
    b=0.55342686
    c=0.99873585
    bezierCircle = [ 
            np.array([(0,a),(b,c),(c,b),(a,0)])     ,
            np.array([(a,0),(c,-b),(b,-c),(0,-a)])  ,
            np.array([(0,-a),(-b,-c),(-c,-b),(-a,0)]) ,
            np.array([(-a,0),(-c,b),(-b,c),(0,a)])  ]
    color=Color()
    fig, ax = plt.subplots(nrows=1, ncols=1, figsize=(8,6))
    ax.set_title('Courbes de Bézier')
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_aspect('equal')
    xlim,ylim = getlimXY(bezierCircle)
    ax.set_xlim(xlim)
    ax.set_ylim(ylim)

    movepts=[ plotBezierCurve(points,fig,ax,color_points=color.blue,color_curve=color.blue,color_moving=color.red) for points in bezierCircle ]
    #movepts=[]
    for points in bezierCircle :
        movepts.append(plotBezierCurve(points,fig,ax,color_points=color.blue,color_curve=color.blue,color_moving=color.red))
    plt.show()

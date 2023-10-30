import sys
import numpy as np
import matplotlib.pyplot as plt
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
    def __init__(self, ax, path, graph, points):
        self.ax = ax
        self.figcanvas = self.ax.figure.canvas
        self.graph = graph
        self.path = path
        self.points = points
        self.moved = None
        self.moving = [False for pt in points]
        self.pressed = False
        self.start = False
        self.color=graph.get_color()

        self.figcanvas.mpl_connect('button_press_event', self.mouse_press)
        self.figcanvas.mpl_connect('button_release_event', self.mouse_release)
        self.figcanvas.mpl_connect('motion_notify_event', self.mouse_move)

    def mouse_release(self, event):
        if self.ax.get_navigate_mode()!= None: return
        if not event.inaxes: return
        if event.inaxes != self.ax: return
        if self.pressed: 
            self.pressed = False
            self.start = False
            self.moving = [False for pt in points]
            self.points = self.moved
            self.graph.set_color(self.color)
            self.path.set_color('k')
            self.figcanvas.draw()
            print(self,self.points)
            return

    def mouse_press(self, event):
        if self.ax.get_navigate_mode()!= None: return
        if not event.inaxes: return
        if event.inaxes != self.ax: return
        if self.start: return
        self.moving= np.where(np.linalg.norm(self.points- np.array([(event.xdata,event.ydata)])  ,axis=1) < 0.05,True,False)
        self.pressed = True

    def mouse_move(self, event):
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
        self.path.remove()
        self.path = self.ax.scatter(ptx, pty, s=20, c="r") 
        curve=np.array(bezier_curve(self.moved))
        self.graph.remove()
        self.graph, = self.ax.plot(curve[:, 0], curve[:, 1], linewidth=1.5,c='r')
        self.figcanvas.draw()

# ------------------------------------------------------------------------------
# Tracer une courbe de Bézier définie par une liste de points
# points sur des axis matplotlib ax
# ------------------------------------------------------------------------------
def plotBezierCurve(points,fig,ax,color='r'):

    curve=np.array(bezier_curve(points))
    ptx,pty=points[:, 0], points[:, 1]
    
    path = ax.scatter(ptx, pty, s=20, c="k")
    graph,= ax.plot(curve[:, 0], curve[:, 1], linewidth=1.5, c=color)
    
    return MoveBezierCurve(ax, path, graph, points)

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
# ------------------------------------------------------------------------------
# retourne une couleur en hex au format #FF00FF à partir d'un tuple de valeurs RGB
# ------------------------------------------------------------------------------
def rgbtohex(color):
    return '#'+''.join([hex(v)[-2:] for v in color])

#Approximation du Cercle par des courbes de Bezier
a=1.00005519
b=0.55342686
c=0.99873585
bezierCircle = [ 
        np.array([(0,a),(b,c),(c,b),(a,0)])     ,
        np.array([(a,0),(c,-b),(b,-c),(0,-a)])  ,
        np.array([(0,-a),(-b,-c),(-c,-b),(-a,0)]) ,
        np.array([(-a,0),(-c,b),(-b,c),(0,a)])  ]

col1=(57,106,177)
col2=(218,124,48)
col3=(62,150,81)
col4=(204,37,41)
colorRGB=[col1,col2,col3,col4]
colors=[rgbtohex(color) for color in colorRGB]

fig, ax = plt.subplots(nrows=1, ncols=1, figsize=(8,6))
xlim,ylim = getlimXY(bezierCircle)
ax.set_xlim(xlim)
ax.set_ylim(ylim)
movepts=[]
for points in bezierCircle:
    movepts.append(plotBezierCurve(points,fig,ax,color=colors[0]))
    print(movepts[-1],movepts[-1].points[0])
ax.set_aspect('equal')
ax.add_artist( plt.Circle(( 0.0 , 0.0 ), 1.0 , color='#0f0f0f15') )
plt.show()

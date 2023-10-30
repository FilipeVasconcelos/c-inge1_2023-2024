import numpy as np
# --------------------------------------------------------------------------
# Linear interpolation entre deux valeurs 
# a et b paramétrée par t. 
# Si t=0 lerp(a,b,t) = a
# si t=1 lerp(a,b,t) = b
# --------------------------------------------------------------------------
def lerp(a,b,t):
  return (1 - t) * a + t * b;
# --------------------------------------------------------------------------
# Lerp entre deux points p et q (un point est un tuple de deux valeurs "x et y" )
# --------------------------------------------------------------------------
def Plerp(p,q,t):
    return ( lerp(p[0],q[0],t), lerp(p[1],q[1],t) )
# --------------------------------------------------------------------------
# Courbe de bezier d'une liste pts de n points
# Pour n=2 :
#       Lerp des points pts[0]et pts[1]<=> Plerp( pts[0], pts[1], t ) 
# Pour n=3 :
#       PLerp de PLerp(pts[0],pts[1]) et PLerp(pts[1],pts[0])
#       Plerp(du bezier des deux premiers et du bezier des deux derniers)
# Pour n :
#       Plerp ( du bezier des n-1 premiers et bezier des n-1 derniers )
#       PLerp ( bezier(pts[:-1],t ) , Bezier(pts[1:] , t )
# --------------------------------------------------------------------------
def bezier(pts,t) :
    if len(pts) == 2 :
        return Plerp(pts[0],pts[1],t) 
    else :
        return Plerp(bezier(pts[ :-1],t), 
                     bezier(pts[1:  ],t),t)

# --------------------------------------------------------------------------
# bezier_curve génère un nombre npts de points de la courbe de bezier pour 
# une liste de points pts
# --------------------------------------------------------------------------
def bezier_curve(pts,npts_curve=64):
    return [bezier(pts,t) for t in np.arange (0, 1.0, 1/npts_curve) ]

points=[(0,0),(0,1),(1,0),(1,1)]
curve=bezier_curve(points)
for pt in curve:
    print(f"{pt[0]:<20.12e}{pt[1]:<19.12e}")

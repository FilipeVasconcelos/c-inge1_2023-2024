import math as m
from mpmath import *
import numpy as np

B=10
LB=1  
NbDigits=1000
size=1+NbDigits//LB

# double precision calculation
def expo(tol):
    u,v,k=1,1,1
    while v>tol:
        v=v/k
        u=u+v
        k+=1
    return u

def mpexpo(size,e,u):
    k=1
    while not isZero(size, u) : 
        div(size,u,k)
        add(size,e,u)
        k+=1
    print("number of iterations",k)

def isZero(n,x):
    for i in range(n):
        if (x[i]!=0) : return False
    return True

def add(n,x,y):
    carry=0
    for i in range(n-1,-1,-1):
        x[i]+=y[i]+carry
        if x[i]<B :
            carry=0
        else:
            carry=1
            x[i]-=B

def div(n,x,d):
    carry=0
    for i in range(n):
        xi= x[i]+carry*B;
        q=xi//d
        carry=xi-q*d
        x[i]=q

def Pprint(n,x):
    print(f"test   : {x[0]}.",end="")
    for k,ek in enumerate(e[1:n]):
        print(f"{ek:0>{LB}}",end='')
    print()


print(f"size: {size}")
e=np.zeros(size,dtype=np.int64)
u=np.zeros(size,dtype=np.int64)
e[0],u[0]=1,1
mpexpo(size,e,u)
Pprint(50,e)
mp.dps=100
print(f"mpmath : {exp(1)}")
print(f"double : {expo(1.0E-100):2.50f}")

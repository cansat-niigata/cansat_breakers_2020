import numpy

data = numpy.loadtxt("MagData.csv", delimiter=',')
x = data[:,[0]]
y = data[:,[1]]
z = data[:,[2]] 
n = len(x)    
x2 = numpy.power(x,2)
y2 = numpy.power(y,2)
z2 = numpy.power(z,2)
xy = x*y
xz = x*z
yz = y*z
E = -x*(x2+y2+z2)
F = -y*(x2+y2+z2)
G = -z*(x2+y2+z2)
H =   -(x2+y2+z2)
x = numpy.sum(x)
y = numpy.sum(y)
z = numpy.sum(z)
x2 = numpy.sum(x2)
y2 = numpy.sum(y2)
z2 = numpy.sum(z2)
xy = numpy.sum(xy)
xz = numpy.sum(xz)
yz = numpy.sum(yz)    
E = numpy.sum(E)
F = numpy.sum(F)
G = numpy.sum(G)
H = numpy.sum(H)
K = numpy.array([  
				[x2,xy,xz,x],
				[xy,y2,yz,y],
				[xz,yz,z2,z],
				[x,y,z,n]
				])
L = numpy.array([E,F,G,H])
P = numpy.dot(numpy.linalg.inv(K),L)
A = P[0]
B = P[1]
C = P[2]
D = P[3]
x0 = (-1/2)* A
y0 = (-1/2)* B
z0 = (-1/2)* C
r  = pow(pow(x0,2)+pow(y0,2)+pow(z0,2)-D,1/2)

print(x0)
print(y0)
print(z0)
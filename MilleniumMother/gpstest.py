import numpy

#>>> point1 = coordinate(37.8708150,138.9261736)
#>>> point2 = coordinate(37.8725758,138.9305010)
#>>> point2.getDistanceFrom(point1)

class coordinate:
	timestamp = [0,0,0]
	latitude = 0
	longitude = 0
	def __init__(self,lat,lng,timestamp=None):
		self.latitude = lat
		self.longitude = lng
		if timestamp != None:
			self.timestamp = timestamp
	

	def toString(self):
		h = self.timestamp[0] if self.timestamp[0] < 24 else self.timestamp[0] - 24
		time_str = ('%02d:%02d:%02d' %(h, self.timestamp[1], self.timestamp[2]))
		return time_str + ':' + str(self.latitude) +','+ str(self.longitude) 

	def getDistanceFrom(self,another):
		lat0 = another.latitude
		lng0 = another.longitude
		lat1 = self.latitude
		lng1 = self.longitude
		#差異が無ければ0
		if numpy.isclose(lat0,lat1) and numpy.isclose(lng0,lng1):
			return {
				'distance':0,
				'courseS2G':0,
				'courseG2S':0
			}
	
		#WGS84
		a = 6378137.0#赤道半径
		f = 1/298.257223563#扁平率
		#短軸半径
		b = (1-f)*a
		#ラジアンへ変換
		rlat0 = numpy.deg2rad(lat0)
		rlng0 = numpy.deg2rad(lng0)
		rlat1 = numpy.deg2rad(lat1)
		rlng1 = numpy.deg2rad(lng1)
		#更成緯度
		U0 = numpy.arctan((1-f)*numpy.tan(rlat0))
		U1 = numpy.arctan((1-f)*numpy.tan(rlat1))
		#2点間の経度差  
		L = rlng1-rlng0
		#補助球上の経度(初期化)
		Lambda=L

		sinU0 = numpy.sin(U0)
		sinU1 = numpy.sin(U1)
		cosU0 = numpy.cos(U0)
		cosU1 = numpy.cos(U1)
	
		for _ in range(1000):
			sinLmd=numpy.sin(Lambda)
			cosLmd=numpy.cos(Lambda)
		
			sins=((cosU1*sinLmd)**2+(cosU0*sinU1-sinU0*cosU1*cosLmd)**2)**(1/2)
			coss=sinU0*sinU1+cosU0*cosU1*cosLmd

			sigma=numpy.arctan2(sins,coss)

			sina=cosU0*cosU1*sinLmd/sins
			cos2a=1-sina**2

			cos2dm=coss-(2*sinU0*sinU1/cos2a)

			C=f*(cos2a)*(4+f*(4-3*cos2a))/16
			Lambda_prev=Lambda
			Lambda=L+(1-C)*f*sina*(sigma+C*sins*(cos2dm+C*coss*(-1+2*coss**2)))
			#精度の指定
			if abs(Lambda-Lambda_prev)<=1e-12:
				break
	
		u2=cos2a*(a**2-b**2)/b**2
		A=1+u2*(4096+u2*(-768+u2*(320-175*u2)))/16384
		B=u2*(256+u2*(-128+u2*(74-47*u2)))/1024
	
		deltas=B*sins*(cos2dm+B*(coss*(-1+2*cos2dm**2)-B*cos2dm*(-3+4*sins**2)*(-3+4*cos2dm**2)/6)/4)

		distance=b*A*(sigma-deltas)
		course0to1=numpy.rad2deg(numpy.arctan2(cosU1*sinLmd,cosU0*sinU1-sinU0*cosU1*cosLmd))
		course1to0=numpy.rad2deg(numpy.arctan2(cosU0*sinLmd,-sinU0*cosU1+cosU0*sinU1*cosLmd) + numpy.pi)

		if course1to0 > 180:
			course1to0 -= 360
		elif course1to0 < -180:
			course1to0 += 360

		return {
			'distance':distance,
			'courseS2G':course0to1,
			'courseG2S':course1to0
		}

import DRV_GPIO

class elevetor:
	table_dict = {                          #データ変換テーブル
			'00':{'00':0,'01':1,'10':-1,'11':-2},#11はエラー
			'01':{'00':-1,'01':0,'10':-2,'11':1},#10はエラー
			'10':{'00':1,'01':-2,'10':0,'11':-1},#01はエラー
			'11':{'00':-2,'01':-1,'10':1,'11':0} #00はエラー
		}
	previos_data = '00'#前回の値
	value = 0
	deg = 0

	def __init__(self,MA=20,MB=21,EA=12,EB=16):
		self.gpio = DRV_GPIO.Gpio()
		self.ma = MA
		self.mb = MB
		self.ea = EA
		self.eb = EB
		self.gpio.setPinOut(self.ma)
		self.gpio.setPinOut(self.mb)
		self.gpio.setInterrupt(self.ea,self.recData)
		self.gpio.setInterrupt(self.eb,self.recData)

	def roll(self,speed=255,invert=False):
		if invert == False:
			self.gpio.setPWM(self.ma,dutycycle=speed)
			self.gpio.toggleOff(self.mb)
		else:
			self.gpio.setPWM(self.mb,dutycycle=speed)
			self.gpio.toggleOff(self.ma)
		print(self.deg)

	def stop(self):
		self.gpio.toggleOff(self.ma)
		self.gpio.toggleOff(self.mb)

	def recData(self,gpio,level,tick):#本当はもっと軽い処理にしたいのでこの辺りは改良の余地ありか？
		follow_data = '00'#初期化
		if gpio == self.ea:#どちらのピンに割り込みが入ったかで分岐
			follow_data = '{0}{1}'.format('1',self.previos_data[1])
		elif gpio == self.eb:
			follow_data = '{0}{1}'.format(self.previos_data[0],'1')
		self.value += self.table_dict[self.previos_data][follow_data]#取得値を変換して足す
		if self.value == -2:
			self.value = 0
		self.previos_data = follow_data#値を保存
		self.deg = 12*self.value#度数法に変換
		return self.deg

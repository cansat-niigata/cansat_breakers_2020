import cv2
import numpy

class detecter:
    margin_x = 50
    margin_y = 50
    def __init__(self,cascadepath='./cascade_traffic_cone.xml'):
        self.cascade = cv2.CascadeClassifier(cascadepath)

    def readImage(self,filename,x=1024,y=768):
        self.filename = filename
        self.img = cv2.imread(filename)
        self.img_orig = cv2.imread(filename)
        if (x!=None)and(y!=None):
            self.img = cv2.resize(self.img,(x,y))
            self.img_orig = cv2.resize(self.img,(x,y))
        
        return self

    def outImage(self,outname):
        cv2.imwrite(outname,self.img)
        return self

    def setMargin(self,x,y):
        self.margin_x = x
        self.margin_y = y
        return self

    def inrangeHSV(self,color='red'):
        if color == 'red':
            hsv_low1 = numpy.array([0,80,40])
            hsv_up1 = numpy.array([25,255,255])
            hsv_low2 = numpy.array([150,64,0])
            hsv_up2 = numpy.array([270,255,255])
        else:
            hsv_low1 = numpy.array([0,64,0])
            hsv_up1 = numpy.array([30,255,255])
            hsv_low2 = numpy.array([150,64,0])
            hsv_up2 = numpy.array([270,255,255])

        img_hsv = cv2.cvtColor(self.img,cv2.COLOR_BGR2HSV)
        img_mask = numpy.zeros((self.img.shape[0],self.img.shape[1],self.img.shape[2]),numpy.uint8)
        mask1 = cv2.inRange(img_hsv,hsv_low1,hsv_up1)
        mask2 = cv2.inRange(img_hsv,hsv_low2,hsv_up2)
        del img_hsv

        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(3,3))
        mask1 = cv2.erode(mask1,kernel,iterations=2)
        mask2 = cv2.erode(mask2,kernel,iterations=2)
        mask1 = cv2.dilate(mask1,kernel,iterations=2)
        mask2 = cv2.dilate(mask2,kernel,iterations=2)
        mask1 = mask1 + mask2

        self.conts = cv2.findContours(mask1,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)[0]
        for cnts in self.conts:
            (x,y,w,h) = cv2.boundingRect(cnts)
            if w*h < 300:
                continue
            cv2.rectangle(self.img,(x-self.margin_x,y-self.margin_y),(x+w+self.margin_x,y+h+self.margin_y),(0,255,0),2)
            cv2.rectangle(img_mask,(x-self.margin_x,y-self.margin_y),(x+w+self.margin_x,y+h+self.margin_y),(255,255,255),-1)

        self.img_masked = cv2.bitwise_and(self.img_orig,img_mask)
        cv2.imwrite('masked_'+self.filename,self.img_masked)
        return self

    def detectbyCascade(self,img=None):
        if img == None:
            img_masked_gray = cv2.cvtColor(self.img_masked,cv2.COLOR_BGR2GRAY)
            targets = self.cascade.detectMultiScale(img_masked_gray)
        else:
            img_gray = cv2.cvtColor(self.img,cv2.COLOR_BGR2GRAY)
            targets = self.cascade.detectMultiScale(img_gray)
        
        average = 0
        for (tx,ty,tw,th) in targets:
            cv2.rectangle(self.img,(tx,ty),(tx+tw,ty+th),(0,0,255),thickness=2)
            average += tx+tw/2
            if len(targets) == 0:
                return False
            average/(len(targets)*self.img.shape[1])
        cv2.imwrite('detected_'+self.filename,self.img)
        return average

    def detectbyCascadev2(self):
        self.inrangeHSV()
        img_gray = cv2.cvtColor(self.img_orig,cv2.COLOR_BGR2GRAY)
        for cnts in self.conts:
            (x,y,w,h) = cv2.boundingRect(cnts)
            if w*h < 300:
                continue
            print(x,y,w,h)
            img_masked_gray = img_gray[y-self.margin_y:y+h+self.margin_y,x-self.margin_x:x+w+self.margin_x]
            targets = self.cascade.detectMultiScale(img_masked_gray)
            for (tx,ty,tw,th) in targets:
                print(tx,ty,tw,th)
                cv2.rectangle(self.img,(x-self.margin_x+tx,y-self.margin_y+ty),(x-self.margin_x+tx+tw,y-self.margin_y+ty+th),(0,0,255),thickness=1)
        cv2.imwrite('detected_'+self.filename,self.img)




            
import picamera
import cv2
import time
import numpy
#from DRV_GPS_Serial import Gps_Serial

class Camera:
    serFlag = False
    def __init__(self,Mx=1024,My=1024,roll=180,path='./img/image_',opath='./img/image_',format_='.ong',cascadepath='cascade_traffic_cone.xml',target_height=0.8):
        self.Mx = Mx
        self.My = My
        self.camera = picamera.PiCamera()
        self.camera.resolution = (Mx,My)
        self.camera.rotation = roll
        self.num = 0
        self.path = path
        self.opath = opath
        self.format = format_
        self.filename = self.path+str(self.num)+self.format
        self.outfilename = self.opath+str(self.num)+self.format
        self.cascade = cv2.CascadeClassifier(cascadepath)
        self.height = target_height

    def getImage(self):
        self.num = self.num+1
        self.filename = self.path+str(self.num)+self.format
        self.outfilename = self.opath+str(self.num)+self.format
        self.camera.capture(self.filename)
        time.sleep(0.2)
        return True

    def detectTarget(self,filename=None):
        if filename != None:
            self.filename = filename
        img = cv2.imread(self.filename,cv2.COLOR_BGR2GRAY)
        target = self.cascade.detectMultiScale(img)
        length = len(target)
        if length == 0:
            return False
        average = 0
        for rect in target:
            self.left_top = tuple(rect[0:2])
            self.right_bottom = tuple(rect[0:2]+rect[2:4])
            cv2.rectangle(img,self.left_top,self.right_bottom,(0,0,255),thickness=2)
            average += (self.left_top[0]+self.right_bottom[0])/2
        average /= len(target)
        self.average = average
        cv2.imwrite(self.outfilename,img)
        return average

    def detectTargetv2(self,filename=None):
        if filename != None:
            self.filename = filename

        margin_x = 80
        margin_y = 80

        hsv_low1 = numpy.array([0,64,0])
        hsv_up1 = numpy.array([30,255,255])
        hsv_low2 = numpy.array([150,64,0])
        hsv_up2 = numpy.array([270,255,255])

        img = cv2.imread(self.filename,cv2.COLOR_BGR2HSV)
        img_hsv = cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
        cv2.imwrite(self.path+str(self.num)+'_hsv'+self.format,img_hsv)
        img_mask = numpy.zeros((img.shape[0],img.shape[1],img.shape[2]),numpy.uint8)

        mask1 = cv2.inRange(img_hsv,hsv_low1,hsv_up1)
        cv2.imwrite(self.path+str(self.num)+'_mask1'+self.format,mask1)
        mask2 = cv2.inRange(img_hsv,hsv_low2,hsv_up2)
        cv2.imwrite(self.path+str(self.num)+'_mask2'+self.format,mask2)
        del img_hsv

        kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(3,3))
        mask1 = cv2.erode(mask1,kernel,iterations=2)
        mask2 = cv2.erode(mask2,kernel,iterations=2)
        mask1 = cv2.dilate(mask1,kernel,iterations=2)
        mask2 = cv2.dilate(mask2,kernel,iterations=2)

        mask1 = mask1 + mask2

        conts1 = cv2.findContours(mask1,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)[0]
        #conts2 = cv2.findContours(mask2,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)[0]
        del mask1
        del mask2

        for cnts in conts1:
            (x,y,w,h) = cv2.boundingRect(cnts)
            if w*h < 250:
                continue
            #cv2.rectangle(img,(x-margin_x,y-margin_y),(x+w+margin_x,y+h+margin_y),(0,255,0),2)
            cv2.rectangle(img_mask,(x-margin_x,y-margin_y),(x+w+margin_x,y+h+margin_y),(255,255,255),-1)
        #for cnts in conts2:
        #    (x,y,w,h) = cv2.boundingRect(cnts)
        #    if w*h < 250:
        #        continue
        #    #cv2.rectangle(img,(x-margin_x,y-margin_y),(x+w+margin_x,y+h+margin_y),(0,255,0),2)
        #    cv2.rectangle(img_mask,(x-margin_x,y-margin_y),(x+w+margin_x,y+h+margin_y),(255,255,255),-1)

        img_masked = cv2.bitwise_and(img,img_mask)
        cv2.imwrite(self.path+str(self.num)+'_masked'+self.format,img_masked)
        img_masked_gray = cv2.cvtColor(img_masked,cv2.COLOR_BGR2GRAY)
        del img_mask

        targets = self.cascade.detectMultiScale(img_masked_gray)
        average = 0
        for (x,y,w,h) in targets:
            cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255),thickness=3)
            average += x+w/2
        if len(targets) == 0:
            return False
        average/len(targets)
        self.average = average
        cv2.imwrite(self.outfilename,img)
        return average

    #def getRange(self):
    #    top_bottom = self.left_top[1]-self.right_bottom[1]
        
    def judge(self):
        if self.average>self.Mx/4:
            if self.average<self.Mx*3/4:
                return True
            return False
        else:
            return False

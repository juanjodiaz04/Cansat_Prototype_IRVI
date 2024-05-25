

from picamera import PiCamera
from time import sleep
import time
from fractions import Fraction
import os
from os import path
    
def get_folder_name(): 
        date  = time.strftime('%d%b') 
        hour = time.strftime('%M-%S')
        aux_name = date+"_"+hour
        fileName =aux_name.replace(" ", "")
        return fileName

def take_pictures():  
    camera = PiCamera()
    redAWB=1.0
    blueAWB=1.0
    customGains = (redAWB, blueAWB)
    camera.awb_mode = "off"
    camera.awb_gains = customGains
    #    camera.hflip = True
    #    camera.vflip = True   
    camera.resolution = (1920,1080)
        
       # camera.framerate = Fraction(1,6)
       # camera.shutter_speed = 6000000
        #camera.exposure_mode = 'off'

       # camera.iso = 800
       
       #time
    

    Name_0 = get_folder_name()
    Name = str(Name_0)
    Path_0 = '/home/pi/Desktop/Images Test/' + Name
    os.mkdir(Path_0)
    Path_Name = path.abspath(Path_0)
    Path = str(Path_Name)

    camera.start_preview()
    for i in range(6):
        sleep(2)
        redAWB=redAWB + 0.2
        blueAWB=blueAWB -0.05
        customGains = (redAWB, blueAWB)
        camera.awb_mode = "off"
        camera.awb_gains = customGains
        camera.capture(Path + '/Image%s.jpg' % i)
        
    camera.stop_preview()

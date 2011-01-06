#!/usr/bin/python
from hardware_agilent import *
from math import *
#wg = WG33220A_serial("/dev/ttyUSB1")
wg = agilent_com_serial("/dev/ttyUSB0")
wg.shape(SQUARE);
wg.frequency(2000)



#points = 100;
#freq = 3.;
#a = floatArray(points)
#i = 0; 
#wg.burst_int(1e4);
#for i in xrange(points):  
#    a[i] = (sin(i*2*freq*pi/points)) +  (cos(i*1*freq*pi/points))-1

#apply_waveform_serial(wg,"TOM", 1e6*freq, a,points)

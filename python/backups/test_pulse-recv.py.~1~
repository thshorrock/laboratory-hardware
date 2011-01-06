#from hardware_coms_manager import *
from hardware_agilent import *
from math import *
wg = WG33220A("172.16.12.151");
points = 1000;
wg.send("DATA VOLATILE, .1, .2, .3, .2, .1, .2, .6, .6, .6");

wg.send("DATA:COPY TOM, VOLATILE\n");
wg.send("FUNC:USER TOM\n");
wg.send("FUNC:SHAP USER\n");

a = floatArray(points)
i = 0; 
for i in xrange(points):  
    a[i] = (sin(i*2*pi/points)+cos(i*2.2*pi/points))/4

wg.reset()
wg.arb_wave("TOM", a,points)
#wg.send("APPL:SIN 5000,5\n");
#wg.reset();
#wg.apply(SIN,3.0,3.0, 0.5);
#wg.shape(SQUARE);
#wg.burst(10);
#wg.burst(10);
#wg.burst(20);
#wg.command("APPL:SIN 5000,3");

#ip = IPmanager("172.16.12.151","5025") ;
#ip.send("APPL:SIN 6000,5\n");
#print ip.recv("SYST:ERR?\n");
#print ip.timed_recv("SYST:ERR?\n",128,  3);
#ip.send("APPL:SIN 2000,5\n");
#ip.control_connection() ;

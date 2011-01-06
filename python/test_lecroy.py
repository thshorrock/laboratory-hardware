#!/usr/bin/python
from hardware_lecroy import *
import numpy 
import Gnuplot

lc = lecroy_64Xi("10.0.0.10")
#lc.set_coupling(C1,D50)
#lc.set_coupling(C2,GND)
#lc.set_coupling(C3,A1M)
#lc.set_coupling(C4,D1M)

#lc.set_timebase(50e-6)
#lc.arm()
#lc.auto_calibrate(0)
#lc.calibrate()
#lc.auto_calibrate(1)
#lc.beep()
#lc.clear_memory(1) ##THIS FUNCTION IS UNTESTED
#lc.clear_sweeps() ##THIS FUNCTION IS NOT WORKING
#lc.clear() ##THIS FUNCTION IS NOT WORKING
#lc.set_date()
#lc.aquire_single()
#lc.message("a message from tom")
#lc.message("another message from tom")
#lc.set_vertical_offset(C2, 1)
#lc.vertical_offset_constant(1)
#lc.set_vertical_offset(C2, 0)
#lc.sequence(10,10000000)
#lc.sequence(1, 1e9)
#lc.persistence(1)
#lc.persistence(0)
#lc.stop()
#lc.reset()
#lc.store(C1, HDD)
#lc.store_setup(C1, HDD, NO_AUTOSAVE, ASCII)
##lc.toggle_trace(C3);
#lc.trace_on(C4)
#lc.trace_off(C4)
#lc.trigger_coupling(C1, DC); 
#lc.trigger_delay(20e-6); 
#lc.trigger_level(C1,0.2);
#lc.trigger_mode(AUTO);
#lc.trigger_pattern("C1,H,C2,L,C4,H,STATE,NOR")
#lc.reset()
#lc.trigger_select(EDGE,C1, PULSE_SMALLER_THAN,20e-9 )
#lc.trigger_negative_edge(C1)
#lc.trigger_positive_edge(C1)
#lc.volts_per_div(C1, 0.5)
lc.waveform_text(C2, "some text")
print "waveform text = ", lc.get_waveform_text(C2)
wf = lc.get_waveform(C2)
data = doubleArray(wf.size1())

data1 = numpy.arange(wf.size1())
#wf.get_data1(data1)

for i in range(0,wf.size1()-1):
    data1[i] =  wf[i]
gp = Gnuplot.Gnuplot(persist = 1)

#data1 = [[0, 0], [1, 1], [2, 4], [3, 9], [4, 16]] # The first data set (a quadratic)
#data2 = [[0, 0], [1, 1], [2, 2], [3, 3], [4, 4]] # The second data set (a straight line)

#plot1 = Gnuplot.PlotItems.Data(data1, with_="lines", title="Quadratic")
#plot2 = Gnuplot.PlotItems.Data(data2, with_="points 3", title=None) # No title

#gp.plot(plot1)




#lc = lecroy_com_manager("172.16.12.152")
#lc.clear()
#lc.send("C1:CPL D50\n") 
#print lc.recv("MSIZ?\n",30)
#print lc.recv("C1:VDIV?\n",30)
#print lc.recv("C1:VDIV?\n",30)
#print lc.recv("C1:VDIV?\n",30)
#from hardware_coms_manager import *
#cm = IPmanager("172.16.12.152","1861")
#cm.send("BUZZ BEEP\n");
#cm.send("BUZZ BEEP\n");
#cm.send("BUZZ BEEP\n");
#cm.send("BUZZ BEEP\n");

#ip = IPmanager("172.16.12.152","1861") 
# print ip.timed_recv("*IDN?\n",128,  3);
#lc.clear();
#print lc.serial_poll_byte()
#lc.send("BUZZ BEEP;DISPLAY OFF\n")
#lc.send("ASET \n")
#lc.send("C1:CPL D50\n") 
#lc.send("C1:gfVDIV 2 \n")
#lc.send("C1:gfVDIV 2 ")


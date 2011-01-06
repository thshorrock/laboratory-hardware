#!/usr/bin/python
#from hardware_coms_manager import *
from hardware_DPR500 import *
from math import *
import time

f = PulserReceiverFactory()
rpl2 = f.make_RPL2()
freq = rpl2.set_prf(600);
rpl2.set_internal_trigger();
rpl2.set_voltage(400);
#rpl2.set_slave()
rpl2.set_pulser_mode(damping_RPL2.thirtyfour_Ohm, energy_policy.HIGH, receive_mode.RECEIVE_ONLY)

rpu2 = f.make_RPU2()
freq = rpu2.set_prf(600);
rpu2.set_internal_trigger();
#rpu2.set_voltage(500);
#rpl2.set_slave()
rpu2.set_pulser_mode(damping_RPU2.fifty_Ohm, energy_policy.LOW)

print "prf set to ", freq 

try:
   dpr500  = DPR500("/dev/ttyUSB1")
   dpr500.blink(100)
   
   dpr500.attach_A(rpu2)
   dpr500.set_gain_A(40)
   dpr500.set_high_pass_filter_A(HPFA.one_MHz)
   dpr500.set_low_pass_filter_A(LPFA.fifteen_MHz)
   
   dpr500.attach_B(rpl2)
   dpr500.set_gain_B(-10)
   dpr500.set_high_pass_filter_B(HPFB.five_MHz)
   dpr500.set_low_pass_filter_B(LPFB.threehundred_MHz)
 
   dpr500.notify()
    
   dpr500.turn_on_A();
   dpr500.turn_on_B();
   print "IS ON"
   time.sleep(4)
   print "IS OfF"
   dpr500.turn_off_A();
   dpr500.turn_off_B();
except failed_to_contact_DPR500_device:
    print "Failed to contact DPR500. Sometimes restarting your system helps..."
#pr.blink(120);
    
#f = PulserReceiverFactory();
#p = f.make_RPL2()
#p.set_gain(30)

#pr.attach_A(p)

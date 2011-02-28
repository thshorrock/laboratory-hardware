#include "lecroy.hpp"
#include "agilent.hpp"
#include "DPR500.hpp"
#include "filename.hpp"
#include "readme.hpp"
#include "stringify.hpp"
#include "boost/progress.hpp"
#include <sstream>


#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
using namespace ICR;
using namespace ICR::lecroy;
using namespace ICR::pulser;
using namespace ICR::agilent;




int
main  (int ac, char **av)
{
  //lecroy_64Xi            scope("10.0.0.10");
  WG33120A               bus_sync("/dev/ttyUSB0");

  WG33220A<agilent::IP>  driving_pulse("10.0.0.20");
  DPR500                 pulser("/dev/ttyUSB1");
  PulserReceiverFactory  factory; 
  

  
  RPL2* imaging_pulse = factory.make_RPL2();

  //IMAGING PULSE
  pulser.blink(200);  
  imaging_pulse->set_external_trigger();
  imaging_pulse->set_pulser_mode(damping_RPL2::fourtyfour_Ohm,
				 energy_policy::HIGH,
				 receive_mode::PULSE_ECHO);
  imaging_pulse->set_voltage(400);
  pulser.attach_A(imaging_pulse);
  std::cout<<"NOTIFY"<<std::endl;

  pulser.notify();
  //)  filter::HPFA::   ten_kHz    sevenpointfive_MHz
  pulser.set_high_pass_filter_A(filter::HPFA::sevenpointfive_MHz);
  pulser.set_low_pass_filter_A(filter::LPFA::fifty_MHz);
  pulser.set_gain_A(50);
  
  pulser.notify();

  //driving_pulse.turn_off();
  //driving_pulse.turn_on();
  pulse_after_n_microseconds(&driving_pulse, 30);
  //sin_after_n_microseconds(&driving_pulse, 0.5, 2.0);
  //BUS
  bus_sync.shape(shape::SQUARE);
  bus_sync.trigger(trigger::BUS);
  bus_sync.burst_on();
 

 
  //LECROY SETUP
  // scope.set_date();

  // scope.trigger_mode(trigger_mode::NORM);
  // scope.trace_on (location::C1);
  // scope.trace_on(location::C2);
  // scope.trace_off(location::C3);
  // scope.trace_off(location::C4);
  // scope.set_coupling(location::C1, coupling::D50);
  // scope.set_coupling(location::C2, coupling::D50);
  // scope.set_coupling(location::C3, coupling::D50);
  // scope.set_coupling(location::C4, coupling::D50);

  // scope.trigger_positive_edge(location::C1);
  // scope.trigger_level(location::C1, 1.0);
  // scope.trigger_select(trigger_type::EDGE,location::C1,trigger_hold_type::NO_HOLD);
  // scope.trigger_delay(-40e-6);
  // scope.set_timebase(10e-6);
  // scope.volts_per_div(location::C1,1.0);
  

  pulser.turn_on_A();
  std::cout<<"ON"<<std::endl;


  for(size_t i=0;i<1000;++i){
    bus_sync.trigger_now();
  }
    
  pulser.turn_off_A();  //make sure the pulser perminantely running
  std::cout<<"OFF"<<std::endl;
  
}

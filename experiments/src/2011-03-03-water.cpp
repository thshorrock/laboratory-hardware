#include "analogic.hpp"
#include "stringify.hpp"
#include "agilent.hpp"
#include "lecroy.hpp"
#include "DPR500.hpp"

#include "get_alines.hpp"
#include "no_imaging_compare.hpp"

using namespace ICR::agilent;
using namespace ICR::analogic;
using namespace ICR::lecroy;
using namespace ICR::pulser;
using namespace ICR;




int
main  (int ac, char **av)
{
  ICR::directory dir("/data/2011/03/04/water/test");  //the current directory

  analogic_remote_control analogic("/dev/ttyUSB0");
  std::cout<<"here"<<std::endl;

  WG33220A<agilent::IP>  driving_pulse("10.0.0.20");

  //std::cout<<"here0"<<std::endl;
  lecroy_64Xi  scope("10.0.0.10");
  std::cout<<"here0.1"<<std::endl;
  
  DPR500                 pulser("/dev/ttyUSB1");
  std::cout<<"here0.2"<<std::endl;

  PulserReceiverFactory  factory;   
  RPL2* imaging_pulse = factory.make_RPL2();
  pulser.attach_A(imaging_pulse);
  double imaging_voltage = 400;


  driving_pulse.turn_off();
  driving_pulse.frequency(.5e6);
  
  driving_pulse.burst_ext(edge::POSITIVE);
  driving_pulse.trigger(trigger::BUS);
  driving_pulse.burst_on();

  const double analogic_delay = 28; //microseconds
  
  const size_t repeats = 1;
  
  //the different pressures
  for(double driving_voltage=10e-3;driving_voltage<160e-3;driving_voltage+=5e-3)
    {
      std::cout<<"driving voltage = "<<driving_voltage<<std::endl;

      driving_pulse.turn_off();
      driving_pulse.cycles(10);
      driving_pulse.voltage(driving_voltage); 
      driving_pulse.shape(shape::SIN);
      
      driving_pulse.phase(0);
      driving_pulse.burst_ext(edge::POSITIVE);
      driving_pulse.trigger(trigger::BUS);
      driving_pulse.burst_on();
      driving_pulse.turn_on();

      // std::cout<<"phases"<<std::endl;
      // phase_plot phases(dir.get_directory(), driving_voltage, "phases");
      // phases.setup_analogic(analogic, analogic_delay);
      // phases.setup_pulser(pulser, imaging_pulse, imaging_voltage);
      // phases.run(driving_pulse,scope,repeats);

      driving_pulse.turn_off();
      //the different times
      for(double agilent_delay=5000;agilent_delay>=100;agilent_delay-=100){
      	std::cout<<"agilent_delay = "<<agilent_delay<<std::endl;


      	//0th exp (just one driving wave and imaging the tail afterwards.)

      	std::cout<<"tail"<<std::endl;
      	imaging_on<1> tail(dir.get_directory(), driving_voltage, "imaging_tail");
      	tail.setup_analogic(analogic, analogic_delay, agilent_delay);
      	tail.setup_pulser(pulser, imaging_pulse, imaging_voltage);
      	tail.run(driving_pulse,scope,repeats);


      	// driving_pulse.turn_off();
      	// two_sin_with_n_microsecond_gap(&driving_pulse, 0.5, agilent_delay, driving_voltage);
      	// driving_pulse.turn_on();

      	// //first exp
      	// std::cout<<"no imag wave"<<std::endl;
      	// no_imaging_compare no_imag(dir.get_directory(), driving_voltage, "no_imaging");
      	// no_imag.setup_analogic(analogic, analogic_delay, agilent_delay);
      	// no_imag.setup_pulser(pulser, imaging_pulse, imaging_voltage);
      	// double xcorr = no_imag.run(driving_pulse,scope,repeats);


      	// //2nd exp
      	// std::cout<<"im 1st"<<std::endl;
      	// imaging_on<0> imag0(dir.get_directory(), driving_voltage, "imaging_1st");
      	// imag0.setup_analogic(analogic, analogic_delay, agilent_delay);
      	// imag0.setup_pulser(pulser, imaging_pulse, imaging_voltage);
      	// imag0.run(driving_pulse,scope,repeats);

      	// //3d exp
      	// std::cout<<"im 2nd"<<std::endl;
      	// imaging_on<1> imag1(dir.get_directory(), driving_voltage, "imaging_2nd");
      	// imag1.setup_analogic(analogic, analogic_delay, agilent_delay);
      	// imag1.setup_pulser(pulser, imaging_pulse, imaging_voltage);
      	// imag1.run(driving_pulse,scope,repeats);
    
    
      }
  
  
    
    }
    
  analogic.turn_off();
    

  
  driving_pulse.turn_off();
}

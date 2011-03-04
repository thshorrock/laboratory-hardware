#include "analogic.hpp"
#include "stringify.hpp"
#include "agilent.hpp"
#include "lecroy.hpp"

#include "get_alines.hpp"
#include "no_imaging_compare.hpp"

using namespace ICR::agilent;
using namespace ICR::analogic;
using namespace ICR::lecroy;
using namespace ICR;









int
main  (int ac, char **av)
{
  ICR::directory dir("/data/2011/02/28/water/shorter_window-rerun");  //the current directory

  analogic_remote_control analogic("/dev/ttyUSB0");
  //std::cout<<"here"<<std::endl;

  WG33220A<agilent::IP>  driving_pulse("10.0.0.20");

  //std::cout<<"here0"<<std::endl;
  lecroy_64Xi  scope("10.0.0.10");
  //std::cout<<"here0.1"<<std::endl;

  //double lecroy_timebase = 5e-6;
  //double lecroy_trigger_delay = -50e-6;
  //double lecroy_timebase = 10e-6;
  //double lecroy_trigger_delay = -60e-6;
  unsigned long lecroy_samples = 250000;

  driving_pulse.turn_off();
  driving_pulse.frequency(.5e6);
  
  driving_pulse.burst_ext(edge::POSITIVE);
  driving_pulse.trigger(trigger::BUS);
  driving_pulse.burst_on();

  double analogic_delay = 3;
  double analogic_high = 2.5;
  double analogic_low  = 0.5;
  
  double clock = 1.25;
  
  const size_t repeats = 1;
  
  //the different pressures
  for(double driving_voltage=10e-3;driving_voltage<160e-3;driving_voltage+=5e-3)
    {
      std::cout<<"driving voltage = "<<driving_voltage<<std::endl;

      

      //the different times
      for(double agilent_delay=5000;agilent_delay>=100;agilent_delay-=100){
	std::cout<<"agilent_delay = "<<agilent_delay<<std::endl;

	if (agilent_delay<640)
	  clock = 1.25;
	else if (agilent_delay<1280) 
	  clock = 2.5;
	else if (agilent_delay<2560) 
	  clock = 5;
	else 
	  clock = 10;

	std::string no_imaging = "AT TRIG RPT 1 ( FOR "+stringify(analogic_delay)+"u 0.0 FOR 2u "+stringify(analogic_low)+" FOR "+stringify(agilent_delay+18)+"u 0.0 FOR 2u "+stringify(analogic_low)+" FOR 6u 0) CLK = "+stringify(clock)+"n";
	std::string imaging_first = "AT TRIG RPT 1 ( FOR "+stringify(analogic_delay)+"u 0.0 FOR 2u "+stringify(analogic_high)+" FOR "+stringify(agilent_delay+18)+"u 0.0 FOR 2u "+stringify(analogic_low)+" FOR 6u 0) CLK = "+stringify(clock)+"n" ;
	std::string imaging_second = "AT TRIG RPT 1 ( FOR "+stringify(analogic_delay)+"u 0.0 FOR 2u "+stringify(analogic_low)+" FOR "+stringify(agilent_delay+18)+"u 0.0 FOR 2u "+stringify(analogic_high)+" FOR 6u 0) CLK ="+stringify(clock)+"n";

	//std::cout<<"here2"<<std::endl;


	driving_pulse.turn_off();
	two_sin_with_n_microsecond_gap(&driving_pulse, 0.5, agilent_delay, driving_voltage);
	driving_pulse.turn_on();
	//std::cout<<"here3"<<std::endl;

	aline aline1;
	aline aline2;
    

	// std::cout<<"im none"<<std::endl;
	// analogic.expression(no_imaging);  //create a pulse
	// analogic.turn_on(); 
	// driving_pulse.turn_on(); 
	// driving_pulse.trigger_now();
	// driving_pulse.trigger_now();
	no_imaging_compare no_imag(dir.get_directory(), driving_voltage);
	no_imag.setup_analogic(analogic, analogic_delay, agilent_delay);
	double xcorr = no_imag.run(driving_pulse,scope,repeats);


	// for(size_t i=0;i<repeats;++i){
	//   get_alines(driving_pulse, scope, aline1, aline2);
      
	//   ICR::lecroy::save_gnuplot(aline2-aline1,   "./test.dat");
	// }
	// std::cout<<"im first"<<std::endl;

	analogic.expression(imaging_first);  //create a pulse
    
	analogic.turn_on(); 
	driving_pulse.trigger_now();
	driving_pulse.trigger_now();
	for(size_t i=0;i<repeats;++i){
	  get_alines(driving_pulse, scope, aline1, aline2);
      
	  ICR::lecroy::save_gnuplot(aline2-aline1,   "./test.dat");
	}
    
	std::cout<<"im 2nd"<<std::endl;
	analogic.expression(imaging_second);  //create a pulse
	analogic.turn_on(); 
    
	driving_pulse.trigger_now();
	driving_pulse.trigger_now();
	for(size_t i=0;i<repeats;++i){
	  get_alines(driving_pulse, scope, aline1, aline2);
      
	  ICR::lecroy::save_gnuplot(aline2-aline1,   "./test.dat");
	}
    
    
      }
  
      analogic.turn_off();
    

  
      driving_pulse.turn_off();
  
    
    }
    
}

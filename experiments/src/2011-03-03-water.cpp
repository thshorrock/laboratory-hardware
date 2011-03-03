#include "analogic.hpp"
#include "stringify.hpp"
#include "agilent.hpp"
#include "lecroy.hpp"

using namespace ICR::agilent;
using namespace ICR::analogic;
using namespace ICR::lecroy;
using namespace ICR;

void 
get_alines(
	  WG33220A<agilent::IP>& pulser,
	  lecroy_64Xi&  scope,
	  aline& al1,
	  aline& al2
	  ) 
{
  try{
    pulser.trigger_now();
    lecroy_file file =  scope.get_waveform(location::C4);
    al1 = file.get_data1(0);
    al2 = file.get_data1(1);
  }
  catch(exception::could_not_get_fresh_aquisition& e){
    std::cout<<"missed trigger - refiring"<<std::endl;
    pulser.trigger_now();
    pulser.trigger_now();
    get_alines(pulser,scope, al1, al2);
  }

}


int
main  (int ac, char **av)
{
  analogic_remote_control analogic("/dev/ttyUSB0");
  std::cout<<"here"<<std::endl;

  WG33220A<agilent::IP>  driving_pulse("10.0.0.20");

  std::cout<<"here0"<<std::endl;
  lecroy_64Xi  scope("10.0.0.10");
  std::cout<<"here0.1"<<std::endl;

  driving_pulse.turn_off();
  driving_pulse.frequency(.5e6);
  
  driving_pulse.burst_ext(edge::POSITIVE);
  driving_pulse.trigger(trigger::BUS);
  driving_pulse.burst_on();

  double analogic_delay = 3;
  double analogic_high = 1.5;
  double analogic_low  = 0.5;
  
  double clock = 1.25;
  
  const size_t repeats = 50;
  
  for(size_t i=50;i<100;i+=50){
  
     double agilent_delay = i;
    if (agilent_delay<800)
      clock = 1.25;
    else
      clock = 2.5;
  std::string no_imaging = "AT TRIG RPT 1 ( FOR "+stringify(analogic_delay)+"u 0.0 FOR 2u "+stringify(analogic_low)+" FOR "+stringify(agilent_delay+18)+"u 0.0 FOR 2u "+stringify(analogic_low)+" FOR 1u 0) CLK = "+stringify(clock)+"n";
    std::string imaging_first = "AT TRIG RPT 1 ( FOR "+stringify(analogic_delay)+"u 0.0 FOR 2u "+stringify(analogic_high)+" FOR "+stringify(agilent_delay+18)+"u 0.0 FOR 2u "+stringify(analogic_low)+" FOR 1u 0) CLK = "+stringify(clock)+"n" ;
    std::string imaging_second = "AT TRIG RPT 1 ( FOR "+stringify(analogic_delay)+"u 0.0 FOR 2u "+stringify(analogic_low)+" FOR "+stringify(agilent_delay+18)+"u 0.0 FOR 2u "+stringify(analogic_high)+" FOR 1u 0) CLK ="+stringify(clock)+"n";


  std::cout<<"here2"<<std::endl;
    two_sin_with_n_microsecond_gap(&driving_pulse, 0.5, agilent_delay );
    std::cout<<"here3"<<std::endl;
    analogic.expression(no_imaging);  //create a pulse
    analogic.turn_on(); 
    driving_pulse.turn_on(); 
    aline aline1;
    aline aline2;
    
    driving_pulse.trigger_now();
    for(size_t i=0;i<repeats;++i){
      get_alines(driving_pulse, scope, aline1, aline2);
      
      ICR::lecroy::save_gnuplot(aline2-aline1,   "./test.dat");
    }
    // aline3 = aline1 + aline     ;
    
    
  }
  
  analogic.turn_off();
    

  
  driving_pulse.turn_off();
  
  //driving_pulse.shape(shape::SQUARE);
  //n_microseconds_after_sin(&driving_pulse, 0.5e6, 10 );
  //sin_after_n_microseconds(&driving_pulse, 0.5, 3);
  // analogic.echo(true);
  // for(size_t i=0;i<20;++i){
    
    
    
  //   analogic.expression("AT TRIG RPT 1 ( FOR "+stringify(i)+"u 0.0 FOR 2u 0.5 FOR 1u 0) CLK = 1.25n MARK = 20u");  //create a pulse
    
  
  // }
}

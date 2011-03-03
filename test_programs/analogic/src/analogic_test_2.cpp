#include "analogic.hpp"
#include "stringify.hpp"


int
main  (int ac, char **av)
{
  //use the coms namespace
  using namespace ICR::analogic;
  using namespace ICR;
  
  
  // //Open a serial connection to a device attached to "COM1" port
  analogic_remote_control analogic("/dev/ttyUSB0");
  analogic.echo(true);
  for(size_t i=0;i<20;++i){


    analogic.expression("AT TRIG RPT 1 ( FOR "+stringify(i)+"u 0.0 FOR 2u 0.5 FOR 1u 0) CLK = 1.25n MARK = 20u");  //create a pulse
    analogic.turn_on();  

  
  }
  //run
  // analogic.turn_off();  

  //make some data
  // constant_segment c1(32e-6, 0.1);
  // constant_segment c2(2e-6, 0.3);
  // constant_segment c3(16e-6, 0.0);
  
  // segment_container s;
  // s.add(c1);
  // s.add(c2);
  // s.add(c3);
  
  // analogic_data data(1.25e-9);
  // data.add(&c1);
  // //data.add(&c2);
  // //data.add(&c3);
  
  // analogic.data(data);

  
    
    

  //The connections automatically close when out of scope.
}

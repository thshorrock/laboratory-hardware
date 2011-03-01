#include "analogic.hpp"
int
main  (int ac, char **av)
{
  //use the coms namespace
  using namespace ICR::analogic;
  
  
  //Open a serial connection to a device attached to "COM1" port
  analogic_remote_control analogic("/dev/ttyUSB0");
  
  analogic.clock_rate(1.25e-9);  //set fastest clock speed
  analogic.expression("AT TRIG FOR 28u 0.0 FOR 2u 0.5 FOR 1u 0");  //create a pulse
  analogic.turn_on();  //run


  //The connections automatically close when out of scope.
}

#include "analogic.hpp"
int
main  (int ac, char **av)
{
  //use the coms namespace
  using namespace ICR::analogic;
  
  
  //Open a serial connection to a device attached to "COM1" port
  analogic_remote_control analogic("/dev/ttyUSB0");
  analogic.echo(true);
  analogic.expression("AT TRIG RPT 1 ( FOR 30u 0.0 FOR 2u 0.5 FOR 1u 0) CLK = 1.25n MARK = 20u");  //create a pulse
  //analogic.turn_on();  //run
  //analogic.turn_off();  


  //The connections automatically close when out of scope.
}

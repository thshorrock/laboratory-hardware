#include "coms_manager.hpp"
int
main  (int ac, char **av)
{
  //use the coms namespace
  using namespace ICR::coms;
  
  
  //Open a serial connection to the analogic device attached to "/dev/ttyUSB0" port 
  serial_manager serial("/dev/ttyUSB0",
			  9600,
			  flow_control::hardware
			  ); 
  
  
  
  ICR::coms::sleep(1000);
  //send a command to the serial device
  serial.send("RUN\r");

  //The connections automatically close when out of scope.
}

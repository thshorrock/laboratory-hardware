#include "lecroy.hpp"
#include "agilent.hpp"
#include "DPR500.hpp"
#include "filename.hpp"
#include "readme.hpp"
#include "stringify.hpp"
#include "boost/progress.hpp"

using namespace ICR;
using namespace ICR::lecroy;
using namespace ICR::pulser;
using namespace ICR::agilent;
using namespace ICR::lecroy::location;
int
main  (int ac, char **av)
{
  
  //Equipment
  lecroy_64Xi     scope("10.0.0.10");
  WG33120A       bus_sync("/dev/ttyUSB0");


  //BUS
  bus_sync.shape(shape::SQUARE);
  bus_sync.trigger(trigger::BUS);
  bus_sync.burst_on();

  
  //LECROY SETUP
  scope.set_date();
   scope.sequence(2,500);


  // scope.demand_fresh_aquisition();

  scope.trigger_mode(trigger_mode::NORM);
  
  scope.auto_calibrate(false);


  for(int i=0;i<100000;++i){
    std::cout<<"i = "<<i<<std::endl;

    //  scope.trigger_mode(trigger_mode::SINGLE);
    
    //Trigger	
    bus_sync.trigger_now();
    lecroy_file file;
      
    file =  scope.get_waveform(C4);
  }
}

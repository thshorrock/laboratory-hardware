#include "DPR500.hpp"

using namespace ICR::pulser;
int
main  (int ac, char **av)
{

  PulserReceiverFactory factory;
  
  RPL2* low_freq_pulser = factory.make_RPL2();
  
  //low_freq_pulser->set_gain(30);
  //low_freq_pulser->set_prf(1000);

  try {
    DPR500 dpr500("/dev/ttyUSB0");
    dpr500.blink(100);
    dpr500.attach_A(low_freq_pulser);

    
    dpr500.notify();
  }
  catch (exceptions::failed_to_contact_DPR500_device& e)    {
    std::cout<<"could not open DPR500 device"<<std::endl;
    
  }
  

}

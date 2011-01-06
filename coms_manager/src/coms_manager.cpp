#include "coms_manager/coms_manager.hpp"
#include <boost/thread.hpp>

ICR::coms::coms_manager::~coms_manager()
{}

void
ICR::coms::sleep(unsigned long ms)
{ 

  boost::this_thread::sleep(boost::posix_time::milliseconds(ms)); 


}

#pragma once
#include "stringify.hpp"
#include "agilent.hpp"
#include "lecroy.hpp"

void 
get_alines(
	  ICR::agilent::WG33220A<ICR::agilent::IP>& pulser,
	  ICR::lecroy::lecroy_64Xi&  scope,
	  ICR::lecroy::aline& al1,
	  ICR::lecroy::aline& al2
	  ) 
{
  try{
    pulser.trigger_now();
    ICR::lecroy::lecroy_file file =  scope.get_waveform(ICR::lecroy::location::C2);
    al1 = file.get_data1(0);
    al2 = file.get_data1(1);
  }
  catch(ICR::exception::could_not_get_fresh_aquisition& e){
    std::cout<<"missed trigger - refiring"<<std::endl;
    pulser.trigger_now();
    pulser.trigger_now();
    get_alines(pulser,scope, al1, al2);
  }

}

void 
get_alines(
	  ICR::agilent::WG33220A<ICR::agilent::IP>& pulser,
	  ICR::lecroy::lecroy_64Xi&  scope,
	  ICR::lecroy::aline& al1
	  ) 
{
  try{
    pulser.trigger_now();
    ICR::lecroy::lecroy_file file =  scope.get_waveform(ICR::lecroy::location::C2);
    al1 = file.get_data1(0);
  }
  catch(ICR::exception::could_not_get_fresh_aquisition& e){
    std::cout<<"missed trigger - refiring"<<std::endl;
    pulser.trigger_now();
    pulser.trigger_now();
    get_alines(pulser,scope, al1);
  }

}

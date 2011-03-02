#pragma once

#include "analogic/trigger_mode.hpp"

#include <string>

namespace ICR {
  namespace analogic {

    class main_header{
      boost::array<char,32> result;
    public:
      main_header(
		  const float& clock_period,
		  const enum ::ICR::analogic::trigmode::type& trigger_mode = ::ICR::analogic::trigmode::POS_START,
		  const float& offset          = 0.0,
		  const float& signal_amp      = 1.0, //pk-pk
		  const float& noise_amp       = 0,
		  const float& noise_bandwidth = 0,
		  const float& filter_cutoff   = 0 //no filter
		  );
      
      std::string
      str() const 
      {
	std::string tmp;
	for(size_t i=0;i<32;++i){
	  tmp.push_back(result[i]);
	}
	return tmp;
      }
    };
    
  }
}


ICR::analogic::main_header::main_header(
					const float& clock_period,
					const enum ::ICR::analogic::trigmode::type& trigger_mode,
					const float& offset,
					const float& signal_amp,
					const float& noise_amp,
					const float& noise_bandwidth,
					const float& filter_cutoff
					)
  : result()
{
  if (sizeof(short) != 4) throw("float must be 4 bytes for analogic to work, sorry");
  if (sizeof(short) != 2) throw("short must be 2 bytes for analogic to work, sorry");
  
  result[0] = (char) clock_period;
  result[4] = (char) offset;
  result[8] = (char) signal_amp;
  result[12] = (char) noise_amp;
  result[16] = (char) noise_bandwidth;
  result[20] = (char) filter_cutoff;
  result[24] = (char) (short) trigger_mode;
  for(size_t i=26;i<32;++i){
    result[i] = 0x00;
  }
}

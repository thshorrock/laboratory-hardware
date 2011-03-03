#pragma once
#include "main_header_detail.hpp"
#include "analogic/trigger_mode.hpp"

#include <string>

namespace ICR {
  namespace analogic {
    
    class main_header{
     char result[32];
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
  detail::main_head_pack p;
  p.header.clock_period = clock_period;
  p.header.offset = offset;
  p.header.signal_amp = signal_amp;
  p.header.noise_amp = noise_amp;
  p.header.noise_bandwidth = noise_bandwidth;
  p.header.filter_cutoff = filter_cutoff;
  p.header.mode = trigger_mode;

  memcpy(result,p.str,32);

  for(size_t i=26;i<32;++i){
    result[i] = 0x00; //finish off
  }

}

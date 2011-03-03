#pragma once

#include "segment_interface.hpp"
#include "data_header.hpp"

namespace ICR{
  namespace analogic{

    /** A constant segment */
    class constant_segment : public ISegment
    {
      const double m_duration,m_value;
    public:
      constant_segment(const double duration, const double& value)
	: m_duration(duration), m_value(value) 
      {}
      
      std::string
      construct_header(const double& dT)
      {
	unsigned int pulses = (unsigned int)( m_duration/dT +0.5);

	data_header<segment_type::CONSTANT> h(pulses, m_value);
	return h.str();
      }
      
      ISegment* 
      clone() 
      {
	return new constant_segment(*this);
      }
      
      
    };


  }
}

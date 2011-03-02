#pragma once

#include "segment_interface.hpp"
#include "data_header.hpp"

namespace ICR{
  namespace analogic{

    /** A constant segment */
    class constant_segment : public ISegment
    {
      const double m_value;
    public:
      constant_segment(const double duration, const double& value)
	: m_value(value) 
      {}
      
      std::string
      construct_header(const double& dT)
      {
	data_header h;
	unsigned int pulses = (unsigned_int) m_duration/dT +0.5;
	h.set_header(segment_type::constant, pulses, m_value);
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

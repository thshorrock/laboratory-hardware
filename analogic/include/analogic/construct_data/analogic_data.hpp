#pragma once
#include "analogic/trigger_mode.hpp"
#include "segment_container.hpp"
#include "main_header.hpp"
#include "get_min_max.hpp"
#include "data_builder.hpp"


namespace ICR {

  namespace analogic {

    class analogic_data
    {
    public:
      analogic_data(const double& dT, const enum ::ICR::analogic::trigmode::type& trigger_mode = ::ICR::analogic::trigmode::POS_START)
	: m_dT(dT), m_mh(dT, trigger_mode), m_segment()
      {}
      

      void 
      add( const ::ICR::analogic::segment_container& s) {m_segment.add(s);}

      std::vector<std::string>
      collect_data() const;
      
    private:
      const double      m_dT;
      main_header       m_mh;
      ::ICR::analogic::segment_container m_segment;
    };

  }
}

std::vector<std::string>
ICR::analogic::analogic_data::collect_data() const
{

  std::vector<std::string> ret;
  ret.push_back(m_mh.str());
  //check there is data to push back.
  if (  m_segment.size() == 0 ) {return ret;}
  
  get_min_max minmax;

  //find the minimum and maximum.
  for_each (m_segment.begin(), m_segment.end(), minmax);

  //add to ret.
  data_builder make_ret(ret, m_dT, minmax);
  for_each (m_segment.begin(), m_segment.end(), make_ret);

  return ret;
}

#pragma once
#include "get_min_max.hpp"
#include <string>
#include <vector>

namespace ICR {
  namespace analogic {

    class data_builder
    {
      std::vector<std::string>& m_data;
      const double m_dT;
      const boost::optional<double> m_min, m_max;
      
    public:
      data_builder(std::vector<std::string>& data, const double& dT, const get_min_max& minmax)
	: m_data(data), m_dT(dT), m_min(minmax.min()), m_max(minmax.max())
      {}

      void operator()(const boost::shared_ptr<ISegment>& it)
      {
	m_data.push_back(it->construct_header(m_dT));
	std::vector<std::string> data =  it->construct_data(m_dT, m_min, m_max);
	m_data.insert(m_data.end(), data.begin(), data.end());
      }
      
    };

  }
}

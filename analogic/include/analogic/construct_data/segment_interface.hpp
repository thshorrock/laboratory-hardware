#pragma once

#include <vector>
#include <string>
#include <boost/optional.hpp>

namespace ICR{
  namespace analogic{
    /** Interface to a segment of data.
     * Note that you will need to overload the min and max functions if you overload the construct_data function. */

    class ISegment 
    {
    public:
      
      /** Clone the segment */
      virtual 
      ISegment* 
      clone() = 0;
      
      /** Obtain the minimum of the data segment.
       * By default this is unitialised.
       */
      virtual
      boost::optional<double>
      min() const 
      {
	boost::optional<double> d;
	return d;
      }
      /** Obtain the maximum of the data segment.
       * By default this is unitialised.
       */
      virtual
      boost::optional<double>
      max() const
      {
	boost::optional<double> d;
	return d;
      }
      /** Construct the header.
       */
      virtual 
      std::string
      construct_header(const double& dT) = 0;
       
      /** Construct the data.
       *  By default there is no data and an empty string is returned.
       *  Note that the maximum size of the data chunks is usually 80 chars.
       */
      virtual
      std::vector< std::string >
      construct_data(const double& dT, 
		     const boost::optional<double>& global_min, 
		     const boost::optional<double>& global_max) const
      {
	std::vector<std::string> empty ; //by default
	return empty; 
      }
      
      virtual 
      ~ISegment(){};

    };
    

  }
}

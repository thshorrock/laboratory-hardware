#pragma once

namespace ICR {
  namespace analogic {
    
    class get_min_max
    {
      boost::optional<double> m_min;
      boost::optional<double> m_max;
    public:
      
      boost::optional<double> min() const {return m_min;}
      boost::optional<double> max() const {return m_max;}

      void operator()(const boost::shared_ptr<ISegment>& s)
      {
	boost::optional<double> min = s->min();
	boost::optional<double> max = s->max();
	
	if (min)
	  {
	    //min is inialised.
	    if (!m_min)
	      m_min = *min;  //m_min is not inialised, initialise it now
	    else{
	      if (*min < *m_min) 
		*m_min = *min;  //update
	    }
	  };
	if (max)
	  {
	    //max is inialised.
	    if (!m_max)
	      m_max = *max;  //m_max is not inialised, initialise it now
	    else{
	      if (*max > *m_max) 
		*m_max = *max; //update
	    }
	  };
      }

      
    };
  }
}

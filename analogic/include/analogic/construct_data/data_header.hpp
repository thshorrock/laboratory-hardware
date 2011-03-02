#include "data_header_details.hpp"
#include <boost/array.hpp>

namespace ICR {
  namespace analogic {


    template<int segment_type>
    class data_header
    {
      boost::array<char,10> m_header;

    public:
      
      data_header(const unsigned int& block_length);
      
      data_header(const unsigned int clock_pulses, const float value);
      
      data_header();
      
      data_header(const enum repeat_loop::type& repeat_option, const unsigned short& repeats);

      data_header(const enum repeat_loop::type& repeat_option);
      
      boost::array<char,10> 
      header() const {return m_header;}


      std::string
      str() const {
	std::string tmp;
	for(size_t i=0;i<10;++i){
	  tmp.push_back(m_header[i]);
	}
	return tmp;
      }


    };

    template<>
    data_header<segment_type::BLOCK>::data_header(const unsigned int& block_length)
    {
      m_header[0] = (char) segment_type::BLOCK;
      segment_length<segment_type::BLOCK> len(block_length);
      constant_value<segment_type::BLOCK> c;
      m_header[2] = (char) len.block_length();
      m_header[6] = (char) c.value();
    }


    template<>
    data_header<segment_type::CONSTANT>::data_header(const unsigned int clock_pulses, const float value)
    {
      m_header[0] = (char) segment_type::CONSTANT;
      segment_length<segment_type::CONSTANT> len(clock_pulses);
      constant_value<segment_type::CONSTANT> c(value);
      m_header[2] = (char) len.constant_clock_pulses();
      m_header[6] = (char) c.value();
    }

    template<>
    data_header<segment_type::END>::data_header()
    {
      m_header[0] = (char) segment_type::END;
      segment_length<segment_type::END> len;
      constant_value<segment_type::END> c;
      m_header[2] = (char) len.end_value();
      m_header[6] = (char) c.value();
    }

    template<>
    data_header<segment_type::REPEAT>::data_header(const enum repeat_loop::type& repeat_option, const unsigned short& repeats)
    {
      m_header[0] = (char) segment_type::REPEAT;
      segment_length<segment_type::REPEAT> len(repeat_option,repeats);
      constant_value<segment_type::REPEAT> c;
      m_header[2] = (char) len.loop_type();
      m_header[4] = (char) len.repeats();
      m_header[6] = (char) c.value();
    }

    template<>
    data_header<segment_type::REPEAT_END>::data_header(const enum repeat_loop::type& repeat_option)
    {
      m_header[0] = (char) segment_type::REPEAT;
      segment_length<segment_type::REPEAT_END> len(repeat_option);
      constant_value<segment_type::REPEAT_END> c;
      m_header[2] = (char) len.end_repeat_value();
      m_header[6] = (char) c.value();
  
    }

  }
}


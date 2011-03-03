#pragma once
#include "data_header_details.hpp"
#include <boost/array.hpp>

namespace ICR {
  namespace analogic {


    template<int segment_type>
    class data_header
    {
      char m_header[10];

    public:
      
      data_header(const unsigned int& block_length);
      
      data_header(const unsigned int clock_pulses, const float value);
      
      data_header();
      
      data_header(const enum repeat_loop::type& repeat_option, const unsigned short& repeats);

      data_header(const enum repeat_loop::type& repeat_option);
      

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
      detail::pack_data_header_1 pack;
      pack.header.type =  segment_type::BLOCK;
      segment_length<segment_type::BLOCK> len(block_length);
      constant_value<segment_type::BLOCK> c;
      pack.header.i = len.block_length();
      pack.header.value  =  c.value();
      memcpy(m_header, pack.str, 10);
    }


    template<>
    data_header<segment_type::CONSTANT>::data_header(const unsigned int clock_pulses, const float value)
    {
      
      detail::pack_data_header_1 pack;
      
       pack.header.pad  = 0x00;
       pack.header.type = (char) segment_type::CONSTANT;
      // std::cout<<"constant type = "<< (int) (char) segment_type::CONSTANT<<std::endl;

       segment_length<segment_type::CONSTANT> len(clock_pulses);
       constant_value<segment_type::CONSTANT> c(value);
       std::cout<<"clock pulses = "<<clock_pulses<<std::endl;

       pack.header.i =  (long) clock_pulses;
       pack.header.value  =  value;
       std::cout<<"value = "<<c.value()<<std::endl;
       
       std::cout<<"pack.i = "<<pack.header.i<<std::endl;


      std::cout<<"p = ";
       for(size_t i=0;i<10;++i){
      	std::cout<<(int)pack.str[i]<<" ";
      }
      std::cout<<std::endl;

      // memcpy(m_header, pack.str, 10);
      m_header[0] = 0;
      m_header[1] = 2;
      m_header[2] = (long) (clock_pulses);
      m_header[6] = (double) value;
    }

    template<>
    data_header<segment_type::END>::data_header()
    {
      detail::pack_data_header_1 pack;
      pack.header.type =  segment_type::END;
      segment_length<segment_type::END> len;
      constant_value<segment_type::END> c;
      pack.header.i =  len.end_value();
      pack.header.value =  c.value();
      memcpy(m_header, pack.str, 10);
    }

    template<>
    data_header<segment_type::REPEAT>::data_header(const enum repeat_loop::type& repeat_option, const unsigned short& repeats)
    {
      detail::pack_data_header_2 pack;
      pack.header.type=  segment_type::REPEAT;
      segment_length<segment_type::REPEAT> len(repeat_option,repeats);
      constant_value<segment_type::REPEAT> c;
      pack.header.loop =  len.loop_type();
      pack.header.repeats =  len.repeats();
      pack.header.value =  c.value();
      memcpy(m_header, pack.str, 10);
    }

    template<>
    data_header<segment_type::REPEAT_END>::data_header(const enum repeat_loop::type& repeat_option)
    {
      detail::pack_data_header_1 pack;
      pack.header.type =  segment_type::REPEAT;
      segment_length<segment_type::REPEAT_END> len(repeat_option);
      constant_value<segment_type::REPEAT_END> c;
      pack.header.i =  len.end_repeat_value();
      pack.header.value =  c.value();
      memcpy(m_header, pack.str, 10);
  
    }

  }
}


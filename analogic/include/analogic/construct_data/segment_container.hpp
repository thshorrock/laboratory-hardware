#include "segment_interface.hpp"
#include "data_header.hpp"

#include "boost/bind.hpp"


namespace ICR{
  namespace analogic{

    class segment_container
    {
      
    public:
      typedef boost::shared_ptr<ISegment> segment_ptr;
      typedef std::vector<segment_ptr>::iterator iterator ;
      typedef std::vector<segment_ptr>::const_iterator const_iterator ;
    
    protected:
      std::vector<segment_ptr> m_segment;
      
      
      void 
      add_segment_ptr(const segment_ptr& s) 
      {
	add(s.get());
      }
    public:
      virtual ~segment_container(){};

      iterator begin() {return m_segment.begin();}
      iterator end()   {return m_segment.end();}
      const_iterator begin() const {return m_segment.begin();}
      const_iterator end()   const {return m_segment.end();}

      segment_ptr& operator[](const size_t& i) {return m_segment[i];}

      virtual void add(ISegment* s) 
      {
	segment_ptr p ( s->clone() );
	m_segment.push_back(p);
      }
      virtual void add(const segment_container& s) 
      {
	for_each(s.begin(), s.end(), boost::bind(&segment_container::add_segment_ptr, this, _1) );
      }
      
      
      size_t size() const {return m_segment.size();}
   
    };
    
    class repeat_segment_container : public segment_container
    {
      class repeat_segment : public ISegment
      { 
	const enum repeat_loop::type m_type;
	const unsigned short m_repeats;
      public:
	repeat_segment(const enum repeat_loop::type& repeat_option, const unsigned short& repeats)
	  : m_type(repeat_option), m_repeats(repeats)
	{}

	std::string
	construct_header(const double& dT)
	{
	  data_header<segment_type::REPEAT> h(m_type, m_repeats);
	  return h.str();
	}

	
	ISegment* 
	clone() 
	{
	  return new repeat_segment(*this);
	}
      
      };

      class end_repeat_segment : public ISegment
      { 
	const enum repeat_loop::type m_type;
      public:
	end_repeat_segment(const enum repeat_loop::type& repeat_option)
	  : m_type(repeat_option)
	{}

	std::string
	construct_header(const double& dT)
	{
	  data_header<segment_type::REPEAT_END> h(m_type);
	  return h.str();
	}
	
	ISegment* 
	clone() 
	{
	  return new end_repeat_segment(*this);
	}
      
      };
    public:
      repeat_segment_container( const enum repeat_loop::type& repeat_option, const unsigned short& repeats) ;
      
      void add(ISegment* s) 
      {
	segment_ptr p ( s->clone() );
	segment_container::m_segment.insert(segment_container::m_segment.end()-1,p); //add one before
      }
    };

  }
}


ICR::analogic::repeat_segment_container::repeat_segment_container(const enum repeat_loop::type& repeat_option, const unsigned short& repeats)
{
  //make a repeat header.
  repeat_segment s(repeat_option, repeats);
  segment_container::add(&s);

  //make a end header
  end_repeat_segment end_s(repeat_option);
  segment_container::add(&end_s);
}

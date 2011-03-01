#include <bitset>

namespace ICR{
  namespace analogic{
    namespace pulses {
      
      class get_integer
      {
	double m_scale,  m_min;
      public:
	get_integer( const double& min, const double& max)
	  : m_scale(16.0/(max-min)), m_min(min)
	{}
	unsigned int operator()(const double& d)
	{
	  double rescaled = (d-m_min)*m_scale;
	  //round  to nearest int.
	  int i = (int) (rescaled+.5); 
	  std::bitset<16> bits; 
	  bits.set(16-i,true);
	  return (unsigned int) bits.to_ulong();
	}
      };
      
      class ISection
      {
      public:
	
	virtual
	double 
	local_min() const = 0 ;
	
	virtual
	double 
	local_max() const = 0 ;
	
	virtual
	vector<double> 
	construct_data(const double& dT, 
		       const double& global_min, 
		       const double& global_max) = 0;
      };

      class single_pulse : public ISection
      {
	double m_delay, m_duration, m_after, m_voltage;
      public:
	single_pulse(const double& delay,
		     const double& duration,
		     const double& after,
		     const double& voltage)
	  : m_delay(delay),
	    m_duration(duration),
	    m_after(after),
	    m_voltage(voltage)
	{}
	
	double local_min() const {return 0.0;}
	double local_max() const {return voltage;}
	
	std::vector<double> 
	construct_data(const double& dT, 
		       const double& global_min, 
		       const double& global_max);

      };

      class data_pack
      {
	data_header m_header;
	boost::array<char,80> m_pack;
      public:
	data_pack(
	std::string  header() const{ return m_header.header_str();}
	std::string  data()   const
	{ 
	  std::string tmp;
	  for(size_t i=0;i<80;++i){
	    tmp.push_back(m_pack[i]);
	  }
	  return tmp;
	}
      }

      class analogic_data
      {
	std::vector<ISection*>& m_sections;
	std::vector<unsigned int> m_data;
	main_header m_mh;
      public:
	analogic_data(const double& dT,std::vector<ISection*>& sections , const enum trigmode::type& trigger_mode = POS_START);
	std::vector<data_pack> data() {return m_data;}
      };
    
    }
  }
}




std::vector<unsigned int> 
ICR::analogic::pulses::construct_data(const double& dT, 
				      const double& global_min, 
				      const double& global_max);
{

  double total_time = m_delay + m_duration + m_after;
  size_t samples = total_time/dT;
  std::vector<int> result(samples);
  
  const size_t samples_pre = samples*m_delay/total_time;
  const size_t samples_dur = samples*m_dur/total_time;
  const size_t samples_post = samples*m_after/total_time;
  
  get_integer get_i(global_min, global_max);
  
  

  for(size_t i=0;i<samples_pre;++i){
    result[i] = get_i(0.0);
  }
  for(size_t i=samples_pre;i<samples_pre+samples_dur;++i){
    result[i] = get_i(m_voltage);
  }
  for(size_t i=samples_pre+samples_dur;i<samples;++i){
    result[i] = get_i(0.0);
  }
  
  return result;
}



ICR::analogic::analogic_data::analogic_data(const double& dT, std::vector<ISection*>& sections, const enum trigmode::type& trigger_mode)  
  : m_mh(dT,trigger_mode),
{
  double global_min;
  double global_max;
  double min = sections[0]->get_local_min();
  double max = sections[0]->get_local_max();
  
  for(size_t i=1;i<sections.size();++i){
    if (min<sections[i]->get_local_min()) min = sections[i]->get_local_min();
    if (max>sections[i]->get_local_max()) max = sections[i]->get_local_max();
  }
  for(size_t i=1;i<sections.size();++i){
    std::vector<unsigned int> tmp = sections->construct_data(dT, min, max) ;
    m_data.insert(end(), tmp.begin(), tmp.end());
  }
}



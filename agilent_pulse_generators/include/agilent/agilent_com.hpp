#pragma once
#include <string>
#include <iostream>
#include "coms_manager.hpp"
#include <boost/smart_ptr.hpp>
//#include "agilent/vxi11_user.h"
#define BUF_LEN 100000

namespace ICR {
  namespace agilent{
    
    
    typedef ICR::coms::IPmanager IP;
    typedef ICR::coms::serial_manager SERIAL;


    namespace shape{
      enum type {SIN, SQUARE, PULSE, TRIANGLE, RAMP, NOISE, DC, USER};
    }
    namespace load{
      enum type {OHM50, INFTY}; 
    }
    namespace trigger{
      enum  type {IMMEDIATE, EXTERNAL, BUS}; 
    }
    
    struct edge{
      enum type {POSITIVE, NEGATIVE};
    };

    namespace exception{
      struct option_not_recognised{};
      struct failed_to_reset{};
      struct too_few_points{};
      struct too_many_points{};
    } 
    
    template<class com_method>
    class agilent_com : public com_method {
      bool m_burst_mode;
    public:
      agilent_com(const std::string& IPaddress);
      virtual ~agilent_com() ;

      
      void send(const std::string& cmd)
	throw (boost::system::system_error) 
      {
	//std::cout<<"cmd = "<<cmd<<std::endl;
	try{
	  com_method::send(cmd);
	  if (m_burst_mode) {
	  
	    ICR::coms::sleep(50);
	    com_method::send("*WAI\n");
	    ICR::coms::sleep(50);
	    int  opc_code = 0;
	    while (opc_code!= 1){
	      //   std::cout<<error()<<std::endl;
	      opc_code  =  atoi(com_method::timed_recv("*OPC?\n", 128,10).c_str()) ;
	      // std::cout<<"opc_code = "<<opc_code<<std::endl;
	    }
	    ICR::coms::sleep(100);
	    //com_method::send("*WAI\n");
	  }
	}
	catch(ICR::exception::timeout_exceeded& e) {
	  e.debug_print();
	  std::cout<<"AGILENT"<<std::endl;

	  throw;
	}
      }
      std::string recv(const std::string& cmd)
	throw(boost::system::system_error )
      {
	std::string ret = com_method::recv(cmd);
	if (m_burst_mode) {
	  com_method::send("*WAI\n");
	  std::string opc_code = com_method::recv("*OPC?\n");
	  //std::cout<<"opc_code = "<<opc_code<<std::endl;
	}
	return ret;
      }
      
      virtual std::string error();
      virtual void apply(const shape::type shape, const double freq,const  double volts,const  double offset);
      
      virtual void load(const load::type);
      virtual void duty_cycle(const double); // square wave only

      virtual void frequency(const double frequency);
      virtual void voltage(const double V);
      virtual void offset(const double offset);

      //burst commands
      virtual void cycles(const unsigned int cycles);
      virtual void burst_on(void);
      virtual void burst_off(void);
      virtual void phase(const double degrees);
      virtual void burst_int(const  double  rate);
      /**External trigger.*/
      virtual  void burst_ext();
      virtual  void burst_ext(const enum edge::type& );

      virtual void trigger(const trigger::type source);
      virtual void trigger_now() ;

      virtual void shape (const shape::type);
      virtual  void arb_wave(const std::string& name, const float*, const unsigned long& size);
      //void arb_wave(vector<double> data);

      virtual void reset();

    };
    
    template<class com_method>
    void 
    apply_waveform(agilent_com<com_method>* generator,const std::string& name, const double& sampling_freq, const  float* voltages, const unsigned long& num_points);


    
    template<class com_method>
    void
    pulse_after_n_microseconds(agilent_com<com_method>* gen, double delay, double duration = 2, double voltage = 2.5)
    {
      //max samples = 16000
      unsigned long samples = 16000;
      //max temp resolution = ((delay + duration)/16000 )e-6 seconds
      //last point = 0 to make it pulse so have only 15999 points
      double temp_res = ((delay+duration)/(samples-1))*1e-6; //seconds
      double freq_res = 1.0/temp_res;
      // if (freq_res >15e6) // round to 15MHz
      // 	freq_res = 15e6;
      // temp_res = 1.0/freq_res;
      samples = (delay+duration)/(temp_res*1e6)+1;
      std::cout<<"samples= "<<samples<<std::endl;

      boost::shared_array<float> data(new float[samples]);
      std::cout<<"temp_res = "<<temp_res<<std::endl;

      for(size_t i=0;i<(samples-1);++i){
	if (i*temp_res*1e6 < delay){
	  data[i] = 0;
	  // std::cout<<"data["<<i<<"]="<<data[i]<<std::endl;

	}
	else if (i*temp_res*1e6<(delay+duration)) {
	  data[i] = voltage;
	  //std::cout<<"data["<<i<<"]="<<data[i]<<std::endl;
	}
	else {
	  data[i] = 0;
	  // std::cout<<"data["<<i<<"]="<<data[i]<<std::endl;
	}
      }
      data[samples-1]=0;
      //std::cout<<"data["<<samples-1<<"]="<<data[samples-1]<<std::endl;
      
      apply_waveform<com_method>(gen,"pulse",freq_res, data.get(),samples);


    }
  }
}


namespace ICR {
  namespace agilent{
    
    template<class com_method>
    inline agilent_com<com_method>::agilent_com(const std::string& device)
    {}

    template<class com_method>
    inline agilent_com<com_method>::~agilent_com() {};

    template<>
    inline agilent_com<coms::IPmanager>::agilent_com(const std::string& IPaddress)
      : coms::IPmanager(IPaddress, "5025"),
	m_burst_mode(false)
    {}

    template<>
    inline  agilent_com<coms::serial_manager>::agilent_com(const std::string& port)
      : coms::serial_manager(port, 9600, ICR::coms::flow_control::hardware,ICR::coms::parity::none,ICR::coms::stop_bits::two,8),
	m_burst_mode(false)
    {
      //send("SYST:REM\n");
      //send("SYST:RWL\n");
      
      reset();

    }
    template<>
    inline agilent_com<coms::serial_manager>::~agilent_com()
    {
      
      //send("SYST:LOC\n");

    }
  }
}

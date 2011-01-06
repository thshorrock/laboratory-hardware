
%module hardware_agilent
%include stl.i
%include "std_string.i"
%include "carrays.i"
%array_class(float, floatArray);
 //%apply const std::string& {std::string* foo};
%{
#include "agilent/agilent_com.hpp"
#include "agilent/33220A.hpp"

/* Include in the generated wrapper file */
//typedef unsigned int size_t;
%}



/* Tell SWIG about it */
typedef unsigned int size_t;


namespace ICR{

  namespace agilent{
    
    
    namespace shape{
      enum type {SIN, SQUARE, PULSE, TRIANGLE, RAMP, NOISE, DC, USER};
    }
    namespace load{
      enum type {OHM50, INFTY}; 
    }
    namespace trigger{
      enum type {IMMEDIATE, EXTERNAL, BUS}; 
    }
    
    namespace exception{
      struct option_not_recognised{};
      struct failed_to_reset{};
    }

    template<class com_method>
    class agilent_com {

    public:
      agilent_com(const std::string& device);
      void send(const std::string& cmd);
      void recv(const std::string& cmd);
      void reset();
      void apply(const shape::type shape, 
		 const double freq,
		 const double volts,
		 const double offset);
      
      void shape(const shape::type);

      void load(const load::type);
      void duty_cycle(const double); // square wave only

      void frequency(const double frequency);
      void voltage(const double V);
      void offset(const double offset);

      /* //burst commands */
      void cycles(const unsigned int c);
      void burst_on(void);
      void burst_off(void);
      void phase(const double degrees);
      void burst_int(const  double  rate); 
      void burst_ext();

      void trigger(const trigger::type source); 
      void trigger_now() {send("*TRG");}; 
      std::string error();
      void arb_wave(const std::string name, const float*, const unsigned long size);
      // void arb_wave(const boost::array data);

    };

    %template(agilent_com_ip) agilent_com<ICR::coms::IPmanager>;   
    %template(agilent_com_serial) agilent_com<ICR::coms::serial_manager>;
    
    template<class com_method>
      class  WG33220A : public agilent_com<com_method>
    {
    public:
    WG33220A(const std::string& device) : agilent_com<com_method>(device) {};
      using agilent_com<com_method>::send;
      using agilent_com<com_method>::recv;
      using agilent_com<com_method>::apply;
      using agilent_com<com_method>::load;
      using agilent_com<com_method>::duty;
      using agilent_com<com_method>::frequency;
      using agilent_com<com_method>::voltage;
      using agilent_com<com_method>::offset;
      using agilent_com<com_method>::cycles;
      using agilent_com<com_method>::burst_on;
      using agilent_com<com_method>::burst_off;
      using agilent_com<com_method>::phase;
      using agilent_com<com_method>::burst_int;
      using agilent_com<com_method>::burst_int;
      using agilent_com<com_method>::burst_ext;
      using agilent_com<com_method>::trigger;
      using agilent_com<com_method>::trigger_now;
      using agilent_com<com_method>::shape;
      using agilent_com<com_method>::arb_wave;
      using agilent_com<com_method>::reset;
      using agilent_com<com_method>::error;
    };

    %template(WG33220A_ip) WG33220A<ICR::coms::IPmanager>;   
    %template(WG33220A_serial) WG33220A<ICR::coms::serial_manager>;
    
    template<class com_method>
    void 
    apply_waveform(agilent_com<com_method>* generator,const std::string& name, const double& sampling_freq, const  float* voltages, const unsigned long& num_points);
    
    %template(apply_waveform_ip)  apply_waveform<ICR::coms::IPmanager>; 
    %template(apply_waveform_serial)  apply_waveform<ICR::coms::serial_manager>; 
  }

}  

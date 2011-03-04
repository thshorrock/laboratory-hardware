#pragma once
#include "agilent_com_detail.hpp" 
//#include "agilent/vxi11_user.h"
#define BUF_LEN 100000

namespace ICR {
  /** exception namespace */
    namespace exception{
      /** Option not recognised */
      struct agilent_option_not_recognised{};
      /** Failed to reset */
      struct failed_to_reset_agilent{};
      /** Too few points */
      struct too_few_points{};
      /** To many points */
      struct too_many_points{};
      
      struct agilent_failed_to_send_cmd{};
    } 
    
  namespace agilent{
    
    
    typedef ICR::coms::IPmanager IP;
    typedef ICR::coms::serial_manager SERIAL;

    /** Waveform Shapes. */
    namespace shape{
      enum type {SIN,    //!< Sinusoid
		 SQUARE, //!< Square
		 PULSE,  //!< Pulse
		 TRIANGLE, //!< Triangular
		 RAMP,     //!<Ramp
		 NOISE,    //!<Noise
		 DC,       //!<DC 
		 USER      //!<USER
      };
    }
    /**Load options */
    namespace load{
      enum type {OHM50,  //!< 50 Ohm 
		 INFTY   //!< High resistance
      }; 
    }
    
    /** The trigger control options */
    namespace trigger{
      enum  type {IMMEDIATE, //!< Internal Triggering 
		  EXTERNAL,  //!< External Triggering 
		  BUS};      //!< Computer controlled triggering
    }
    
    /** The triggering edge option */
    struct edge{
      enum type {POSITIVE, //!< Positive Edge
		 NEGATIVE  //!< Negative edge
      };
    };

    /** The agilent communication class.
	@tparam com_method Either ICR::coms::serial_manager or ICR::coms::IPmanager */
    template<class com_method>
    class agilent_com  //:
    {
      bool m_burst_mode;
      com_method comm;
    public:
      /** Constructor.
       * @param address the address of the device. For example "COM1" for a serial device, or the IP address for a TCP connection.
       */
      agilent_com(const std::string& address);

      /** Destructor */
      virtual ~agilent_com() ;

      /** Send a command. 
       * @param cmd The command to send. These must be terminated with a new line character. 
       * @throws boost::system::system_error Something went wrong with the communication.
       */
      void send(const std::string& cmd)
	throw (boost::system::system_error) 
      {
	//std::cout<<"cmd = "<<cmd<<std::endl;
	
	bool send_successful = false;
	// size_t
	size_t  attempts = 0;
	while (!send_successful && attempts <10)
	  {
	
	    comm.send(cmd);
	    send_successful = true;
	    if (m_burst_mode) {
	      ICR::coms::sleep(50);
	      comm.send("*WAI\n");
	      detail::agilent_command_completed<com_method> complete(comm);
	      boost::thread thr(complete);
	      ICR::coms::sleep(50);
	      
	      if (!thr.timed_join(boost::posix_time::milliseconds(3000)))
		{
		  send_successful= false; 
		  ++attempts;
		  std::cout<<"opc signal not returned from agilent, attempts = "<<attempts <<std::endl;

		}
	     
	    }
	    ICR::coms::sleep(100);
	  }
	
	//com_method::send("*WAI\n");
	
      
	if (attempts==10) 
	  throw ICR::exception::agilent_failed_to_send_cmd();
      }
      /** Receive a response from the device.
       * @param cmd The command requesting a response.
       * @return The response 
       * @throws boost::system::system_error Something went wrong with the communication.
     */
      std::string recv(const std::string& cmd)
	throw(boost::system::system_error )
      {
	try{
	  std::string ret = comm.recv(cmd);
	  return ret;

	  if (m_burst_mode) {
	    comm.send("*WAI\n");
	    std::string opc_code = comm.recv("*OPC?\n");
	    //std::cout<<"opc_code = "<<opc_code<<std::endl;
	  }
	}
	catch(ICR::exception::exception_in_receive_you_must_resend_command& e) {
	  std::cout<<"AGILENT receive exception caught (in recv), resending command"<<std::endl;
	  recv(cmd);
	}
      }
      
      /** Get the error response of the scope.
       * @return The error message 
       */
      virtual std::string error();

      /** Apply the following set of parameters to the device.
       * @param shape The shape of the wave.
       * @param freq The frequency of the wave.
       * @param volts The voltage of teh wave.
       * @param offset The voltage offset of the wave.
       */	 
      virtual void apply(const shape::type shape, const double freq,const  double volts,const  double offset);
      
      /** The load of the terminal (eg. 50 Ohm).
       * @param load The requested load.
       */
      virtual void load(const load::type load);
      
      /** The duty cycle of the wave (square wave only).
       * @param dc The duty cycle as a percentage.
       */
      virtual void duty_cycle(const double); // square wave only

      /** Set the output to be on.  */
      virtual void turn_on();
      /** Set the output to be off.  */
      virtual void turn_off();

      /** Set the fequency.
       * @param frequency The frequency
       */
      virtual void frequency(const double frequency);

      /** Set the voltage.
       * @param V The voltage requested
       */
      virtual void voltage(const double V);
      
      /** Set the voltage offset.
       * @param offset The offset (in volts)
       */
      virtual void offset(const double offset);

      //burst commands
      /** Set the number of cycles in a burst.
       * @param cycles The number of cycles in a burst.
       */
      virtual void cycles(const unsigned int cycles);
      
      /** Turn the burst mode on.
       * It is best to do this after all the burst parameters have been set.
       */
      virtual void burst_on(void);

      /** Turn the burst mode off. */
      virtual void burst_off(void);

      /** Set the phase of the burst (in degrees).
       * @param degrees The phase in degrees.
       */
      virtual void phase(const double degrees);
      
      /** Set the burst with internal triggering.
       * @param rate The internal trigger rate in Hz.
       */
      virtual void burst_int(const  double  rate);
      /**Set the burst with external triggering.*/
      virtual  void burst_ext();
      /** Set the burst with external triggering and specify the edge on which to trigger.
       * @param edge The edge on which to trigger.
       */
      virtual  void burst_ext(const enum edge::type& edge);

      /** Set the trigger source.
       * @param source The trigger source.
       */
      virtual void trigger(const trigger::type source);

      /** Trigger now (if in bus mode).*/
      virtual void trigger_now() ;

      /** Set the shape of the wavefront.
       * @param shape The shape option to choose. */
      virtual void shape (const shape::type shape);
      
      /** Create an arbitrary wavefront.
       * @param name The name under which to save the wavefront.
       * @param data The data points (between -1 and 1).
       * @param size The number of data points 
       * @see apply_waveform
       * @see pulse_after_n_microseconds
       */
      virtual  void arb_wave(const std::string& name, const float* data, const unsigned long& size);
      //void arb_wave(vector<double> data);

      /** Reset the agilent */
      virtual void reset();

    };
    
    /** Apply a waveform to the agilent scope.
     * @tparam com_method The method of communicating with the scope
     * @param generator The agilent scope that you wish to apply the waveform to.
     * @param name The name of the wavefront
     * @param sampling_freq The sampling frequency of the temporal data.
     * @param voltages The data in volts
     * @param num_points The number of data points
     */
    template<class com_method>
    void 
    apply_waveform(agilent_com<com_method>* generator,const std::string& name, const double& sampling_freq, const  float* voltages, const unsigned long& num_points);


    /** Create a waveform that is a pulse after n microseconds.
     * @param gen The agilent that you would like to apply this method to.
     * @param delay The delay until the pulse
     * @param duration The duration of the pulse
     * @param voltage The voltage of the pulse
     * @attention the number of points in an arbitrary waveform is at most 16000. This limits the rise-time of the pulse
     * for pulses that have a large temporal offset.
     */     
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

    
    /** Create a waveform that is a sin after n microseconds.
     * @attention the number of points in an arbitrary waveform is at most 16000. This limits the rise-time of the pulse
     * for pulses that have a large temporal offset.
     */     
    template<class com_method>
    void
    sin_after_n_microseconds(agilent_com<com_method>* gen, double frequency,  double delay, double voltage = 2, size_t cycles = 10 )
    {
      //max samples = 16000
      unsigned long samples = 16000;
      double duration = cycles/frequency;
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
	  data[i] = voltage*std::sin(2*M_PI*frequency*1.0e6*temp_res*i);
	  //std::cout<<"data["<<i<<"]="<<data[i]<<std::endl;
	}
	else {
	  data[i] = 0;
	  // std::cout<<"data["<<i<<"]="<<data[i]<<std::endl;
	}
      }
      data[samples-1]=0;
      //std::cout<<"data["<<samples-1<<"]="<<data[samples-1]<<std::endl;
      
      apply_waveform<com_method>(gen,"sin_delay",freq_res, data.get(),samples);


    }

    /** Create a waveform that is a sin after n microseconds.
     * @attention the number of points in an arbitrary waveform is at most 16000. This limits the rise-time of the pulse
     * for pulses that have a large temporal offset.
     */     
    template<class com_method>
    void
    two_sin_with_n_microsecond_gap(agilent_com<com_method>* gen, double frequency,  double gap, double voltage = 2, size_t cycles = 10 )
    {
      //max samples = 16000
      unsigned long samples = 65536;
      double duration = cycles/frequency;
      std::cout<<"duration = "<<duration<<std::endl;

      //max temp resolution = ((delay + duration)/16000 )e-6 seconds
      //last point = 0 to make it pulse so have only 15999 points
      double temp_res = ((gap+2.*duration)/double(samples-1))*1e-6; //seconds
      double freq_res = 1.0/temp_res;
      // if (freq_res >15e6) // round to 15MHz
      // 	freq_res = 15e6;
      // temp_res = 1.0/freq_res;
      samples = (gap+2*duration)/(temp_res*1e6)+1;
      std::cout<<"samples= "<<samples<<std::endl;
      boost::shared_array<float> data(new float[samples]);
      std::cout<<"temp_res = "<<temp_res<<std::endl;
      size_t j = 0;
      for(size_t i=0;i<(samples-1);++i){
	if (i*temp_res*1e6 < duration){
	  data[i] = voltage*std::sin(2*M_PI*frequency*1.0e6*temp_res*i);
	  // std::cout<<"data["<<i<<"]="<<data[i]<<std::endl;

	}
	else if ((i)*temp_res*1e6<(gap+duration)) {
	  data[i] = 0;
	}
	// else if (extra < 2){
	//   data[i] = 0;
	//   ++extra;
	// } 
	else if  (i*temp_res*1e6<(gap+2*duration)){
	  data[i-2] = voltage*std::sin(2*M_PI*frequency*1.0e6*temp_res*j);
	  ++j;
	  
	  // std::cout<<"data["<<i<<"]="<<data[i]<<std::endl;
	}
	else{
	  std::cout<<"end zreo"<<std::endl;

	  data[i] = 0;
	}
      }
      data[samples-1]=0;
      data[0]=0;
      //std::cout<<"data["<<samples-1<<"]="<<data[samples-1]<<std::endl;
      std::cout<<"applying two sin waveform"<<std::endl;

      apply_waveform<com_method>(gen,"sin_gap",freq_res, data.get(),samples);


    }

 
    /** Create a waveform that is a sin after n microseconds.
     * @attention the number of points in an arbitrary waveform is at most 16000. This limits the rise-time of the pulse
     * for pulses that have a large temporal offset.
     */     
    template<class com_method>
    void
    n_microseconds_after_sin(agilent_com<com_method>* gen, double frequency,  double delay, double voltage = 2, size_t cycles = 10 )
    {
      //max samples = 16000
      unsigned long samples = 16000;
      double duration = cycles/frequency;
      //max temp resolution = ((delay + duration)/16000 )e-6 seconds
      //last point = 0 to make it pulse so have only 15999 points
      double temp_res = ((delay+duration)/(samples))*1e-6; //seconds
      double freq_res = 1.0/temp_res;
      // if (freq_res >15e6) // round to 15MHz
      // 	freq_res = 15e6;
      // temp_res = 1.0/freq_res;
      samples = (delay+duration)/(temp_res*1e6);
      std::cout<<"samples= "<<samples<<std::endl;

      boost::shared_array<float> data(new float[samples]);
      std::cout<<"temp_res = "<<temp_res<<std::endl;

      for(size_t i=0;i<(samples-1);++i){
	if (i*temp_res*1e6<(duration)) {
	  data[i] = voltage*std::sin(2*M_PI*frequency*1.0e6*temp_res*i);
	  //std::cout<<"data["<<i<<"]="<<data[i]<<std::endl;
	}
	else {
	  data[i] = 0;
	}
      }
      //std::cout<<"data["<<samples-1<<"]="<<data[samples-1]<<std::endl;
      
      apply_waveform<com_method>(gen,"sin_delay",freq_res, data.get(),samples);


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
      : comm(IPaddress, "5025"),
	m_burst_mode(false)
    {}

    template<>
    inline  agilent_com<coms::serial_manager>::agilent_com(const std::string& port)
      : comm(port, 9600, ICR::coms::flow_control::hardware,ICR::coms::parity::none,ICR::coms::stop_bits::two,8),
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

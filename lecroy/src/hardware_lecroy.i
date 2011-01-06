
%module hardware_lecroy
%include stl.i
%include "std_string.i"
%include "carrays.i"
%array_class(float, floatArray);
%array_class(double, doubleArray);

%include "numpy.i"
/* %init %{ */
/* import_array(); */
/* %} */

%apply (double* INPLACE_ARRAY1, int DIM1) {(double* data, int n)};
//%apply (double* INPLACE_ARRAY1, int DIM1) {(double* data1, int n1), (double* data2, int n2)};
//%apply (double* INPLACE_ARRAY1, int DIM1) {(double* data, int n)};
 //%apply const std::string& {std::string* foo};
%{
#include "lecroy/lecroy_com_manager.hpp"
#include "lecroy/lecroy_com.hpp"
#include "lecroy/lecroy_file.hpp"
#include "lecroy/lecroy_header.hpp"


/* Include in the generated wrapper file */
//typedef unsigned int size_t;
%}



/* Tell SWIG about it */
typedef unsigned int size_t;


namespace ICR{

  namespace lecroy{
    
    
    
    class lecroy_header{
    public:
      lecroy_header();
      
      void operator>>(const std::string& header);
      
      void decode_string(const std::string& header);

      operator const std::string();
      
      ~lecroy_header();
    };
    
    class lecroy_file
    {
    public:
      lecroy_file(const lecroy_header& lh);
      void set_data1( const double* data1);
    
      void set_data2( const double* data2);
      //double* get_data1() const;
      void get_data1(double* data1, int n);
      double* get_data2() const;
      
      
      const double& operator[](const size_t& i) const ;
      double& operator[](const size_t& i);
      const double operator()(const size_t& i) const ;
      double& operator()(const size_t& i);
      
      
      %extend {
	void setitem(const size_t& index, const double& value)
	 {
	   $self->operator[](index) = value; 
	 }
	double getitem(int index){return $self->operator[](index);} 
	
      }
      
      size_t size1() const;
      size_t size2() const;
    };

    namespace exception{ 
      /** Option not recognised. */
      struct option_not_recognised{};
      
      /** Scope failed to reset. */
      struct failed_to_reset{};
      
      /**Channel value not valid. */
      struct channel_value_not_valid{}; 

      /** store mode not valid. */
      struct store_mode_not_valid{};  
      
      /** store format not valid. */
      struct store_format_not_valid{};
      
      /** coupling option not recognised. */
      struct coupling_option_not_recognised{};  
      
      /**trigger mode not recognised. */
      struct trigger_mode_not_recognised{};

      /**  trigger state not recognised. */
      struct trigger_state_not_recognised{}; 
      
      /**trigger type not recognised. */
      struct trigger_type_not_recognised{};  
      
      /**  trigger hold type not recognised. */
      struct trigger_hold_type_not_recognised{};

      /** Scope calibration failed. */
      struct calibration_failed_exception{}; 

      /** Invalid memory option.*/
      struct invalid_memory_option{};

      /** More than 160 characters in the waveform description */
      struct waveform_description_too_long{};
    } 

    template<typename coms_method>
    class lecroy_com_manager : com_method {

    public:
      lecroy_com_manager(const std::string& IPaddress);
      void send(const std::string& cmd);

      std::string recv(const std::string& cmd, const unsigned int& buffsize = 128) ;
      std::string timed_recv(const std::string& cmd, const unsigned int& buffsize = 128, const double& seconds = 5) ;
      
      void clear();
      //std::string serial_poll_byte() ;
    };

    %template(lecroy_com_manager_ip) lecroy_com_manager< ICR::coms::IPmanager>;       
    %template(lecroy_com_manager_serial) lecroy_com_manager<ICR::coms::serial_manager>; 
 
    

    namespace location{
      enum{C1,C2,C3,C4,F1,F2,F3,F4,F5,F6,F7,F8,M1,M2,M3,M4,ALL_DISPLAYED, EX, EX10,ETM10, FILE, HDD};
    }
    namespace store_mode{
      enum {NO_AUTOSAVE, FILL, WRAP};
    }
    namespace store_format{
      enum {ASCII, BINARY, EXCEL, MATHCAD,MATLAB};
    }

    
    namespace coupling{
      enum {DC, AC, A1M, D1M, D50, GND};
    }

    namespace trigger_mode{
      enum {AUTO, NORM, SINGLE, STOP};
    }
    // namespace trigger_state{
    //   enum {LOW, HIGH};
    //   std::string get_string(const int&);
    // }
    namespace trigger_type{
      enum {DROPOUT, EDGE, GLITCH, INTERVAL, STD,SINGLE_SOURCE ,STATE_QUALIFIED,EDGE_QUALIFIED };
    }
    namespace trigger_hold_type{
      enum {TIME_GREATER_THAN, TIME_WIDTH, EVENTS,
	    PULSE_SMALLER_THAN, PULSE_GREATER_THAN, 
	    INTERVAL_SMALLER_THAN, INTEVAL_GREATER_THAN, 
	    PULSE_WIDTH,INTERVAL_WIDTH, NO_HOLD}; //,
    }
    
    typedef ICR::coms::IPmanager IP;
    typedef ICR::coms::serial_manager SERIAL;

    template <class coms_method>
    class lecroy_com : public lecroy_com_manager<coms_method> {

    public:
      lecroy_com(const std::string& IPaddress);
      virtual void send(const std::string& cmd);

      std::string recv(const std::string& cmd, const unsigned int& buffsize = 128) ;
      virtual void set_coupling(const int& channel,const int& coupling);
      virtual void set_timebase(const double& timebase);  //in seconds
      virtual void arm();
      virtual void auto_calibrate(const bool& b);
      virtual void calibrate();
      virtual void beep();
      virtual void clear_memory(const int i);
      virtual void set_date();
      virtual void aquire_single();
      virtual void message(const std::string& message);
      virtual void set_vertical_offset(const int& channel, const double& offset);
      virtual void vertical_offset_constant(const bool& b);
      virtual void persistence(const bool& b);
      virtual void reset();
      virtual void sequence(const int& segments, const double& max_samples);
      virtual void stop();
      virtual void store(const int& trace, const int& dest);
      virtual void store_setup(const int& trace, const int& dest, const int& mode, const int& type
		       );
      virtual void toggle_trace(const int& trace);
      virtual void trace_on(const int& trace) ;
      virtual void trace_off(const int& trace);
      //      virtual void transfer_file(const std::string& filename);
      virtual void trigger_coupling(const int& channel, const int& coupling);
      virtual void trigger_delay(const double& delay);// negive numbers for post trigger
      virtual void trigger_level(const int& channel, const double& level);
      virtual void trigger_mode(const int& mode);
      virtual void trigger_pattern(const std::string& pattern);
      virtual void trigger_select(const int& type,const int& source,const int& hold_type, const double& hold_value1, const double& hold_value2=0.0);
      virtual void trigger_positive_edge(const int& channel);
      virtual void trigger_negative_edge(const int& channel);
      virtual void volts_per_div(const int& channel,const double& v);
      virtual void wait() ;
      virtual lecroy_file get_waveform(const int& channel); 
      virtual std::string get_waveform_text(const int& channel);
      virtual void waveform_text(const int& channel,const std::string& text) 
	throw(exception::waveform_description_too_long);
      virtual void display(const bool& do_display);
    };
    
    
    %template(lecroy_com_ip) lecroy_com< ICR::coms::IPmanager>;       // Rename to a suitable identifier
    %template(lecroy_com_serial) lecroy_com<ICR::coms::serial_manager>;       // Rename to a suitable identifier
 
    
    class lecroy_64Xi : public lecroy_com<ICR::coms::IPmanager>
    {
    public:
      lecroy_64Xi(const std::string device) : lecroy_com<ICR::coms::IPmanager>(device) {}
      using lecroy_com<ICR::coms::IPmanager>::send;
      using lecroy_com<ICR::coms::IPmanager>::recv;
      using lecroy_com<ICR::coms::IPmanager>::set_coupling;
      using lecroy_com<ICR::coms::IPmanager>::set_timebase;
      using lecroy_com<ICR::coms::IPmanager>::arm;
      using lecroy_com<ICR::coms::IPmanager>::auto_calibrate;
      using lecroy_com<ICR::coms::IPmanager>::calibrate;
      using lecroy_com<ICR::coms::IPmanager>::beep;
      using lecroy_com<ICR::coms::IPmanager>::clear_memory;
      using lecroy_com<ICR::coms::IPmanager>::clear_sweeps;
      using lecroy_com<ICR::coms::IPmanager>::clear;
      using lecroy_com<ICR::coms::IPmanager>::set_date;
      using lecroy_com<ICR::coms::IPmanager>::aquire_single;
      using lecroy_com<ICR::coms::IPmanager>::message;
      using lecroy_com<ICR::coms::IPmanager>::set_vertical_offset;
      using lecroy_com<ICR::coms::IPmanager>::vertical_offset_constant;
      using lecroy_com<ICR::coms::IPmanager>::persistence;
      using lecroy_com<ICR::coms::IPmanager>::reset;
      using lecroy_com<ICR::coms::IPmanager>::sequence;
      using lecroy_com<ICR::coms::IPmanager>::stop;
      using lecroy_com<ICR::coms::IPmanager>::store;
      using lecroy_com<ICR::coms::IPmanager>::store_setup;
      using lecroy_com<ICR::coms::IPmanager>::toggle_trace;
      using lecroy_com<ICR::coms::IPmanager>::trace_on;
      using lecroy_com<ICR::coms::IPmanager>::trace_off;
      using lecroy_com<ICR::coms::IPmanager>::trigger_coupling;
      using lecroy_com<ICR::coms::IPmanager>::trigger_delay;
      using lecroy_com<ICR::coms::IPmanager>::trigger_level;
      using lecroy_com<ICR::coms::IPmanager>::trigger_mode;
      using lecroy_com<ICR::coms::IPmanager>::trigger_pattern;
      using lecroy_com<ICR::coms::IPmanager>::trigger_negative_edge;
      using lecroy_com<ICR::coms::IPmanager>::volts_per_div;
      using lecroy_com<ICR::coms::IPmanager>::wait; 
      using lecroy_com<ICR::coms::IPmanager>::get_waveform;
      using lecroy_com<ICR::coms::IPmanager>::get_waveform_text;
      using lecroy_com<ICR::coms::IPmanager>::waveform_text;
      using lecroy_com<ICR::coms::IPmanager>::display;
    };

  }

}  
%extend ICR::lecroy::lecroy_file
{
  double __getitem__(unsigned int i)
  {
    return (*self)[i];
  };

  void  __setitem__(unsigned int i, double value)
  {
    (*self)[i] = value;
  };

  
};

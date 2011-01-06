
%module hardware_DPR500
%include stl.i
%include "std_string.i"
%include "carrays.i"
 /* %include "boost_shared_ptr.i" */
 /* %shared_ptr(pulser::RPL2) */
%array_class(float, floatArray);
//%apply const std::string& {std::string* foo};
%{
#include "DPR500/DPR500.hpp"
#include "DPR500/pulser_receivers.hpp"
#include "DPR500/exceptions.hpp"

  /* Include in the generated wrapper file */
  //typedef unsigned int size_t;
  %}



/* Tell SWIG about it */
typedef unsigned int size_t;


namespace ICR{

  namespace pulser{
 
    namespace exceptions{
      struct DPR500_exception{};
      /** Invalid Query Exception. */
      struct invalid_query :  public DPR500_exception {};
      /** Failed to contact the DPR500 device. */
      struct failed_to_contact_DPR500_device : public DPR500_exception {};
      /** pulser receiver not attached to the  DPR500 device. */
      struct pulser_receiver_not_attached_to_DPR500 : public DPR500_exception {};
      /** Incorrect input to gain command */
      struct incorrect_input_to_gain_command : public DPR500_exception {};
      /** Requested gaind out of bounds */
      struct requested_gain_out_of_bounds : public DPR500_exception {};
    }
      
    namespace channel{
      enum type {A, B};
    }
    

    struct damping_RPL2{
      enum type {threehundredthirty_Ohm,onehundredfour_Ohm,fourtyfour_Ohm,thirtyfour_Ohm};
    };

    struct damping_RPU2{
      enum type {onehundred_Ohm,fifty_Ohm,twentyone_Ohm,seventeen_Ohm};
    };
    
    struct energy_policy{
      enum type{LOW, HIGH};
    };
    struct receive_mode{
      enum type{PULSE_ECHO, RECEIVE_ONLY};
    };
      
    /** The filter Namespace */
    namespace filter{
    class HPFA{
      public:
	enum type {ten_KHz, one_MHz, twopointfive_MHz, five_MHz, sevenpointfive_MHz};
      };

      class LPFA{
      public:
      	enum type {three_MHz,sevenpointfive_MHz, ten_MHz, fifteen_MHz, twentytwopointfive_MHz, fifty_MHz};
      };

      class HPFB{
      public:
	enum type {   five_MHz,thirty_MHz};
      };


      class LPFB{
      public:
      	enum type {fivehundred_MHz,threehundred_MHz};
      };
    }
    
    class pulser_receiver
    {
    public:
      pulser_receiver();

      /** Let the DPR500 access these methods. */
      friend class pulser::DPR500;
      
      /** If attached to the DPR500, return the channel character @return the channel. */
      virtual char get_channel_char() const ;
      
      /** If attached to the DPR500, return the address character @return the address. */
      virtual char get_address_char() const ;
      virtual short get_gain() const = 0;
      virtual std::string get_serial_number() const = 0;
    };


    class  RPL2 : public pulser_receiver
    {
      
    protected:
      RPL2(); //protected constructor.
    public:
      void set_pulser_mode(const enum damping_RPL2::type&, 
			   const enum energy_policy::type&,
			   const enum receive_mode::type&);
      double set_prf (const double& frequency);
      void set_internal_trigger();
      void set_external_trigger();
      void set_master();
      void set_slave() ;
      void set_voltage(const unsigned short& voltage);
    };


    class  RPU2 : public pulser_receiver
    {
      
    protected:
      RPU2(); //protected constructor.
    public:
      void set_pulser_mode(const enum damping_RPU2::type&, 
			   const enum energy_policy::type&);
      double set_prf (const double& frequency);
      void set_internal_trigger();
      void set_external_trigger();
      void set_master();
      void set_slave() ;
    };


  
    class PulserReceiverFactory
    {
    public:
      typedef boost::shared_ptr<pulser_receiver> pulser_receiver_ptr; 
      
      pulser::RPL2* make_RPL2() const ;

      pulser::RPU2* make_RPU2() const ;


    };
    
    class DPR500 :  public ICR::coms::serial_manager
      {
      public:
	DPR500(const std::string& port) throw(exceptions::failed_to_contact_DPR500_device);
      
	void notify();

	void blink(const short& speed = 100);
	void attach_A(pulser_receiver* p);
	void attach_B(pulser_receiver* p);
	/** Detach the pulser/receiver from channel A. */
	void detach_A();
	/** Detach the pulser/receiver from channel B. */
	void detach_B();
	void turn_on_A() ;
	void turn_off_A();
	void turn_on_B() ;
	void turn_off_B();

	void set_gain_A(const short& g);
	void set_high_pass_filter_A(const enum filter::HPFA::type& v);
	void set_low_pass_filter_A (const enum filter::LPFA::type& v);
      
	void set_gain_B(const short& g);
	void set_high_pass_filter_B(const enum filter::HPFB::type& v);
	void set_low_pass_filter_B (const enum filter::LPFB::type& v);
	// void impedance();
	// void energy();
	// void echo();
	// void gain(const enum channel::type& c, const unsigned int& gain);
	// void triggering(const channel::type& c,const trigger::type t);
	// void on(const channel::type& c);
	// void off(const channel::type& c);
      
      };
  
  }

}  

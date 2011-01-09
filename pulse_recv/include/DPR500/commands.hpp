#pragma once
#include "enums.hpp"
#include "filters.hpp"
#include "stringify.hpp"
#include <boost/smart_ptr.hpp>
#include <boost/array.hpp>
#include <iostream>
#include <string>
#include <bitset>

/** The Institute of Cancer Research Namespace */
namespace ICR{

  /** The pulse-receiver Namespace */
  namespace pulser{
    //forward ref
    class pulser_receiver;

    /** The command Namespace */
    namespace command{
      
      /** The command interface.
       * This abstract class defines the interface to the basic command 
       * that issues information without requesting a reply.
       */
      struct cmd
      {
	/** The implicit conversion from the class to a std::string */
	virtual operator const std::string() = 0;
      };

      /** The receive command interface.
       * This abstract class defines the interface to the basic command 
       * that issues information that requests a reply.
       */
      struct recv_cmd 
      {
	virtual operator const std::string() = 0;
	/** The length of the expected reply. @return The length in bytes. */
	virtual size_t reply_size() const = 0;
	/** Decode the reply. @return The useful information emitted from the DPR500. */
	virtual std::string decode(const std::string&) = 0;
      };
      
      /** The Pulser/Receiver command interface.
       * This abstract class defines the interface to a command 
       * that affects an individual pulser/receiver rather than the DPR500 box itself.
       * For example, a command to control the gain.
       */
      class PulserReceiver_cmd : public recv_cmd
      {
      protected:
      	std::string m_cmd;
      	char m_channel_char;
      	char m_address;
	pulser_receiver* m_pr; //reference to pulser-receiver that owns this gain command.
	virtual void update();
	virtual void set_pulser_receiver(pulser_receiver* p){m_pr = p;}
	virtual void set_cmd() = 0;
      public:
	/** Constructor */
	PulserReceiver_cmd(); 
	/* allow update_cmd access to the update method. 
	 * @param command The command to be updated. */
	friend void update_cmd(boost::shared_ptr<command::PulserReceiver_cmd>& command);
	friend class ICR::pulser::pulser_receiver;
	virtual operator const std::string() {return m_cmd;}
      };
      
      /** Update of of the pulser / receiver commands. 
       * @param command The command to be updated.
       */
      inline void update_cmd(boost::shared_ptr<command::PulserReceiver_cmd>& command){command-> update();};

      /** The DPR500 command interface.
       * This abstract class defines the interface to commands
       * that affects an DPR500 box itself that do not expect a reply.
       * For example, the blinking of the power button.
       */
      struct DPR500_cmd : public cmd
      {};


      /** The DPR500 command interface.
       * This abstract class defines the interface to commands
       * that affects an DPR500 box itself that  expect a reply.
       */
      struct DPR500_recv_cmd : public recv_cmd
      {};


      /** Control the blinking of the power button on the DPR500.*/
      class blink : public DPR500_recv_cmd
      {
	std::string m_cmd;
      public:
	/** Constructor. 
	 * @param address The address of the device.
	 * @param speed The blink speed. Must be between 50 and 200. */
	blink(const char& address, const short& speed = 100);
	size_t reply_size() const {return 4;}
	std::string decode(const std::string& ans){return  stringify((int) *ans.substr(3,1).c_str());}
	operator const std::string() {return m_cmd;};
      };

      
      /** Command that initates an address assignment phase of the DPR500.*/
      class initiate_address_assignment : public DPR500_cmd{
	std::string m_cmd;
      public:
	/** Constructor */
	initiate_address_assignment();
	operator const std::string() {return m_cmd;};
      };
      /** Command that ends the address assignment phase of the DPR500.*/
      class end_address_assignment : public DPR500_cmd{
	std::string m_cmd;
	void set_cmd(const char& address);
      public:
	/** Constructor */
	end_address_assignment(const char& address = 0);
	operator const std::string() {return m_cmd;};
      };

      /** Command that sets the address of the DPR500.*/
      class set_address: public DPR500_cmd{
	std::string m_cmd;
      public:
	/** Constructor */
	set_address(const char&);
	operator const std::string() {return m_cmd;};
      };

      /** Command that requests the address  of the DPR500.*/
      class instrument_address : public DPR500_recv_cmd{
	std::string m_cmd;
      public:
	/** Constructor */
	instrument_address();
	operator const std::string() {return m_cmd;};
	size_t reply_size() const {return 6;}
	std::string decode(const std::string&);
      };
      
      /** Command that requests the instrument type.*/
      class instrument_type : public DPR500_recv_cmd{
	std::string m_cmd;
      public:
	/** Constructor */
	instrument_type();
	operator const std::string() {return m_cmd;};
	size_t reply_size() const {return 9;}
	std::string decode(const std::string&);
      };

      /** Command that requests the serial number of the device.*/
      class instrument_serial_number : public DPR500_recv_cmd{
	std::string m_cmd;
      public:
	/** Constructor */
	instrument_serial_number();
	operator const std::string() {return m_cmd;};
	size_t reply_size() const {return 9;}
	std::string decode(const std::string&);
      };


      /** Command that requests the firmware of the device.*/
      class instrument_firmware : public DPR500_recv_cmd{
	std::string m_cmd;
      public:
	/** Constructor */
	instrument_firmware();
	operator const std::string() {return m_cmd;};
	size_t reply_size() const {return 6;}
	std::string decode(const std::string&);
      };


      /** Command that requests the serial number of the DPR500.*/
      class DPR500_serial_number : public DPR500_recv_cmd{
	std::string m_cmd;
      public:
	/** Constructor */
	DPR500_serial_number();
	operator const std::string() {return m_cmd;};
	size_t reply_size() const {return 9;}
	std::string decode(const std::string&);
      };

	
      // // class I_command{
      // // 	std::string m_cmd;
      // // public:
      // // 	I_command(const  char& i);
      // // 	operator const std::string() {return m_cmd;};
      // // };

      
      // // class A_command{
      // // 	std::string m_cmd;
      // // public:
      // // 	A_command(const char& address) ;
      // // 	operator const std::string() {return m_cmd;};
      // // };

      // // class E_command{
      // // 	std::string m_cmd;
      // // public:
      // // 	E_command(const char& address) ;
      // // 	operator const std::string() {return m_cmd;};
      // // };
      // // class Function_command{
      // // 	std::string m_cmd;
      // // public:
      // // 	Function_command(const char& address, const char& command, const char& value, const char & channel = 0x00);
	
      // // 	operator const std::string() {return m_cmd;};

      // // };
      // class Query_command{
      // 	std::string m_cmd;
      // public:
      // 	Query_command(const char& address, const char& command,  const  char& channel);
      // 	operator const std::string() {return m_cmd;};

      // };
      
      // class QueryI_command{
      // 	std::string m_cmd;
      // public:
      // 	QueryI_command(const char& address, const char& command, const char& value, const char& channel, const char& channel_option);
      // 	operator const std::string() {return m_cmd;};

      // };
      
      
      /** Command that requests the serial number of the pulser.*/
      class pulser_serial_number :  public  DPR500_recv_cmd
      {

	std::string m_cmd;
	char m_address, m_channel_char;

      protected:
      	void set_cmd();
      public:
	/** Constructor.
	 *   @param addr The address of the DPR500.
	 *   @param channel The channel of the DPR500 specified by the address */
      	pulser_serial_number(const char& addr, const enum channel::type& channel);
	operator const std::string() {return m_cmd;};
      	virtual size_t reply_size() const {return 9;};
      	virtual std::string decode(const std::string&);
      };

      /** Command that sets the gain of the DPR500 channel.*/
      class gain :  public  DPR500_recv_cmd
      {
      	
	std::string m_cmd;
	
      	short m_measured, m_min, m_max,  m_step;
      	char m_gain_char;
	char m_address, m_channel_char;

      protected:
      	void set_cmd();
      public:
	/** Constructor.
	 *   @param addr The address of the DPR500.
	 *   @param channel The channel of the DPR500 specified by the address
	 *   @param measured The measured preamp gain
	 *   @param min The min gain
	 *   @param max The max gain
	 *   @param step The step size of the gain 
	 */
      	gain(const char& addr, const enum channel::type& channel,const short& measured, const short& min, const short& max, const short step = 1);
      	virtual void set(const short& gain);
	operator const std::string() {return m_cmd;};
      	virtual size_t reply_size() const {return 5;};
      	virtual std::string decode(const std::string&);
      };

	
      /** Command that sets the high pass filter on the DPR500. */
      template<class filter_option>
      class high_pass_filter : public  DPR500_recv_cmd
      {	
	std::string m_cmd;
	char m_filter_char;
	filter_option m_option;
	char m_address, m_channel_char;
      protected:
	void set_cmd()
	{
	  m_cmd.clear();
	  m_cmd.push_back(m_address);
	  m_cmd.push_back(0x01); 
	  m_cmd.push_back(0x68); //h
	  m_cmd.push_back(m_filter_char);
	  m_cmd.push_back(m_channel_char);
	  m_cmd.push_back(0x00);
	}
	  
      public:
	high_pass_filter(const char& addr, const  enum channel::type&  c) 
	  : DPR500_recv_cmd(),
	    m_filter_char(0x00),
	    m_option(),
	    m_address(addr)
	{
	  if ( c == channel::A) 
	    m_channel_char =  0x41;
	  else
	    m_channel_char =  0x42;
	  set_cmd();
	}
    
	operator const std::string() {return m_cmd;};
	void set(const enum filter_option::type& option)
	{
	  m_filter_char = char(int(option));
	  set_cmd();
	}

	size_t reply_size() const {return 5;}
	virtual std::string decode(const std::string& ans){return stringify((int) *ans.substr(3,1).c_str());}
      };
      
    	
      /** Command that sets the low pass filter on the DPR500. */
      template<class filter_option>
      class low_pass_filter : public  DPR500_recv_cmd
      {	
	std::string m_cmd;
	char m_filter_char;
	filter_option m_option;
	char m_address, m_channel_char;
      protected:
	void set_cmd()
	{
	  m_cmd.clear();
	  m_cmd.push_back(m_address);
	  m_cmd.push_back(0x01); 
	  m_cmd.push_back(0x6c); //l
	  m_cmd.push_back(m_filter_char);
	  m_cmd.push_back(m_channel_char);
	  m_cmd.push_back(0x00);
	}
	  
      public:
	low_pass_filter(const char& addr, const enum channel::type&  c) 
	  : DPR500_recv_cmd(),
	    m_filter_char(0x00),
	    m_option(),
	    m_address(addr)
	{
	  if ( c == channel::A) 
	    m_channel_char =  0x41;
	  else
	    m_channel_char =  0x42;
	  set_cmd();
	}
    
	operator const std::string() {return m_cmd;};
	void set(const enum filter_option::type& option)
	{
	  m_filter_char = char(int(option));
	  set_cmd();
	}

	size_t reply_size() const {return 5;}
	virtual std::string decode(const std::string& ans){return stringify((int) *ans.substr(3,1).c_str());}
      };
      


      /** Command that sets the master / slave triggering on the pulser receiver .*/
      class master  : public PulserReceiver_cmd
      {
      	bool m_master;
      protected:
      	void set_cmd();
      public:
      	master(const bool& is_master);
      	void set(const bool& is_master);
      	size_t reply_size() const {return 5;};
      	virtual std::string decode(const std::string& ans){return stringify((int) *ans.substr(3,1).c_str());}
      };

      /** Command turns on a channel on the DPR500 */
      class turn_on :  public DPR500_recv_cmd
      {
	std::string m_cmd;
	char m_address, m_channel_char;
      protected:
      	void set_cmd();
      public:
      	turn_on(const char& addr, const enum channel::type&  channel);
      	size_t reply_size() const {return 5;};
      	virtual std::string decode(const std::string& ans){ return stringify((int) *ans.substr(3,1).c_str());};
	operator const std::string() {return m_cmd;};
      };

      /** Command turns off a channel on the DPR500 */
      class turn_off : public DPR500_recv_cmd
      {
	std::string m_cmd;
	char m_address, m_channel_char;
      protected:
      	void set_cmd();
      public:
      	turn_off(const char& addr, const enum channel::type&  channel);
      	size_t reply_size() const {return 5;};
      	virtual std::string decode(const std::string& ans){return stringify((int) *ans.substr(3,1).c_str());}
	operator const std::string() {return m_cmd;};
      };



      /** Command sets the pulse repetition frequency on the pulser  */
      class prf : public PulserReceiver_cmd
      {
      	boost::array<char,2> m_prf;
	unsigned short m_max_prf;
      protected:
      	void set_cmd();
      public:
      	prf(const unsigned short& max_prf);
	/** Set the requested frequency.
	 * @warning The requested frequency will be close to, 
	 * but not (in general) equal to the frequency emitted by ther pulser/receiver. 
	 * See the returned value for the actual frequency set.
	 * @param frequency The requested frequenc.
	 * @return The frequency actually sent to the pulser/receiver.
	 */
      	double set(const double& frequency);
      	size_t reply_size() const {return 6;};
      	virtual std::string decode(const std::string&);
      };


      /** Command sets external triggering on a pulser  */
      class external_trigger : public PulserReceiver_cmd
      {
      protected:
      	void set_cmd();
      public:
      	external_trigger();
      	size_t reply_size() const {return 5;};
      	virtual std::string decode(const std::string& ans){return stringify((int) *ans.substr(3,1).c_str());}
      };

      /** Command sets internal triggering on a pulser  */
      class internal_trigger : public PulserReceiver_cmd
      {
      protected:
      	void set_cmd();
      public:
      	internal_trigger();
      	size_t reply_size() const {return 5;};
      	virtual std::string decode(const std::string& ans){return stringify((int) *ans.substr(3,1).c_str());}
      };


      /** Command sets the voltage on a pulser  */
      class voltage : public PulserReceiver_cmd
      {
      	boost::array<char,2> m_voltage;
      	double m_min, m_max;
      protected:
      	void set_cmd();
      public:
      	voltage(const double& min, const double& max);
      	void set(const double& v);
      	size_t reply_size() const {return 6;};
      	virtual std::string decode(const std::string& ans);
      };


      /** Command that queries the voltage on a pulser  */
      class query_voltage : public PulserReceiver_cmd
      {
      	boost::array<char,2> m_voltage;
      	double m_min, m_max;
      protected:
      	void set_cmd();
      public:
      	query_voltage();
      	size_t reply_size() const {return 6;};
      	virtual std::string decode(const std::string& ans);
      };


      /** Command that sets the pulser mode */
      template<class damping_policy>
      class pulser_mode : public PulserReceiver_cmd
      {
	char m_pulser_mode;
      	double m_min, m_max;
      protected:
      	void set_cmd()
	{
	  PulserReceiver_cmd::m_cmd.clear();
	  PulserReceiver_cmd::m_cmd.push_back(m_address);
	  PulserReceiver_cmd::m_cmd.push_back(0x01); 
	  PulserReceiver_cmd::m_cmd.push_back(0x66); //f
	  PulserReceiver_cmd::m_cmd.push_back(m_pulser_mode);
	  PulserReceiver_cmd::m_cmd.push_back(m_channel_char);
	  PulserReceiver_cmd::m_cmd.push_back(0x00);
	}
      public:
      	pulser_mode()
	  : PulserReceiver_cmd(),
	    m_pulser_mode(0x00)
	{}
      	void set (const enum damping_policy::type& damping, 
		  const enum energy_policy::type&  energy,
		  const enum receive_mode::type&  receiver)
	{
	  
	  std::bitset<8> bits; bits.reset();
	  std::bitset<2> bits_dp(damping); 
	  std::bitset<1> bits_ep(energy); 
	  std::bitset<1> bits_rp(receiver);

	  bits.set(4, bits_dp.test(0));
	  bits.set(5, bits_dp.test(1));
	  bits.set(6, bits_rp.test(0));
	  bits.set(7, bits_ep.test(0));
	  
	  // char dp = (char) damping;
	  // char ep = (char) energy;
	  // char rp = (char) receiver;
	  // char cmd = 0x00;
	  // cmd |= (dp<<4);
	  // cmd |= (ep<<1);
	  // cmd |= (rp<<2);
	  m_pulser_mode = char(bits.to_ulong());
	  //round value to nearest integer
	  set_cmd();
	}
      	size_t reply_size() const {return 5;};
      	virtual std::string decode(const std::string& ans)
	{
	  char a = *ans.substr(3,1).c_str();
	  std::bitset<8> bits(a);
	  std::bitset<2> bits_dp; 
	  std::bitset<1> bits_ep; 
	  std::bitset<1> bits_rp; 
	  bits_dp.set(0, bits.test(4));
	  bits_dp.set(1, bits.test(5));
	  bits_ep.set(0, bits.test(7));
	  bits_rp.set(0, bits.test(6));
	  
	  char dp = char(bits_dp.to_ulong());
	  char ep = char(bits_ep.to_ulong());
	  char rp = char(bits_rp.to_ulong());
	  
	  std::ostringstream s;
	  s << "damping enum = "
	    << (int) dp 
	    << ", energy enum = "
	    << (int) ep 
	    << ", receive mode = "
	    << (int) rp;
	  
	  return s.str();
	}
      };
      // // class impedence_type1
      // // {
      // // public: 
      // // 	enum type{threethreezero=330,onezerofour=104,fourfour=44,threefour=34};
      // // }
      
      // // template<class impedance>
      // // class damping_impedance : public recv_cmd
      // // {
      // // 	enum impedance::type m_value;
      // // 	char m_channel_char;
      // // 	std::string m_cmd;
      // // 	boost::scoped_ptr<pulser_receiver> m_pr;
      // // 	vector<boost::shared_ptr<pulser_cmd> > m_cmds;
      // // protected:
      // // 	void update() {m_channel_char = m_pr->get_channel_char();}
      // // 	void set_pulser_receiver(boost::shared_ptr<pulser_receiver> p){m_pr = p;}
      // // 	void set_cmd();
      // // public:
      // // 	damping_impendance(const enum impedance::type& value){m_value = value; set_cmd();};
      // // 	operator const std::string() {return m_cmd;}
      // // 	size_t reply_size() const {return 6;};
      // // 	virtual std::string decode(const std::string&){return stringify((int) *ans.substr(3,1).c_str());}
      // // };

      // // template<class impedance>
      // // damping_impedance<impedance>::set_cmd()
      // // {
      // // 	m_cmd.push_back(m_address);
      // // 	m_cmd.push_back(0x01);
      // // 	m_cmd.push_back(0x66); //f
      // // 	short s = (short) m_value;
      // // 	m_cmd.push_back(s);
      // // 	m_cmd.push_back(s>>8);
      // // 	m_cmd.push_back(m_channel_char);
      // // 	m_cmd.push_back(0x00);
      // // }

      // // struct energy_level {};
      // // struct receiver_mode {};

      // // class low_energy : public recv_cmd , public energy_level
      // // {
      // // 	char m_channel_char;
      // // 	std::string m_cmd;
      // // 	boost::scoped_ptr<pulser_receiver> m_pr;
      // // protected:
      // // 	void update() {m_channel_char = m_pr->get_channel_char();}
      // // 	void set_pulser_receiver(boost::shared_ptr<pulser_receiver> p){m_pr = p;}
      // // 	void set_cmd();
      // // public:
      // // 	void set(const double& v);
      // // 	operator const std::string() {return m_cmd;}
      // // 	size_t reply_size() const {return 6;};
      // // 	virtual std::string decode(const std::string&){return stringify((int) *ans.substr(3,1).c_str());}
      // // };
      // // class high_energy : public recv_cmd , public energy_level
      // // {
      // // 	char m_channel_char;
      // // 	std::string m_cmd;
      // // 	boost::scoped_ptr<pulser_receiver> m_pr;
      // // protected:
      // // 	void update() {m_channel_char = m_pr->get_channel_char();}
      // // 	void set_pulser_receiver(boost::shared_ptr<pulser_receiver> p){m_pr = p;}
      // // 	void set_cmd();
      // // public:
      // // 	void set(const double& v);
      // // 	operator const std::string() {return m_cmd;}
      // // 	size_t reply_size() const {return 6;};
      // // 	virtual std::string decode(const std::string&){return stringify((int) *ans.substr(3,1).c_str());}
      // // };

      // // class echo_mode : pubic recv_cmd, public receiver_mode
      // // {

      // // 	boost::array<char,2> m_voltage;
      // // 	double m_min, m_max;
      // // 	char m_channel_char;
      // // 	std::string m_cmd;
      // // 	boost::scoped_ptr<pulser_receiver> m_pr;
      // // protected:
      // // 	void update() {m_channel_char = m_pr->get_channel_char();}
      // // 	void set_pulser_receiver(boost::shared_ptr<pulser_receiver> p){m_pr = p;}
      // // 	void set_cmd();
      // // public:
      // // 	void set(const double& v);
      // // 	operator const std::string() {return m_cmd;}
      // // 	size_t reply_size() const {return 6;};
      // // 	virtual std::string decode(const std::string&){return stringify((int) *ans.substr(3,1).c_str());}
      // // };
      
      // // class receive_only_mode : public recv_cmd, public receiver_mode
      // // {

      // // 	boost::array<char,2> m_voltage;
      // // 	double m_min, m_max;
      // // 	char m_channel_char;
      // // 	std::string m_cmd;
      // // 	boost::scoped_ptr<pulser_receiver> m_pr;
      // // protected:
      // // 	void update() {m_channel_char = m_pr->get_channel_char();}
      // // 	void set_pulser_receiver(boost::shared_ptr<pulser_receiver> p){m_pr = p;}
      // // 	void set_cmd();
      // // public:
      // // 	void set(const double& v);
      // // 	operator const std::string() {return m_cmd;}
      // // 	size_t reply_size() const {return 6;};
      // // 	virtual std::string decode(const std::string&){return stringify((int) *ans.substr(3,1).c_str());}

      // // };
	
      // template<int damping_policy, int energy_policy, int receiver_policy>
      // class pulser_mode : public recv_cmd
      // {
      // 	char m_channel_char;
      // 	std::string m_cmd;
      // 	boost::scoped_ptr<pulser_receiver> m_pr;
      // protected:
      // 	void update() {m_channel_char = m_pr->get_channel_char();}
      // 	void set_pulser_receiver(boost::shared_ptr<pulser_receiver> p){m_pr = p;}
      // public:
      // 	operator const std::string() {return m_cmd;}
      // 	size_t reply_size() const {return 6;};
      // 	virtual std::string decode(const std::string&){return stringify((int) *ans.substr(3,1).c_str());}

	
      // };
      
      

      // template<int damping_policy, int energy_policy, int receiver_policy>
      // pulser_mode<damping_policy, energy_policy, receiver_policy>::pulser_mode()
      // {
	
      // 	m_cmd.push_back(m_address);
      // 	m_cmd.push_back(0x01);
      // 	m_cmd.push_back(0x66); //f
      // 	char dp = (char) damping_policy;
      // 	char ep = (char) energy_policy;
      // 	char rp = (char) receiver_policy;
      // 	char cmd = 0x00;
      // 	cmd |= (dp<<4);
      // 	cmd |= (ep<<2);
      // 	cmd |= (rp<<1);
      // 	m_cmd.push_back(cmd);
      // 	m_cmd.push_back(m_channel_char);
      // 	m_cmd.push_back(0x00);

      // }
    

      /** A factory for creating initiation sequence commands.
       *  The DPR500 has an initiation sequence that in which the address of the 
       *  instrument is set.  
       *  Other characteristics of the instrument may also be probed at this time.
       *  This class provides an interface to the commands of this sequence.
       *  
       *  
       *  Commands that expect a reply are differentiated from commands that do not
       *  by their interface.  
       *   - InitiationFactor::cmd_ptr is a command pointer that do not expect a reply.
       *     They are issued with the @code DPR500::send() @endcode function of the DPR500.
       *   - InitiationFactor::recv_cmd_ptr is a command pointer that does expect a reply.
       *     They are issued with the @code DPR500::recv() @endcode 
       *     function or the  @code DPR500::timed_recv() @endcode functions.
       *   
       * The initiation sequence must begin with an 
       * InitiationFactory::initiate_address_assignment() function, 
       * and must end with an InitiationFactory::end_address_assignment(address) 
       * function call. 
       * 
       * A minimal sequence for initiating the DPR500 will therefore be 
       * @code 
       * command::InitiationFactory f;
       * typedef command::InitiationFactory::cmd_ptr cmd;
       * typedef command::InitiationFactory::recv_cmd_ptr rcmd;
       * cmd  init = f.initiate_address_assignment();
       * rcmd addr = f.instrument_address();
       * cmd  close;
       * send(init); 
       * char address = timed_recv(addr)[0]; //returned string is of 1 char length.
       * close =  f.end_address_assignment(address);
       * send(close);
       * @endcode
       *  
       */
      class InitiationFactory
      {
	
	
      public:
	/** A command pointer */
	typedef boost::shared_ptr<cmd>      cmd_ptr;
	/** A receive command pointer */
	typedef boost::shared_ptr<recv_cmd> recv_cmd_ptr;

	/** Start the address assignment phase.
	 * This command must be issued first.
	 * @return a cmd_ptr for the initiate_address_assignment command.
	 *
	 * @warning This command does not seem to always work immediately.
	 * Sometimes several attempts need to be issued.
	 * One way to do this is to test that an address has been assigned, for example:
	 * @code 
	 * cmd  init = f.initiate_address_assignment();
	 * rcmd addr = f.instrument_address();
	 * cmd  close;
	 * bool failed_to_open = true;
	 * unsigned int count = 0;
	 * while (count<3 && failed_to_open){
	 *   send(init);
	 *   boost::this_thread::sleep(boost::posix_time::seconds(1)); //need a pause here
	 *   send(init);
	 *   try{
	 *     m_address = timed_recv(addr,1)[0];
	 *     close =  f.end_address_assignment(m_address);
	 *     send(close);
	 *     failed_to_open = false; 
	 *   }
	 *   catch (boost::system::system_error& e)
	 *   {}
	 *   ++count;
	 * }
	 * if(failed_to_open) throw ICR::exception::failed_to_contact_DPR500_device();
	 * @endcode.
	 */
	virtual
	boost::shared_ptr<command::initiate_address_assignment> 
	initiate_address_assignment() const;  //cmd_ptr
	
	/** End address assignment mode.
	 * This command must be issued at the end of the address assignment mode.
	 * @param address The address of the device.
	 * @return a cmd_ptr for the end_address_assignment command.
	 */
	virtual
	boost::shared_ptr<command::end_address_assignment> 
	end_address_assignment(const char& address) const; //cmd_ptr
	
	/** Set the address of the device.
	 * @param i the address.  This must be a number between 1 and 255.
	 * @return A cmd_ptr to the set_address command.
	 */
	virtual
	boost::shared_ptr<command::set_address> 
	set_address(const char& i)  const; //cmd_ptr
	
	/** Request the address of the device.
	 * @return A recv_cmd_ptr to the instrument_address command.
	 */
	virtual
	boost::shared_ptr<command::instrument_address> 
	instrument_address() const;
	
	/** Request the intrument type
	 * @return A recv_cmd_ptr to the instrument_type command.
	 */
	virtual
	boost::shared_ptr<command::instrument_type> 
	instrument_type() const;

	/** Request the intrument serial number
	 * @return A recv_cmd_ptr to the instrument_serial_number command.
	 */
	virtual
	boost::shared_ptr<command::instrument_serial_number> 
	instrument_serial_number() const;
	
	/** Request the intrument firmware code
	 * @return A recv_cmd_ptr to the instrument_firmware command.
	 */
	virtual
	boost::shared_ptr<command::instrument_firmware> 
	instrument_firmware() const;
	
	/** Request the intrument serial number
	 * @return A recv_cmd_ptr to the instrument_serial_number command.
	 */
	virtual
	boost::shared_ptr<command::DPR500_serial_number> 
	DPR500_serial_number() const;
      };
      
      /** A factory for generating DPR500 commands */
      class DPR500CommandFactory
      {
	char m_address;
      public:
	/** A constructor.
	 * @param addr The address of the DPR500 */
	DPR500CommandFactory(const char& addr) : m_address(addr) {}
	/* Set the address of the DPR500.
	 * @param addr The address. */
	void set_address(const char& addr) {m_address = addr;}

	/** A receive command pointer */
      	typedef boost::shared_ptr<command::DPR500_recv_cmd> recv_cmd_ptr;


	/** Generate a blink command. 
	 * @param speed The speed of the blinking.
	 * @return A blink command pointer */
	virtual
	boost::shared_ptr<command::blink> 
	blink( const short& speed = 100)
	{return  boost::shared_ptr<command::blink>(new command::blink(m_address,speed));}
  


	/** Generate a turn on command. 
	 * @param c The channel to turn on
	 * @return The command */
      	virtual
      	boost::shared_ptr<command::turn_on>
      	turn_on(const enum channel::type& c)
      	{return boost::shared_ptr<command::turn_on>(new command::turn_on(m_address, c));}
	  
	/** Generate a turn off command. 
	 * @param c The channel to turn off
	 * @return The command */
      	virtual
      	boost::shared_ptr<command::turn_off>
      	turn_off(const enum channel::type& c)
      	{return boost::shared_ptr<command::turn_off>(new command::turn_off(m_address, c));}
	  
	/** Generate a gain command. 
	 * @param channel The channel
	 * @param measured The measured preamp gain
	 * @param min The minimum acceptable voltage
	 * @param max The maximum acceptable voltage
	 * @param step The step size
	 * @return The command */
      	virtual
      	boost::shared_ptr<command::gain>
      	gain(const channel::type& channel, const short& measured, const short& min, const short& max, const short step = 1) 
      	{return boost::shared_ptr<command::gain>(new command::gain(m_address, channel, measured,min,max,step));}
	  
	/** Generate a high pass filter command.
	 * @param ch The channel
	 * @return The command
	 */
      	template<class filter_option>
      	boost::shared_ptr<command::high_pass_filter<filter_option> >
      	high_pass_filter(const enum channel::type& ch) 
      	{return boost::shared_ptr<command::high_pass_filter<filter_option> >(new command::high_pass_filter<filter_option> (m_address, ch));}
	  
	  
	/** Generate a high pass filter command.
	 * @param ch The channel
	 * @return The command 
	 */
      	template<class filter_option>
      	boost::shared_ptr<command::low_pass_filter<filter_option> >
      	low_pass_filter(const enum channel::type& ch) 
      	{return boost::shared_ptr<command::low_pass_filter<filter_option> >(new command::low_pass_filter<filter_option> (m_address, ch));}
	  
	/** Generate a pulser serial number command.
	 * @param c The channel
	 * @return The command
	 */
	virtual
	boost::shared_ptr<command::pulser_serial_number> 
	pulser_serial_number(const enum channel::type& c)
	{return  boost::shared_ptr<command::pulser_serial_number>(new command::pulser_serial_number(m_address,c));}
  
      };

      
      /** Factory for producing pulser commands */
      class PulserCommandFactory 
      {
      public:
      	typedef boost::shared_ptr<command::recv_cmd> recv_cmd_ptr;
	
	/** Generate a pulser mode command 
	 */
      	template<class damping_policy>
	boost::shared_ptr<command::pulser_mode<damping_policy> >
	pulser_mode()
	{return boost::shared_ptr<command::pulser_mode<damping_policy>  > (new command::pulser_mode<damping_policy> ());}
	
	/** Generate a master command 
	 * @param is_master true or false.
	 * @return The command 
	 */
      	virtual
      	boost::shared_ptr<command::master>
      	master(const bool& is_master) 
      	{return boost::shared_ptr<command::master>(new command::master(is_master));}
	  
	/** Generate a pulse repitition frequency command 
	 * @param max_prf The maximum prf
	 * @return The command 
	 */
      	virtual
      	boost::shared_ptr<command::prf>
      	prf(const unsigned short& max_prf)  
      	{return boost::shared_ptr<command::prf>(new command::prf(max_prf));}

	   
	/** Generate an internal trigger command
	 * @return The command 
	 */
      	virtual
      	boost::shared_ptr<command::internal_trigger>
      	internal_trigger()
      	{return boost::shared_ptr<command::internal_trigger>(new command::internal_trigger());}

	/** Generate an external trigger command
	 * @return The command 
	 */
      	virtual
      	boost::shared_ptr<command::external_trigger>
      	external_trigger()
      	{return boost::shared_ptr<command::external_trigger>(new command::external_trigger());}
  
	/** Generate a set voltage command
	 * @param min The min voltage
	 * @param max The max voltage 
	 * @return The command 
	 */
      	virtual
      	boost::shared_ptr<command::voltage>
      	voltage(const double& min, const double& max)
      	{return boost::shared_ptr<command::voltage>(new command::voltage(min, max));}


      };
      

    }
  }
}

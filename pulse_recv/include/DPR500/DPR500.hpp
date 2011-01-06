#include "coms_manager/serial_manager.hpp"
#include "commands.hpp"
#include "enums.hpp"
#include "exceptions.hpp"
#include "filters.hpp"
#include "pulser_receivers.hpp"
#include <string>
namespace ICR{

  namespace pulser{
    namespace exceptions{
    }

    // class DPR500_com : public ICR::coms::serial_manager
    // {
    //   std::string attach_header(const std::string& cmd);
    // protected:
    //   char m_address;
    // public:
    //   DPR500_com(const std::string& port);
    //   /** The distructor.*/
    //   virtual ~DPR500_com();
    //   /** Send a command to the scope.  Most of the time this will be null terminated @param cmd The command string.*/
    //   virtual void send(const std::string& cmd);
      
    //   /** Send a request to the scope.  Most of the time this will be null terminated 
    //    * @param cmd The request string 
    //    * @param buffsize The size of the buffer to collect.  The returned string can be shorter than this size.
    //    * @return The returned string from the scope (missing the header). */
    //   virtual std::string recv(const std::string& cmd, const unsigned int& buffsize = 128) ;
    //   // virtual std::string timed_recv(const std::string& cmd, const unsigned int& buffsize = 128, const double& seconds = 5) ;
    // };


      
    // class RPL2 : public pulser_receiver
    // {
    //   typedef boost::shared_ptr<command::pulser_receiver_cmd> command_ptr;
    // private:
    //   std::vector<command_ptr> m_cmds;
    //   boost::shared_ptr<command::gain>             m_gain;
    //   // boost::shared_ptr<command::high_pass_filter<command::> > m_hpf;
    //   // boost::shared_ptr<command::low_pass_filter<command:: > >  m_lpf;

      
    //   command::ReceiverCommandFactory m_rf;
    //   command::PulserCommandFactory m_pf;
    // protected:
    //   void attach_cmd(command_ptr c);
    //   char get_channel_char();
    //   void notify();
      
    // public:
    //   RPL2(const enum channel::type& channel,
    // 	   /* pulser */
    // 	   const double& cal_fall_time = 6.0 /*ns*/,
    // 	   const double& cal_min_neg_pulse_amp = 170 /*V*/,
    // 	   const double& cal_pulse_width = 109 /*ns FWHM*/,
    // 	   /* receiver */
    // 	   const double& cal_gain = 5.3 /*db */,
    // 	   const double& cal_high_freq_cut_off  = 118 /*MHz*/
    // 	   )
    // 	:  m_gain(new command::gain((short)cal_gain, -13, 66, 1))
    //   {
    // 	m_gain = rf.gain(cal_gain, -13,66,1);
    // 	// m_hpf  = rf.hpf<command::>();
    // 	// m_lpf  = rf.lpf<command::>();
	
    // 	// m_master = pf.master(true);
    // 	// m_prf    = pf.prf();
    // 	// m_voltage = pf.voltage(min,max);
	
    // 	// m_internal_trig = m_pf.internal_trigger();
    // 	// m_external_trig = m_pf.external_trigger();
	
    // 	attach_cmd(m_gain);
    // 	// attach_cmd(m_hpf);
    // 	// attach_cmd(m_lpf);
    // 	// attach_cmd(m_master);
    // 	// attach_cmd(m_prf);
    // 	// attach_cmd(m_voltage);
    // 	// attach_cmd(m_external_trig);
	
    //   }
      
    //   void gain(const unsigned int& gain) {m_gain.set(gain);}
    //   void high_pass_filter(const unsigned int& value) {m_hpf=value;};
    //   void low_pass_filter(const unsigned int& value)  {m_lpf=value;};
    //   void voltage(const unsigned int& value)  {m_voltage=value;};
      
      
      
    //   // void turn_on() {
    //   // 	cmd_ptr on = m_pf.turn_on();
    //   // }
      
    //   // void turn_off() {
    //   // 	cmd_ptr off = m_pf.turn_off();
    //   // }
      
    //   void internal_trigger {
    // 	detach_cmd(external_trig);
    // 	attach_cmd(internal_trig);
    //   }
    //   void external_trigger  {
    // 	detach_cmd(internal_trig);
    // 	attach_cmd(external_trig);
    //   }
      
      
      
    // };
      
      
    // class RPU2 : public pulse_receiver
    // {
      
    // private:
    //   channel::type m_channel;
    //   double m_preamp_gain;
    //   char m_gain;
    //   char m_low_pass_filter, m_high_pass_filter, m_prf, m_voltage;
    //   bool m_is_on, m_is_master, m_is_internal_trigger;
      
    // protected:
    //   void set_channel(const enum channel::type& channel) {m_channel = channel;};
    //   enum channel::type& get_channel      {return m_channel;}
    //   char get_gain_char()    const;       {return m_gain;}
    //   char get_channel_char() const;       {return m_channel;} 
    //   char get_low_pass_filter() const;  {return m_low_pass_filter;}
    //   double get_high_pass_filter() const; {return m_high_pass_filter;}
    //   bool is_master() const               {return m_is_on;};
    //   bool is_on() const                   {return m_is_on;};
    //   double get_prf() const;              {return m_prf;}
    //   bool is_internal_trigger() const     {return m_is_internal_trigger;}
    //   double get_voltage() const;          {return m_voltage;}
      
      
    // public:
    //   RPL2(
    // 	   /* pulser */
    // 	   const double& cal_fall_time = 0.7 /*ns*/,
    // 	   const double& cal_min_neg_pulse_amp = 251 /*V*/,
    // 	   const double& cal_pulse_width = 1.3 /*ns FWHM*/,
    // 	   /* receiver */
    // 	   const double& cal_gain = 6.6 /*db */,
    // 	   const double& cal_high_freq_cut_off  = 305 /*MHz*/,
    // 	   const double& cal_low_freq_cut_off  = 30 /*MHz*/
    // 	   );
      
    //   void gain(const unsigned int& gain){
    // 	m_gain = (char) (gain- m_preamp_gain + 13);
    //   };  
    //   void is_master(const bool& b){m_is_master = b;}
      
      
    //   class high_pass{
    //   public:
    // 	enum type {ten_KHz, one_MHz, twopointfive_MHz, five_MHz, sevenpointfive_MHz};
    //   }

    //   class low_pass{
    //   public:
    // 	enum type {sevenpointfive_MHz, ten_MHz, fifteen_MHz, twentytwopointfive_MHz, fiftyMHz};
    //   }


    //   void high_pass_filter(const high_pass::type& value)
    //   {
    // 	switch(value){
    // 	case ten_KHz: m_low_pass_filter = (char) 0; break;
    // 	}
	  
    //   }
    //   void low_pass_filter (const high_pass::type& value);
      

    // };


    /** The DPR500 class. 
     * This class controls the DPR500 box only.
     * For example commands that change the flashing of the power button may be issued.
     * Functions such as the gain and the pulse repetition frequency are controlled
     * by individual pulser-receivers.
     * These may be attached to this class using the attach_A() and attach_B() 
     * method (to attach the device to channel A or B respectively).
     * The pulser/receivers are controlled via their interface, 
     * the pulser_receiver abstract class.
     *
     * To notify the DPR500 box of any changes to the pulsers/reveivers,
     * the notify() command must be issued.
     *
     */
    class DPR500 :  public ICR::coms::serial_manager
    {
     
    private: 
      typedef pulser_receiver* pr_ptr;
      char m_address;
      bool A_attached, B_attached;
      pr_ptr m_A;
      pr_ptr m_B;
      command::DPR500CommandFactory m_CF;

      std::list<boost::shared_ptr<command::DPR500_recv_cmd> > cmds_A;
      std::list<boost::shared_ptr<command::DPR500_recv_cmd> > cmds_B;
      // command::ReceiverCommandFactory m_rf;

      // void 
      // notify_hardware(pulser_receiver* p);
      
      boost::shared_ptr<command::gain>                             m_gain_A;
      boost::shared_ptr<command::gain>                             m_gain_B;
     
      boost::shared_ptr<command::high_pass_filter<filter::HPFA> >  m_hpf_A;
      boost::shared_ptr<command::low_pass_filter <filter::LPFA> >  m_lpf_A;
      boost::shared_ptr<command::high_pass_filter<filter::HPFB> >  m_hpf_B;
      boost::shared_ptr<command::low_pass_filter <filter::LPFB> >  m_lpf_B;
      
    protected:
      /** send a command to the DPR500.
       * @param cmd A pointer to the command::cmd to be sent. */
      void send(const boost::shared_ptr<command::cmd>& cmd);
      
      /** Send a request to the DPR500 and  receive the reply.
       * @param recv_cmd A pointer to the command::recv_cmd to be sent.
       * @return The decoded reply from the DPR500 */
      std::string recv(const boost::shared_ptr<command::recv_cmd>& recv_cmd);
      
      /** Send a timed request to the DPR500 and  receive the reply.
       * If the time interval runs out then a 
       *  boost::system::system_error exception will be thrown.
       * @param recv_cmd A pointer to the command::recv_cmd to be sent.
       * @param seconds The number of seconds to wait for a reply from the DPR500.
       * @return The decoded reply from the DPR500 */
      std::string timed_recv(const boost::shared_ptr<command::recv_cmd>& recv_cmd,
			     const unsigned int& seconds =1);
      
      void init() throw(exceptions::failed_to_contact_DPR500_device);
      // void gain(pr_ptr);
    public:
      /** Constructor.
       * @param port The serial port that the device is attached to.
       *         - On windows this will be, for example, "COM1",
       *         - On linux based systems this will be, for example "/dev/ttyS0".
       */
      DPR500(const std::string& port);
      
      /** Attach a pulser/receiver to channel A.
       * @param p The pulser/reveiver to attach.
       */
      void attach_A(pulser_receiver* p);
      
      /** Attach a pulser/receiver to channel B.
       * @param p The pulser/reveiver to attach.
       */
      void attach_B(pulser_receiver* p);
      /** Detach the pulser/receiver from channel A. */
      void detach_A();
      /** Detach the pulser/receiver from channel B. */
      void detach_B();
      void turn_on_A() ;
      void turn_off_A();
      void turn_on_B() ;
      void turn_off_B();

      void set_high_pass_filter_A(const enum filter::HPFA::type& v){m_hpf_A->set(v);}
      void set_low_pass_filter_A (const enum filter::LPFA::type& v){m_lpf_A->set(v);}
      void set_high_pass_filter_B(const enum filter::HPFB::type& v){m_hpf_B->set(v);}
      void set_low_pass_filter_B (const enum filter::LPFB::type& v){m_lpf_B->set(v);}
      void set_gain_A (const short& gain){if(!A_attached) throw exceptions::nothing_attached_to_A();m_gain_A->set(gain);}
      void set_gain_B (const short& gain){if(!B_attached) throw exceptions::nothing_attached_to_B();m_gain_B->set(gain);}
      
      /** Notify the DPR500 of any changes that have been 
       * requested in the attached devices. */
      void notify();
      
      /** Set the blink speed to the power button. 
       * @param speed A number between 50 and 200.
       */
      void blink(const short& speed=100);
      
    };

  }

}

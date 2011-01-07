#include "coms_manager/serial_manager.hpp"
#include "commands.hpp"
#include "enums.hpp"
#include "exceptions.hpp"
#include "filters.hpp"
#include "pulser_receivers.hpp"
#include <string>
namespace ICR{

  namespace pulser{

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
      
      void init() throw(ICR::exception::failed_to_contact_DPR500_device);
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
      /** Turn on channel A */
      void turn_on_A() ;
      /** Turn off channel A */
      void turn_off_A();
      /** Turn on channel B */
      void turn_on_B() ;
      /** Turn off channel B */
      void turn_off_B();
      
      /** Set the value of the high pass filter for channel A.
       * @param v The 3db value
       */
      void set_high_pass_filter_A(const enum filter::HPFA::type& v){m_hpf_A->set(v);}

      /** Set the value of the low pass filter for channel A.
       * @param v The 3db value
       */
      void set_low_pass_filter_A (const enum filter::LPFA::type& v){m_lpf_A->set(v);}

      /** Set the value of the high pass filter for channel B.
       * @param v The 3db value
       */
      void set_high_pass_filter_B(const enum filter::HPFB::type& v){m_hpf_B->set(v);}
    
      /** Set the value of the low pass filter for channel B.
       * @param v The 3db value
       */
      void set_low_pass_filter_B (const enum filter::LPFB::type& v){m_lpf_B->set(v);}

      /** Set the gain for channel A.
       * @param gain The gain
       */
      void set_gain_A (const short& gain){if(!A_attached) throw ICR::exception::nothing_attached_to_A();m_gain_A->set(gain);}

      /** Set the gain for channel B.
       * @param gain The gain
       */
      void set_gain_B (const short& gain){if(!B_attached) throw ICR::exception::nothing_attached_to_B();m_gain_B->set(gain);}
      
      /** Notify the DPR500 of any changes that have been 
       * requested in the attached devices.
       * @attention This is required to pass on any information to pulser/receivers */
      void notify();
      
      /** Set the blink speed to the power button. 
       * @param speed A number between 50 and 200.
       */
      void blink(const short& speed=100);
      
    };

  }

}

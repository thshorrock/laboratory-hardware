#pragma once
#include "basic_pulser_receiver.hpp"


namespace ICR{

  namespace pulser{
    
    /** The RPL2 pulser receiver. 
     *  The pulser and receiver implement a singleton pattern.
     *  If you have two RPL2 pulser-receivers then you need a second RPL2 class.
     */
    class RPL2 : public pulser_receiver
    {
      /** A typedef defining the cmd_ptr interface */
    public: typedef boost::shared_ptr<command::PulserReceiver_cmd> cmd_ptr;
    private:

      command::PulserCommandFactory   m_pf;

      boost::shared_ptr<command::internal_trigger>                 m_int_trig;
      boost::shared_ptr<command::external_trigger>                 m_ext_trig;
      boost::shared_ptr<command::pulser_mode<damping_RPL2> >    m_pulser_mode;
      boost::shared_ptr<command::master>                           m_master;
      boost::shared_ptr<command::prf>                              m_prf;
      boost::shared_ptr<command::voltage>                          m_voltage;
        
      


      //Singleton pattern.
      static boost::shared_ptr<RPL2> m_instance;
      
    protected:

      /** Constructor. */
      RPL2(); //protected constructor.
    public:
      /** Get the (singleton) instance. @return a pointer to the instance. */
      static RPL2* Instance();
     
      /** Set the pulser mode.
       * @param damping The damping policy, from the damping_RPL2 struct.
       * @param energy The energy policy, either energy_policy::HIGH or energy_policy::LOW
       * @param mode Either receive_mode::PULSE_ECHO or receive_mode::RECEIVE_ONLY
       */
      void set_pulser_mode(const enum damping_RPL2::type& damping, 
			   const enum energy_policy::type& energy,
			   const enum receive_mode::type& mode);

      /** Set the pulse repetition frequency.
       *  @param frequency The requested frequency the frequency in Hertz
       *  @return The actual frequency used */
      double  set_prf (const double& frequency){return m_prf->set(frequency);}
      /** Use the internal trigger */
      void set_internal_trigger();
      /** Use an external trigger */
      void set_external_trigger();
      /** Set in master triggering mode */
      void set_master() {m_master->set(true);}
      /** Set in slave triggering mode */
      void set_slave()  {m_master->set(false);};
      /** Set the voltage.
       * @param voltage The voltage between 275 and 500
       */
      void set_voltage(const unsigned short& voltage){m_voltage->set(voltage);}
      
      //These are overloads
      short get_gain() const {return 7;}
      std::string get_serial_number() const {return "GE3108";}
    };
    
    
    /** The RPU2 pulser receiver. 
     * The is a pulser receiver with the following parameters:
     *   - p1
     *   - p2
     */
    class RPU2 : public pulser_receiver
    {
      /** A typedef defining the cmd_ptr interface */
    public: typedef boost::shared_ptr<command::PulserReceiver_cmd> cmd_ptr;
    private:

      command::PulserCommandFactory   m_pf;

      boost::shared_ptr<command::internal_trigger>                 m_int_trig;
      boost::shared_ptr<command::external_trigger>                 m_ext_trig;
      boost::shared_ptr<command::pulser_mode<damping_RPU2> >       m_pulser_mode;
      boost::shared_ptr<command::master>                           m_master;
      boost::shared_ptr<command::prf>                              m_prf;
      boost::shared_ptr<command::voltage>                          m_voltage;
        
      
      boost::shared_ptr<command::query_voltage> m_query_voltage;
      


      //Singleton pattern.
      static boost::shared_ptr<RPU2> m_instance;
      
    protected:

      /** Constructor. */
      RPU2(); //protected constructor.
    public:
      /** Get the (singleton) instance. @return a pointer to the instance. */
      static RPU2* Instance();
     

      /** Set the pulser mode.
       * @param damping The damping policy, from the damping_RPU2 struct.
       * @param energy The energy policy, either energy_policy::HIGH or energy_policy::LOW
       */
      void set_pulser_mode(const enum damping_RPU2::type& damping, 
			   const enum energy_policy::type& energy);
      /** Set the pulse repetition frequency.
       *  @param frequency The requested frequency the frequency in Hertz
       *  @return The actual frequency used */
      double  set_prf (const double& frequency){return m_prf->set(frequency);}
      /** Use the internal trigger */
      void set_internal_trigger();
      /** Use an external trigger */
      void set_external_trigger();
      /** Set in master triggering mode */
      void set_master() {m_master->set(true);}
      /** Set in slave triggering mode */
      void set_slave()  {m_master->set(false);};
      
      //These are overloads
      short get_gain() const {return 7;}
      std::string get_serial_number() const {return "GG1041";}
    };

    /** Create a pointer to a pulser receiver object. */
    class PulserReceiverFactory
    {
    public:
      /** A pulser-receiver pointer */
      typedef boost::shared_ptr<pulser_receiver> pulser_receiver_ptr; 
      
      /** Make an RPL2 object. @return A pointer to an RPL2 object */
      pulser::RPL2* make_RPL2() const 
      {return  RPL2::Instance();} //Return the singleton to this class
      
      /** Make an RPU2 object. @return A pointer to an RPU2 object */
      pulser::RPU2* make_RPU2() const 
      {return  RPU2::Instance();} //Return the singleton to this class

    };

  }
}

#pragma once
#include "basic_pulser_receiver.hpp"


namespace ICR{

  namespace pulser{
    
    /** The RPL2 pulser receiver. 
     * The is a pulser receiver with the following parameters:
     *   - p1
     *   - p2
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
      static RPL2* Instance();
     
      // /** Set the gain. @param gain The gain */
      // void set_gain(const short& gain){m_gain->set(gain);}

      void set_pulser_mode(const enum damping_RPL2::type&, 
			   const enum energy_policy::type&,
			   const enum receive_mode::type&);
      double  set_prf (const double& frequency){return m_prf->set(frequency);}
      void set_internal_trigger();
      void set_external_trigger();
      void set_master() {m_master->set(true);}
      void set_slave()  {m_master->set(false);};
      void set_voltage(const unsigned short& voltage){m_voltage->set(voltage);}
      
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
      static RPU2* Instance();
     
      // /** Set the gain. @param gain The gain */
      // void set_gain(const short& gain){m_gain->set(gain);}

      void set_pulser_mode(const enum damping_RPU2::type&, 
			   const enum energy_policy::type&);
      double  set_prf (const double& frequency){return m_prf->set(frequency);}
      void set_internal_trigger();
      void set_external_trigger();
      void set_master() {m_master->set(true);}
      void set_slave()  {m_master->set(false);};
      
      short get_gain() const {return 7;}
      std::string get_serial_number() const {return "GG1041";}
    };


    class PulserReceiverFactory
    {
    public:
      typedef boost::shared_ptr<pulser_receiver> pulser_receiver_ptr; 
      
      pulser::RPL2* make_RPL2() const 
      {return  RPL2::Instance();} //Return the singleton to this class
      pulser::RPU2* make_RPU2() const 
      {return  RPU2::Instance();} //Return the singleton to this class

    };

  }
}

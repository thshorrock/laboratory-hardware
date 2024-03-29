#pragma once
#include "coms_manager.hpp"
#include "exceptions.hpp"
#include <boost/asio/serial_port.hpp> 
#include <string>
namespace ICR{
  namespace coms{
    /** flow_control: Acceptable options;
     * flow_contrl::type none, software, hardware;
     */
    typedef boost::asio::serial_port_base::flow_control flow_control ;
    /** parity: Acceptable options;
     * parity::type none, odd, even;
     */
    typedef boost::asio::serial_port_base::parity parity ;
    /** stop_bits: Acceptable options;
     * stop_bits::type one, onepointfive, two;
     */
    typedef boost::asio::serial_port_base::stop_bits stop_bits ;
    
    /** Manage serial connections.*/
    class serial_manager : public coms_manager
    {
      
      boost::asio::io_service m_io_service;
      std::string m_name;
      const unsigned int m_baud_rate;
      const enum flow_control::type m_flow_control;
      const enum parity::type m_parity;
      const enum stop_bits::type m_stop_bits;
      const unsigned int m_char_size;
      boost::asio::serial_port  m_SerialPort;
      boost::system::error_code m_error;
      
    public:
      /** A constructor.
       *  @param name The dvice name, for example "COM1" (windows, or "/dev/ttyS0" (linux).
       *  @param baud_rate The baud rate. 
       *  @param flow_control The flow control. Acceptable values are flow_control::none, flow_control::software, flow_control::hardware.
       *  @param parity The parity of the connection. Acceptable values are parity::none, parity::even, parity::odd.
       *  @param stop_bits The number of stop bits. Acceptable values are stop_bits::one, stop_bits::one_point_five, stop::bits::two
       *  @param char_size The number of characters in connection.
       */
      serial_manager(const std::string& name, 
		     const unsigned int& baud_rate  = 19200,
		     const enum flow_control::type&   flow_control  = flow_control::none,
		     const enum parity::type&         parity        = parity::none,
		     const enum stop_bits::type&      stop_bits     = stop_bits::one,
		     const unsigned int& char_size  = 8
		     ) 
	throw(exception::serial_port_does_not_exist)
	;
    
      /** A destructor.*/
      ~serial_manager();
      
      /** Cancels the queue of commands.
       */ 
      void 
      cancel();
      
      /** Open the connection. 
       * @attention Automatically called when the serial connection is constructed.
       */ 
      void 
      open();
      
      /** Close the connection. */ 
      void 
      close();
    
      /* The following functions are overloads. */
      void 
      send(const std::string& cmd);

      
      std::string 
      recv(const std::string& cmd, 
	   const unsigned long& buffsize = 128, 
	   const bool& size_exactly=false);

       
      std::string 
      timed_recv(const std::string& cmd, 
		 const unsigned long& buffsize = 128, 
		 const double& seconds = 5, 
		 const bool& size_exactly = false)
	throw(exception::timeout_exceeded) ;

       
      std::string 
      recv( const unsigned long& buffsize = 128, 
	    const bool& size_exactly = false)
	throw(exception::exception_in_receive_you_must_resend_command );

       
      std::string 
      timed_recv( const unsigned long& buffsize = 128, 
		  const double& seconds = 5, 
		  const bool& size_exactly = false) 
	throw(exception::timeout_exceeded,
	      exception::exception_in_receive_you_must_resend_command );
      
    };
  }
  
}

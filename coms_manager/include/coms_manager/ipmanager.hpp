#pragma once

#include "coms_manager.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <vector>
#include <string>


namespace ICR{
  namespace coms{

    /** Manage LAN connections (via TCP).*/
    class IPmanager : public coms_manager
    {
      
      boost::asio::io_service m_io_service;
      boost::asio::ip::tcp::socket m_socket;
      boost::system::error_code m_error;

      std::string m_address, m_port;
      
      
    public:
      /** A constuctor.
       *  @param address The IP address of the device.
       *  @param port The port of the connection.
       */
      IPmanager(const std::string& address, 
		const std::string& port = "http");
    
      /** A destructor.*/
      ~IPmanager();
      
      /** Return the socket used in the connection.
       *  @return The current socket.
       */
      boost::asio::ip::tcp::socket& 
      socket()  {return m_socket;}
      
      /** Close the socket. */
      void 
      close() {m_socket.close();}
      
      /** Do not use Nagle algorithm (if implemented).
       * @param do_not_delay If true, the Nagle algorithm is not used.
       * @attention On Windows machines this command has no effect.*/
      void
      no_delay(const bool& do_not_delay = true);
      

      /** Keep the connection alive (if implemented).
       *  This keeps the connection alive if no data has been sent and received.
       * The default keep-alive timeout is 2 hours and the keep-alive interval is 1 second. 
       * The default number of keep-alive probes varies based on the version of Windows.
       * @param do_keep_alive If true, send requests to keep the connection alive.
       */
      void
      keep_alive(const bool& do_keep_alive = true);


      /** open the socket. 
       */
      void 
      open() ;

      /**Cancel all asynchronous operations associated with the socket.
       */
      void 
      cancel() {m_socket.cancel();}
      
      /* The following functions are overloads. */
       
      void 
      send(const std::string& cmd) ;
      
       
      std::string 
      recv(const std::string& cmd, 
	   const unsigned long& buffsize = 128, 
	   const bool& size_exactly = false) 
	throw( ) ;
    
       
      std::string 
      timed_recv(const std::string& cmd, 
		 const unsigned long& buffsize = 128, 
		 const double& seconds = 5, 
		 const bool& size_exactly = false) 
	throw(exception::timeout_exceeded);

       
      std::string 
      recv( const unsigned long& buffsize = 128, 
	    const bool& size_exactly = false)
	throw ( exception::exception_in_receive_you_must_resend_command) ;
      
       
      std::string 
      timed_recv( const unsigned long& buffsize = 128, 
		  const double& seconds = 5, 
		  const bool& size_exactly = false)
	throw(exception::timeout_exceeded,
	      exception::exception_in_receive_you_must_resend_command) ;
      
    };
  }
  
}

#pragma once
#include <string>
#include <boost/optional.hpp>
#include <boost/asio.hpp>

  /** The Institute of Cancer Research namespace.*/
namespace ICR{
  /** The communications namespace.*/
  namespace coms{
    
    /** Sleep the current thread.
     * @param ms The number of milliseconds to sleep for.
     */
    void sleep(unsigned long ms);

    
    inline void check_timeout(boost::optional<boost::system::error_code>* a, boost::system::error_code b) 
      { 
	*a = b; 
      } 

    inline void check_timeout_and_size(boost::optional<boost::system::error_code>* a, boost::system::error_code b, unsigned int* actually_read,unsigned int size ) 
      { 
	*a = b; 
	*actually_read = size;
	//std::cout<<"check size = "<<size<<std::endl;

      } 

    /** An interface to basic communication with a decive.*/
    class coms_manager
    {
    public:
      virtual 
      ~coms_manager();
      /** Send a command.
       *  @param cmd The command to send.
       *  @throws boost::system::system_error A boost asio error occured.
       */
      virtual 
      void 
      send(const std::string& cmd) = 0;

      /** Receive a command.
       *  @param cmd The command that requests then receive.  
       *  @param buffsize The number of bytes to receive.
       *  @param size_exactly True if exactly buffsize bytes are to be received.  If false, then fewer bytes may be received.
       *  @return The message received.
       *  @attention Note, this command calls the local send command and NOT the overloaded send command.
       *  @throws boost::system::system_error A boost asio error occured.
       */
      virtual 
      std::string 
      recv(const std::string& cmd, 
	   const unsigned long& buffsize = 128, 
	   const bool& size_exactly = false) = 0 ;
      
      /** Timed receive command.
       *  @param cmd The command that requests then receive.  
       *  @param buffsize The number of bytes to receive.
       *  @param seconds The number of seconds in the timeout.
       *  @param size_exactly True if exactly buffsize bytes are to be received.  If false, then fewer bytes may be received.
       *  @return The message received.
       *  @attention Note, this command calls the local send command and NOT the overloaded send command.
       *  @throws boost::system::system_error A boost asio error occured.
       *  @throws exception::timeout_exceeded The timeout was exceeded
       */
      virtual 
      std::string 
      timed_recv(const std::string& cmd,
		 const unsigned long& buffsize = 128, 
		 const double& seconds = 5, 
		 const bool& size_exactly = false) = 0;


      /** Receive a command.
       *  @param buffsize The number of bytes to receive.
       *  @param size_exactly True if exactly buffsize bytes are to be received.  If false, then fewer bytes may be received.
       *  @return The message received.
       *  @throws boost::system::system_error A boost asio error occured.
       */
      virtual 
      std::string 
      recv( const unsigned long& buffsize = 128, 
	    const bool& size_exactly = false) = 0 ;

      /** Timed receive command.
       *  @param buffsize The number of bytes to receive.
       *  @param seconds The number of seconds in the timeout.
       *  @param size_exactly True if exactly buffsize bytes are to be received.  If false, then fewer bytes may be received.
       *  @return The message received.
       *  @throws boost::system::system_error A boost asio error occured.
       *  @throws exception::timeout_exceeded The timeout was exceeded
       */
      virtual 
      std::string 
      timed_recv( const unsigned long& buffsize = 128, 
		  const double& seconds = 5, 
		  const bool& size_exactly = false) = 0;
    };

  }
}

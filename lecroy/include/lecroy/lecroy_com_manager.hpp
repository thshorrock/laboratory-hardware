

/** @file lecroy_com_manager.hpp Contains the VICP control for communicating to the lecroy scope. */

#pragma once

#include <string>
#include <iostream>
#include "coms_manager.hpp"
#include <boost/thread.hpp>

#include <bitset>

namespace ICR{
  
  namespace exception{
    
    /** General lecroy exception. */
    struct lecroy_exception{};
    /** Invalid communiciation method */
    struct invalid_communication_method : public lecroy_exception{};
  }
  namespace lecroy {
 


    /** A class handling the VICP header to the Lecroy scope */
    class lecroy_com_header
    {
    private:
      boost::array<unsigned char, 8> m_header;
      //operation bits
      bool m_data, m_remote, m_lockout, m_clear, m_srq, m_serial_poll, m_eoi;
      
      //version
      unsigned char m_version;
      
      //sequence number
      unsigned char  m_sequence_number;    // only set in version 1A and later

      void set_operation();
      void set_length(const std::string& cmd);
      void set_sequence(void);
      void set_version(void);
    public:
      /** Constuctor. 
       *@param eol whether to terminate with an end of line character ("\n").
       *@param lock whether to attempt to lock the scope.
       */
      lecroy_com_header(const bool& eol=true,
			const bool& lock = false );
     
      /** A desructor */
      virtual ~lecroy_com_header(){};
      
      /** Add the VICP  header to the command 
       * @param cmd The naked command
       * @return the command with the header.
       */
      std::string 
      add(const std::string& cmd);

      /** Set whether to add a clear bit to the header.
       * @param b True or false */
      void 
      set_clear(const bool& b) {m_clear = b; }
      
      /** Set whether to add a lockout bit to the header.
       * @param b True or false */
      void 
      set_lockout(const bool& b) {m_lockout = b; }
      
      /** Set whether to add an end of input bit to the header.
       * @param b True or false */
      void 
      set_eoi(const bool& b) {m_eoi = b; ;}
      
      /** Set whether to add a remote bit to the header.
       * @param b True or false */
      void 
      set_remote(const bool& b) {m_remote = b;}
      
      /** Set whether to add a "containing data" bit to the header.
       * @param b True or false */
      void 
      set_data(const bool& b) {m_data = b; }

      /** Get the number of data bits transmitted (following the header)
       * @param header The header to be read
       * @return the number of bits
       */
      unsigned long 
      get_length(const std::string& header);
      
      /** Is the eoi bit set in the header.
       * @param header The header.
       * @return true or false. */

      bool 
      is_eoi(const std::string& header) const;

    };
    
    namespace detail{

      template<class com_method>
      class lecroy_command_completed 
      {
	com_method& m_comm;
	lecroy_com_header& m_header;
	bool keep_going;
      public:
	lecroy_command_completed(com_method& comm, lecroy_com_header& header) 
	  : m_comm(comm), m_header(header), keep_going(true)
	{}

	
	void stop(){keep_going = false;}
	void
	operator()()
	{
	  while(  keep_going ){ 
	    try{
	      int opc_code = 0;
	      while( keep_going){
		m_comm.send(m_header.add("*OPC?\n"));
		std::string opc_bit =  m_comm.timed_recv(20,1,false);
		if (opc_bit.size() == 10){
		  opc_code  =  atoi(opc_bit.substr(8,1).c_str()) ;
		  if (opc_code == 1)  
		    return;
		}
		std::cout<<"opc l = "<<opc_code<<std::endl;
	      }
	    }
	    catch(ICR::exception::timeout_exceeded& e) {
	      e.debug_print();
	      std::cout<<"Lecroy timed out in send OPC?... trying to send again"<<std::endl;
	    }
	    catch(ICR::exception::exception_in_receive_you_must_resend_command& e) {
	      std::cout<<"LecroyLecroy receive exception caught (in send), resending command"<<std::endl;
	    } 
	    // ICR::coms::sleep(100);
	  
	  
	  }
	}
      };
    


      

    }

    typedef ICR::coms::IPmanager IP;
    typedef ICR::coms::serial_manager SERIAL;

    /** Lecroy VICP communications manager */
    template<class com_method>
    class lecroy_com_manager  //: com_method //coms::IPmanager
    {
      
    
      
      com_method comm;
      
      lecroy_com_header header;
      void clean_string(std::string& str, const std::string& badchar);
      protected:
      
    public:
      /** Open a communication with the lecroy scope.
       * @param device The locaton of the device.
       */
      lecroy_com_manager(const std::string& device);
      /** The distructor.*/
      virtual ~lecroy_com_manager();
      /** Send a command to the scope.  Most of the time this will be null terminated 
       *  @param cmd The command string..*/
      void send(const std::string cmd)  ;
      
      /** Send a request to the scope.  Most of the time this will be null terminated 
       * @param cmd The request string 
       * @param buffsize The size of the buffer to collect.  The returned string can be shorter than this size.
       * @param exact If exactly buffsize characters are to be collected
       * @return The returned string from the scope (missing the header). */
      std::string recv(const std::string cmd, const unsigned int& buffsize = 128, const bool& exact=false)   ;
      
      /* receive surplus bits.
       * If a communication fails the sometimes there are bits backed up.
       * this function reads until and end of file bit.
       * @return The surplus bits.
       */
      //std::string recv_surp() ;
      // std::string timed_recv(const std::string& cmd, const unsigned int& buffsize = 128, const double& seconds = 5, const bool& exact=false) ;
      void clear();

      /** Send a wait command.
       * @param seconds The timeout in seconds
       */
      void wait(const double& seconds = 0);

      /** Send a calibrate command.
       */
      void calibrate();
    };


    



  }

}



namespace ICR{

  namespace lecroy {
    template<class T>
    inline lecroy_com_manager<T>::lecroy_com_manager(const std::string& device)
      : T(device)
    {
      throw ICR::exception::invalid_communication_method();
    }

    template<class T>
   inline  lecroy_com_manager<T>::~lecroy_com_manager()
    {
    }
    
    template<>
    inline lecroy_com_manager<ICR::lecroy::IP>::lecroy_com_manager(const std::string& IPaddress)
      : comm(IPaddress, "1861"),
	header(false,false)
    {

      comm.no_delay();
      //request an anser to make sure all is well.
     
      // std::string reply;
      // while (reply.size()==0){
	// header.set_remote(true);
	// header.set_lockout(true);
	// header.set_clear(true);
	// header.set_eoi(false);
	// header.set_data(false);
	//ICR::lecroy::IP::send( header.add("") );
	//  send(""); //make remote
	 // header.set_data(true);
	 // header.set_eoi(true);
	 // header.set_clear(false);
	 // ICR::lecroy::IP::send( header.add("") );
      clear();
      send("CHDR OFF\n");
       
      // std::string ib= recv("INR?\n",5);
      // std::bitset<16> inr(atoi(ib.c_str()));
      // for(size_t i=0;i<16;++i){
      // 	std::cout<<"inr["<<i<<"]= "<<inr[i]<<std::endl;
      // }
	
	// try{
	//   reply= timed_recv("CMR?\n");

	// }
	// catch(...)
	//   {
	//     std::cout<< "no relpy from device... trying again... "<<std::endl;
	//   }
      // }
    }

      template<>
    inline lecroy_com_manager<ICR::lecroy::IP>::~lecroy_com_manager()
    {
      send("*CLS\n");
      header.set_data(false);
      header.set_eoi(false);
      header.set_remote(false);
      header.set_lockout(false);
      header.set_clear(true);
      comm.send( header.add("") );
      // send("",false); //make live
    }

    template<>
    inline lecroy_com_manager<ICR::lecroy::SERIAL>::lecroy_com_manager(const std::string& port)
      : comm(port)
    {
    }

    template<>
    inline lecroy_com_manager<ICR::lecroy::SERIAL>::~lecroy_com_manager()
    {
    }
  }
}

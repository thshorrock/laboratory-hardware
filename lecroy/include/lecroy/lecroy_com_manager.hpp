

/** @file lecroy_com_manager.hpp Contains the VICP control for communicating to the lecroy scope. */

#pragma once

#include <string>
#include <iostream>
#include "coms_manager.hpp"

#include <bitset>

namespace ICR{

  namespace lecroy {
    namespace exceptions{
      struct invalid_communication_method{};
    }
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
      lecroy_com_header(const bool& eol=true,const bool& lock = false );
      virtual ~lecroy_com_header(){};
      std::string add(const std::string& cmd);
      void set_clear(const bool& b) {m_clear = b; }
      void set_lockout(const bool& b) {m_lockout = b; }
      void set_eoi(const bool& b) {m_eoi = b; ;}
      void set_remote(const bool& b) {m_remote = b;}
      void set_data(const bool& b) {m_data = b; }

      unsigned long get_length(const std::string& header)
      {
	// int i = 4;
	// unsigned long val = 0;

	// for  ; i <= 7; i++ )
	//   val = ( val << 8 ) | header[ i ];

	std::bitset<32> bits; bits.reset();
	std::bitset<8> byte1(header[4]);
	std::bitset<8> byte2(header[5]);
	std::bitset<8> byte3(header[6]);
	std::bitset<8> byte4(header[7]);

	
	for(size_t i=0;i<8;++i){
	  bits.set(i, byte4.test(i));
	}

	for(size_t i=0;i<8;++i){
	  bits.set(i+8, byte3.test(i));
	}
	for(size_t i=0;i<8;++i){
	  bits.set(i+16, byte2.test(i));
	}

	for(size_t i=0;i<8;++i){
	  bits.set(i+24, byte1.test(i));
	}
	
	// for(size_t i=0;i<16;++i){
	//   std::cout<<"bits ["<<i<<"] = "<<bits.test(i)<<std::endl;
	// }

	return bits.to_ulong();
      }
      bool is_eoi(const std::string& header) const
      {
	//operation bit
	char op = header[0];
	return op & ( 1 << 0 );
      }

    };
    
    typedef ICR::coms::IPmanager IP;
    typedef ICR::coms::serial_manager SERIAL;

    /** Lecroy VICP communications manager */
    template<class com_method>
    class lecroy_com_manager  : com_method //coms::IPmanager
    {

      
      lecroy_com_header header;
      void clean_string(std::string& str, const std::string& badchar);

    protected:
    public:
      /** Open a communication with the lecroy scope.
       * @param IPaddress The IP address of the lecroy scope.
       */
      lecroy_com_manager(const std::string& device);
      /** The distructor.*/
      virtual ~lecroy_com_manager();
      /** Send a command to the scope.  Most of the time this will be null terminated @param cmd The command string.*/
      void send(const std::string& cmd, const bool& wait = false);
      
      /** Send a request to the scope.  Most of the time this will be null terminated 
       * @param cmd The request string 
       * @param buffsize The size of the buffer to collect.  The returned string can be shorter than this size.
       * @param exact If exactly buffsize characters are to be collected
       * @return The returned string from the scope (missing the header). */
      std::string recv(const std::string& cmd, const unsigned int& buffsize = 128, const bool& exact=false) ;
      std::string recv_surp() ;
      std::string timed_recv(const std::string& cmd, const unsigned int& buffsize = 128, const double& seconds = 5, const bool& exact=false) ;
      void clear();

      /** Send a wait command.
       * @param seconds The timeout in seconds
       */
      void wait(const double& seconds = 0);

    };


    



  }

}



namespace ICR{

  namespace lecroy {
    template<class T>
    inline lecroy_com_manager<T>::lecroy_com_manager(const std::string& device)
      : T(device)
    {
      throw ICR::lecroy::exceptions::invalid_communication_method();
    }

    template<class T>
   inline  lecroy_com_manager<T>::~lecroy_com_manager()
    {
    }
    
    template<>
    inline lecroy_com_manager<ICR::lecroy::IP>::lecroy_com_manager(const std::string& IPaddress)
      : coms::IPmanager(IPaddress, "1861"),
	header(false,false)
    {

      lecroy::IP::no_delay();
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
	ICR::lecroy::IP::send( header.add("") );
      // send("",false); //make live
    }

    template<>
    inline lecroy_com_manager<ICR::lecroy::SERIAL>::lecroy_com_manager(const std::string& port)
      : coms::serial_manager(port)
    {
    }

    template<>
    inline lecroy_com_manager<ICR::lecroy::SERIAL>::~lecroy_com_manager()
    {
    }
  }
}

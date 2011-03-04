#pragma once


#include <string>
#include <iostream>
#include "coms_manager.hpp"
#include <boost/smart_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

namespace ICR{

  namespace agilent {

    namespace detail {

      template<class com_method>
      class agilent_command_completed 
      {
	com_method& m_comm;
      public:
	agilent_command_completed(com_method& comm) 
	  : m_comm(comm)
	{}


	void
	operator()()
	{
	  int opc_code = 0;
	  while(opc_code!=1){
	    try{
	      while(opc_code!=1){
		opc_code  =  atoi(m_comm.timed_recv("*OPC?\n", 128,5).c_str()) ;
		
	      }
	    }
	    catch(ICR::exception::timeout_exceeded& e) {
	      e.debug_print();
	      std::cout<<"AGILENT timed out in send ...  trying to send  again"<<std::endl;
	      ICR::coms::sleep(100);
	    }
	    catch(ICR::exception::exception_in_receive_you_must_resend_command& e) {
	      std::cout<<"AGILENT receive exception caught (in send), resending command"<<std::endl;
	      ICR::coms::sleep(100);
	    } 
	    catch(...)
	      {
		std::cout<<"unknown error in completed thread"<<std::endl;

	      }
	  
	  }
	}

      };


    }


  }

}

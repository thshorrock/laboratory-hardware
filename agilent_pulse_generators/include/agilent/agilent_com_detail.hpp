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
	bool run;
      public:
	agilent_command_completed(com_method& comm) 
	  : m_comm(comm), run(true)
	{}

	void stop() {run =false;}
	void
	operator()()
	{
	  while(run){
	    try{
	      while(run){
		int opc_code  =  atoi(m_comm.timed_recv("*OPC?\n", 128,5).c_str()) ;
		if (opc_code == 1)
		  return;
	      }
	    }
	    catch(ICR::exception::timeout_exceeded& e) {
	      e.debug_print();
	      std::cout<<"AGILENT timed out in send OPC?...  trying to send  again"<<std::endl;
	    }
	    catch(ICR::exception::exception_in_receive_you_must_resend_command& e) {
	      std::cout<<"AGILENT receive exception caught (in send), resending command"<<std::endl;
	    } 
	    catch(...)
	      {
		std::cout<<"unknown error in agilent completed thread"<<std::endl;
	      }
	      ICR::coms::sleep(100);
	  
	  }
	}

      };


    }


  }

}

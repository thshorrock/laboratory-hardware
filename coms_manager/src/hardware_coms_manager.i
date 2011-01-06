
%module hardware_coms_manager
%include stl.i
%include "std_string.i"
 //%apply const std::string& {std::string* foo};
%{
#include "coms_manager/ipmanager.hpp"
/* Include in the generated wrapper file */
//typedef unsigned int size_t;
%}

//%naturalvar std::string;


/* Tell SWIG about it */
typedef unsigned int size_t;


//interface for rng
namespace ICR{
  namespace coms{

    class IPmanager{

    public:
      IPmanager(const std::string& address, const std::string& port = "80");
      void send(const std::string& cmd);
      std::string recv(const std::string& cmd, const  unsigned int& buffsize = 128) ;
      std::string timed_recv(const std::string& cmd, const  unsigned int& buffsize = 128, const double& seconds = 5) ;
    };
  }
}  

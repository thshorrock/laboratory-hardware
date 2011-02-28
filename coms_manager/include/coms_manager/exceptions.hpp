#pragma once
#include <iostream>
namespace ICR{
  /** Exception namespace */
  namespace exception{
    /**A general hardware exception
     * @attention This does not catch all exceptions possible in this library - the boost::system::system_error exceptions are also used.
     */
    struct hardware_exception{
      /**Print an error message.*/
      virtual 
      void 
      debug_print() const
      {std::cerr<<"A general hardware exception occured\n";}
    };
    
    /**The timeout was exceeded.*/
    struct timeout_exceeded : public hardware_exception
    {
      void debug_print()
      {std::cerr<<"timeout for communication was exceeded\n";}
    };
    
    struct serial_port_does_not_exist : public hardware_exception
    {
      std::string device;
      serial_port_does_not_exist(std::string d) : device(d) {}
      void debug_print()
      {std::cerr<<"serial port "<<device<<" does not seem to exist\n";}
    };
    
    
  }
}

#pragma once

#include "filename.hpp"
#include <boost/smart_ptr.hpp>
#include <fstream>
#include <string>

/** The Institute of Cancer Research Namespace */
namespace ICR{
  
  /** Create a README log file. */
  class readme{
  private:
    std::string m_filename;
    boost::shared_ptr<std::fstream> pOut;
    void init(const std::string& path) throw  (ICR::exception::directory_does_not_exist,
						  ICR::exception::path_is_not_a_directory) ;
    
  public:
    /** A constructor.
     * @param path The directory path 
     * @throws exception::directory_does_not_exist The directory you requested does not exist.
     * @throws exception::path_is_not_a_directory The string you passed exists but does not point to a directory. 
     * @attention To enter a Windows (backslash path), double backslashes are required. 
     * For example @code readme("C:\\data\\2011_1_1 "); @endcode
     */
    readme(const std::string path)
      throw (ICR::exception::directory_does_not_exist,
	     ICR::exception::path_is_not_a_directory) ;
    /** A copy constructor. @param other The file that you wish to copy */
    readme(const readme& other);
    /** A destructor. */
    ~readme();
    
    /** Write a title within the log.
     * @param title The title you want to enter.
     * @return The current (updated) readme log
     */
    readme& operator()(const std::string& title);

    /** Write a variable to the log.
     * These appear in the format varname = var
     * @tparam T the type (declared implicitly) to be put into the log file.
     * @param varname The variable name.
     * @param var the value of the variable.
     * @return The current (updated) readme log.
     */
    template<class T>
    readme& operator()(const std::string& varname, const T& var)
    {
      *pOut<<varname.c_str()<<" = "<<var<<std::endl;
      return *this;
    }
    
  };

}

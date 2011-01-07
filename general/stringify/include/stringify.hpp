#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>

/*! \mainpage Stringify Documentation 
 *
 *  \section Purpose
 *
 *  Convert various types into a string
 * 
 * \code
 #include "stringify.hpp"
 #include <iostream>
 int
 main  (int ac, char **av[])
 {
     //setup data
     double a_number = 1.3;
     std::string a_string = "The number is ";
     //use the stringify function
     a_string += ICR::stringify(a_number);
     
     std::string another_string = ICR::stringify_with_zeros(13,4);
     //print result
     std::cout<<a_string<<std::endl;     
     std::cout<<another_string<<std::endl;
 }
 * \endcode 
 * will print "The number is 1.3" and "0013".
 * \section Acknowledgements
 * Based on the answer to the frequently asked question by Marshall
 * Cline, 
 * available here:
 * http://www.parashift.com/c++-faq-lite/misc-technical-issues.html#faq-39.1
 * 
 *  \section Install
 *  Setup bjam (which can be downloaded from http://www.boost.org/)
 *  and then go to the `icr' root directory and type:
 *  \code bjam --v2 \endcode
 *
 *  
 *  \subsection Dependances
 *  To run the tests the Boost Test library are required.
 *  This should automatically be build, or alternatively the most
 *  recent versions can be obtained from http://www.boost.org/.
 *
 *
 */


/*! \file stringify.hpp
 *  \brief The stringify function  is defined.
 *
 */





//! The main Institute of Cancer Research namespace
namespace ICR{
  //! The error exception class
  namespace error{
    //! The BadConversion error class
    class BadConversion : public std::runtime_error {
    public:
      BadConversion(const std::string& s)
	: std::runtime_error(s)
      { }
    };
  }
  
  /*! \brief The stringify funtion

    \tparam T The type (declaired implicitly) to be converted into a
    string
    \param x The value to be converted into a string 
    \return The stringified result
   */
  template<class T>
  std::string stringify (T x) throw(error::BadConversion)  {
    std::ostringstream o;
    if (!(o << x))
      throw error::BadConversion("stringify()");
    return o.str();
  }
  
  /*! Stringify a function with preceeding zeros.

    \tparam T The type (declaired implicitly) to be converted into a
    string
    \param x The value to be converted into a string
    @param width The width of the resulting string
    \return The stringified result
   */
  template<class T>
  std::string stringify_with_zeros (T x, size_t width) throw(error::BadConversion)  {
    std::ostringstream o;
    o.fill('0');
    o.width(width);
    if (!(o <<  x))
      throw error::BadConversion("stringify()");
    return o.str();
  }

  /** Turn a string into another (template specified) type. 
   * @tparam The type to return.
   * @param s The string to destringify.
   * @return The type requested */
  template<class T>
  T destringify (std::string s) throw(error::BadConversion)  {
    std::istringstream stream(s);
    T x;
    if (!(stream >> x))
      throw error::BadConversion("destringify()");
    return x;
  }
  
} //namespace icr

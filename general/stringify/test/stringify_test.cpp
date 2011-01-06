#include "stringify.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/bind.hpp>

namespace icr = institute_of_cancer_research;
using namespace boost::unit_test;
using namespace icr;

void add_double()
{
  double a_number = 1.3;
  std::string a_string = "The number is ";
  //use the stringify function
  a_string += stringify(a_number);
  BOOST_CHECK(a_string == "The number is 1.3"); //check it now exists
  
}


//____________________________________________________________________________//

test_suite*
init_unit_test_suite( int argc, char* argv[] ) 
{
    framework::master_test_suite().
      add( BOOST_TEST_CASE( &add_double ) );
    return 0;
}




#include <boost/progress.hpp>
#include <boost/timer.hpp>

#define BOOST_TEST_MODULE filename_tests
#include <boost/test/unit_test.hpp>

#include "get_filename.hpp"
#include <string>
#include <vector>
#include <list>
namespace icr = institute_of_cancer_research;
using std::string;
using std::vector;
using std::list;



//____________________________________________________________________________//




// a predicate implemented as a function:
bool not_a_dat_file (const string& filename) { 
  icr::filename::file f(filename);
  if (f.extension()!=".dat") return true; else return false; 
}



BOOST_AUTO_TEST_SUITE( DIRECTORY_SUITE )
BOOST_AUTO_TEST_CASE( directory_constructor_test )
{ 
  
  string dir = "/mirror/progs/cpp/icr-10.02/src/libs/general/get_filename/test/" ;// "/mirror/cu/data/tom/beamplots/72db_lft/";
  icr::filename::directory d(dir);
  
  //get the list of all the filenames in the directory
  list<string> files = d.get_filenames();

  //remove the file from the list if its not a ".dat" file
  files.remove_if (not_a_dat_file);

  
  //iterate over the list (i.e. process each one in turn..
  list<string>::iterator pfilename = files.begin();  //from the beginning
  while(pfilename!=files.end()){  //to the end

    //put the current file in a file object
    icr::filename::file f(*pfilename); 

    //decode the filename into a vector of strings
    std::vector<string> f_decode = f.decode();  
    
    //output the results
    //std::cout<<"file "<<f.full()<<" is decoded as follows:"<<std::endl  
    //         <<"\t";
    //for(size_t i=0;i<f_decode.size()-1;++i){
    // std::cout<<"'"<<f_decode[i]<<"', ";
    //}
    //the last one without a final comma
    //std::cout<<"'"<<f_decode[f_decode.size()-1]<<"'"<<std::endl;

    

    ++pfilename;  //move onto the next file.
  }


  // goto a new directory "./processed" (the directory is made if necessary)
  //d/="processed";
  
  

}



BOOST_AUTO_TEST_SUITE_END()


//____________________________________________________________________________//




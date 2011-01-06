#include "filename/get_filename.hpp"

/** \mainpage get_filename Documentation 
 *
 *  \section Purpose
 *
 *  find and analyse filenames
 *
 * \section Example
 *
 * Often data files are have experimenal variables encoded within them,
 * for example, a file may be called "data_x0.02y0.03z-2.2e-6.dat", say,
 * where x, y, z are three variables, with the corresponding values given.
 * The task to analyse such files is to decode the filename to extract the numbers.
 * This is done in the following example.
 *
 * First a string containing a directory is given.
 * All the files in the directory are then found and stored in a std::list container.
 * The extension is easily analysed, and files that are not ".dat" files removed from the list
 * (the files themselves are not removed!).
 * Then, the filename is decoded.
 * This simply returns a std::vector<string> where the filename has been broken into 'character parts'
 * and 'numeric' parts.
 * For example, the filename "data_x0.02y0.03z-2.2e-6.dat" will return a vector
 * containing "data_x", "0.02", "y", "0.03", "z" and "-2.2e-6".
 * To analyse the x-variable, then,
 * you need to analayse the first element of the decoded filename 
 * (the function a2f -- character to float) will most likely be useful here...
 * 
 * \code
 * #include "filename.hpp"
 * #include <string>
 * #include <vector>
 * #include <list>
 * using std::string;
 * using std::vector;
 * using std::list;
 *
 * // test the extension of the string of the filename
 * bool not_a_dat_file (const string& filename) { 
 *   //make a file object so extention may be tested
 *   ICR::filename::file f(filename);
 *   //test to see if the extention of the string is not a ".dat", return true in this case.
 *   if (f.extension()!=".dat") return true; else return false; 
 * }
 *
 * int
 * main  (int ac, char **av[])
 * {
 *
 *   //The directory to search
 *   string dir = "/mirror/progs/cpp/icr-10.02/src/libs/general/get_filename/test/" ;
 *
 *   //create a directory object to analyse the directory
 *   ICR::filename::directory d(dir);
 *
 *   //create a list of all the filenames in the directory
 *   list<string> files = d.get_filenames();
 *
 *   //remove the files from the list that are  not ".dat" files
 *   files.remove_if (not_a_dat_file);
 *
 *   //iterate over the list (i.e. process each one in turn..
 *   list<string>::iterator pfilename = files.begin();  //from the beginning
 *   while(pfilename!=files.end()){  //to the end
 *
 *     //put the current file in a file object
 *     ICR::filename::file f(*pfilename); 
 *
 *     //decode the filename into a vector of strings
 *     std::vector<string> f_decode = f.decode();  
 *  
 *     //output the results
 *     std::cout<<"file "<<f.full()<<" is decoded as follows:"<<std::endl  
 *              <<"\t";
 *     for(size_t i=0;i<f_decode.size()-1;++i){
 *       std::cout<<"'"<<f_decode[i]<<"', ";
 *     }
 *     //the last one without a final comma
 *     std::cout<<"'"<<f_decode[f_decode.size()-1]<<"'"<<std::endl;
 *
 *     ++pfilename;  //move onto the next file.
 *   }
 *
 * }
 * \endcode 
 * If the directory contains the file "data_x0.02y0.03z-2.2e-6.dat", say,
 * then the output will be 
 * \verbatim
 file /mirror/progs/cpp/icr-10.02/src/libs/general/get_filename/test/data_x0.02y0.03z-2.2e-6.dat is decoded as follows:
        'data_x', '0.02', 'y', '0.03', 'z', '-2.2e-6
 \endverbatim
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


/*! \file include/get_filename.hpp
 *  \brief All the definitions are pooled here.
 *
 */

/*! \file include/get_filename/get_filename.hpp
 *  \brief The get_filename class and the directory class are defined.
 *
 */





#include "filename.hpp"
#include "stringify.hpp"
#include "lecroy.hpp"

#include <boost/program_options.hpp>
#include <list>
#include <vector>

using namespace ICR;
using std::list;

namespace po = boost::program_options;

// test the extension of the string of the filename
bool is_not_a_gnuplot_file (const string& filename) { 
  //make a file object so extention may be tested
  file f(filename);
  std::string stub = f.stub();
  if (stub.substr(stub.size()-3,3)!="_GP") return true; else return false; 
}
bool is_a_gnuplot_file (const string& filename) { 
  //make a file object so extention may be tested
  file f(filename);
  std::string stub = f.stub();
  if (stub.substr(stub.size()-3,3)=="_GP") return true; else return false; 
}

bool not_a_dat_file (const string& filename) { 
  file f(filename);
  if (f.extension()!=".dat") return true; else return false; 
}

class out_of_range
{
  std::pair<size_t,size_t> m_range;
public:
  out_of_range( const std::pair<size_t,size_t>& range)
    : m_range(range) {}
  bool operator()(const string& filename) ;
};

bool
out_of_range::operator()(const string& filename) {
  file f(filename);
  std::cout<<"filename to decode = "<<f.full()<<std::endl;

  std::vector<string> numbers = f.decode();
  std::cout<<"numbers size = "<<numbers.size()<<std::endl;

  int id = destringify<int>( numbers[0] );
  
  if (id< m_range.first || id> m_range.second)
    return true;
  else
    return false;
}

class do_average
{
  size_t m_n;
  lecroy::aline m_sum;
public:
  do_average() : m_n(0), m_sum() {};
  void operator() (const string& filename);
  lecroy::aline result() const {return m_sum/m_n;}
};

void
do_average::operator() (const string& filename)
{
  //load the file
  lecroy::aline current;
  std::cout<<"filename = "<<filename<<std::endl;

  load(current,filename);
  // std::cout<<"has loaded"<<std::endl;
  if (m_n==0) 
    m_sum = current; //the first time
  else 
    m_sum+=current;
  ++m_n;

}

int
main  (int ac, char **av)
{


  std::string dir;
  std::pair<size_t, size_t> range;

  //************************************************************* 
  //The directory and file readin properties
  po::options_description generic("Generic options");
  generic.add_options()
    ("version,v", "print version string")
    ("help,h", "produce help message")    
    ;

  
  po::options_description file_options("File options");
  file_options.add_options()
    ("directory,d",
     po::value<string>(&dir)->default_value("."),   
     "The directory you want to convert.")
    ("from",
     po::value<size_t>(&range.first) -> default_value(0),
     "The first file to include in the average")
    ("to",
     po::value<size_t>(&range.second) -> default_value(0),
     "The last file to include in the average. If zero then the number of files -1 will be used.")
    ;

  po::options_description cmdline_options;
  cmdline_options.add(generic).add(file_options);
  

  po::options_description visible("Allowed options");
  visible.add(generic).add(file_options);

  
  po::variables_map vm;
  po::store(po::parse_command_line(ac, av,visible ), vm);
  po::notify(vm);    
      
  if (vm.count("help")) {
    std::cout <<visible  << "\n";
    return 1;
  }


  if (vm.count("version")) {
    std::cout << "version 0.1.\n";
    return 1;
  } 
  //***********************************************************

  
  //create a directory object to analyse the directory
  directory d(dir);
  
  //create a list of all the filenames in the directory
  list<string> files = d.get_filenames();
  

  list<string>::iterator it = files.begin();
  // for(size_t i=0;i<files.size();++i){
  //   std::cout<<*it<<std::endl;
  //   ++it;
  // }


  if (range.second==0) //not set
    range.second = files.size() -1;
  out_of_range is_not_in_range(range);
  
  

  //remove the files from the list that are  not ".dat" files

  files.remove_if (not_a_dat_file);
  files.remove_if (is_a_gnuplot_file);
  // files.remove_if (is_not_in_range) ;


  
  // std::cout<<"ls files 2= "<<std::endl;
  // it = files.begin();
  // for(size_t i=0;i<files.size();++i){
  //   std::cout<<*it<<std::endl;
  //   ++it;
  // }


  do_average average;
  average("/data/2011/01/09/water/imaging_on_first_burst/imaging_wave_only/./imaging_solo_00.dat");
  //   //  average = std::for_each(files.begin(), files.begin()+1, average);
  lecroy::aline the_average = average.result();

  file average_file(d,
		    "the_average_GP.average");

  save_gnuplot(the_average, average_file.full() );

}

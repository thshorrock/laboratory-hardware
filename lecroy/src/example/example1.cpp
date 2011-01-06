#include "lecroy.hpp"

using namespace ICR::lecroy;
using namespace ICR::lecroy::location;
int
main  (int ac, char **av)
{
  
  lecroy_64Xi lc("10.0.0.10");
  

  //lc.reset();
  //lc.sequence(2,500);
  
  
  lecroy_file file = 
    lc.get_waveform(C2);
  // // file.save("test.dat");
  
  // aline a = file.get_data1(0);
  // aline b = file.get_data1(0);
  // a.save_ascii("testa.txt");
  // b.save_ascii("testb.txt");
    
  // aline c = a-b;
  // c.save_ascii("testc.txt");
    
  
  
  // std::ofstream ofs("test_archive.txt");
  // boost::archive::text_oarchive oa(ofs);
  // // write class instance to archive
  // oa << a;
}

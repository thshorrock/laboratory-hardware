
#include "readme/readme.hpp"
#include "filename.hpp"



ICR::readme::readme(const std::string path)
{
  init(path);
}

ICR::readme::readme(const readme& other)
{
  init(other.m_filename);
}

void
ICR::readme::init(const std::string& path)
{
  m_filename = path;
  ICR::directory d;
  try{
    d.cd(path); //try to go to the path
    ICR::file f(d, "README.log");
    pOut = boost::shared_ptr<std::fstream>
      (new std::fstream(f.full().c_str(), std::ios_base::out ));
	
  }
  catch(exception::filename_exception& e){
    e.debug_print();
  }
}


ICR::readme& 
ICR::readme::operator()(const std::string& title)
{
  *pOut<<std::endl<<std::endl<<title.c_str()<<std::endl;
  return *this;
}

ICR::readme::~readme()
{
  pOut->close();
}



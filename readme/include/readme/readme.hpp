#pragma once



#include <boost/smart_ptr.hpp>
#include <fstream>
#include <string>

namespace ICR{

  class readme{
  private:
    std::string m_filename;
    boost::shared_ptr<std::fstream> pOut;
    void init(const std::string& path);
    
  public:
    readme(const std::string path);
    readme(const readme& other);
    ~readme();
    
    // readme& operator()(std::string path);

    template<class T>
    readme& operator()(const std::string& varname, const T& var)
    {
      *pOut<<varname.c_str()<<" = "<<var<<std::endl;
      return *this;
    }
    
  };

}

#include "filename.hpp"
#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations
#include <iostream>               // for std::cout


#include <stdexcept>
namespace fs = boost::filesystem;


ICR::file::file(const std::string filename) throw(exception::file_does_not_exist)
  : m_path(filename)
{
  if ( !fs::exists( full() ) ) {
    throw exception::file_does_not_exist();
  }
}


ICR::file::file(const directory& dir, const std::string filename) 
{
  m_path = dir.m_path;
  m_path /= boost::filesystem::path(filename);
}



ICR::directory::directory(const std::string directory) throw(exception::directory_does_not_exist)
{
  try{
    pushd(directory);
  }
  catch( exception::path_is_not_a_directory& e) {
    //goto directory of the file pointed at
    std::cout<<m_path.file_string()<<" is not directory"<<std::endl;
    m_path = m_path.parent_path();
    std::cout<<"directory path set to "<<m_path.file_string() <<std::endl;
  }
  catch(exception::directory_does_not_exist& e){
    e.debug_print();
    throw; //not sure what to do.
  }
 
}

void
ICR::directory::cd(const std::string path)  throw (exception::directory_does_not_exist,
						   exception::path_is_not_a_directory)
{

  boost::filesystem::path p(path);
  //check to see the path exists
  if ( !fs::exists( p  ) ) 
    throw exception::directory_does_not_exist();
  
  //check to see whether its a directory
  if ( !fs::is_directory( p ) )
    throw exception::path_is_not_a_directory();
  
  //everything is okay
  m_path = p;
}



void
ICR::directory::pushd(const std::string path)  throw (exception::directory_does_not_exist,
						   exception::path_is_not_a_directory)
{
  try{
    cd(path);
    //everything is okay
    m_push_list.push_back(m_path.file_string());
  }
  catch( exception::path_is_not_a_directory& e) {
    e.debug_print();
    throw; //not sure what to do.
  }
  catch(exception::directory_does_not_exist& e){
    e.debug_print();
    throw; //not sure what to do.
  }
}

void
ICR::directory::popd() throw (exception::push_list_empty,
			      exception::directory_does_not_exist,
			      exception::path_is_not_a_directory)
{
  if (m_push_list.size()==0) throw exception::push_list_empty();
  
  m_push_list.pop_back();
  std::string last_location = m_push_list.back();

  cd(last_location);
}


void
ICR::directory::mkdir(const std::string path)
{
  
  boost::filesystem::path p(path);
  //check to see the path exists
  if ( fs::exists( p  ) ) 
    return;

  boost::filesystem::create_directory (p);
  
}



std::list<std::string>
ICR::directory::get_filenames() const
{
  std::list<std::string> files;
  fs::directory_iterator end_iter;
  for ( fs::directory_iterator dir_itr( m_path );
        dir_itr != end_iter;
        ++dir_itr )
    {
      try {
        if ( fs::is_regular_file( dir_itr->status() ) ){
          fs::path p =  system_complete(dir_itr->path());
          files.push_back(p.file_string());
        }
      }
      catch ( const std::exception & ex ){
        std::cerr << dir_itr->path().filename() << " " << ex.what() << "\n";
      }
    }
  return files;
}

std::list<std::string>
ICR::directory::get_subdirectories() const
{
  std::list<std::string> dirs;
  fs::directory_iterator end_iter;
  for ( fs::directory_iterator dir_itr( m_path );
        dir_itr != end_iter;
        ++dir_itr )
    {
      try {
        if ( fs::is_directory( dir_itr->status() ) ){
          fs::path p =  system_complete(dir_itr->path());
          dirs.push_back(p.file_string());
        }
      }
      catch ( const std::exception & ex ){
        std::cerr << dir_itr->path().filename() << " " << ex.what() << "\n";
      }
    }
  return dirs;
}

std::list<std::string>
ICR::directory::get_filenames_recursive() const
{
  std::list<std::string> files;
  std::list<std::string> dirs;
  fs::directory_iterator end_iter;
  dirs.push_back(m_path.file_string());
  while(dirs.size()>0){
    for ( fs::directory_iterator dir_itr( dirs.front() );
	  dir_itr != end_iter;
	  ++dir_itr )
      {
	try {
	  if ( fs::is_regular_file( dir_itr->status() ) ){
	    fs::path p =  system_complete(dir_itr->path());
	    files.push_back(p.file_string());
	  }
	  if ( fs::is_directory( dir_itr->status() ) ){
	    fs::path p =  system_complete(dir_itr->path());
	    dirs.push_back(p.file_string());
	  }
	
	}
	catch ( const std::exception & ex ){
	  std::cerr << dir_itr->path().filename() << " " << ex.what() << "\n";
	}
      }
    dirs.pop_front();
  }
  return files;
}



std::vector<std::string>
ICR::file::decode(void)
{
  
  std::vector<string> results;
  std::string text = stub();
  std::cout<<"stub = "<<text<<std::endl;

  const string regx = "(([[:alpha:]_=\\s]+)|([-+]?[0-9]+\\.?[0-9]*[eE]?[-+]?[0-9]*))+";
  boost::regex e(regx);
  boost::smatch what;
  if(boost::regex_match(text, what, e, boost::match_extra))
    {

      if (what.size()>=1){
        for(size_t j = 0; j < what.captures(1).size(); ++j)
          {
	    std::cout<<"what = "<<what.captures(1)[j]<<std::endl;
            results.push_back(what.captures(1)[j]);
          }
      }
   }
  
  return results;
  
}

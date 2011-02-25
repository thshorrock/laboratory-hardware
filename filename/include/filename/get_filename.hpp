

#pragma once
#include <boost/regex.hpp>
#include <iostream>
#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations
#include <string>
#include <vector>
#include <list>

/** ICR namespace */
namespace ICR{

  namespace exception{
    /** A general filename exception.*/
    struct filename_exception{
      virtual void debug_print(){std::cerr<<"general filename exception\n";}
    };
    /** The file does not exist */
    struct file_does_not_exist : public filename_exception 
    {
      virtual void debug_print(){std::cerr<<"file does not exist\n";}
    };
    /** The directory does not exist */
    struct directory_does_not_exist : public filename_exception 
    {
      virtual void debug_print(){std::cerr<<"directory does not exist\n";}
    };
    
    /**The path is not a directory.
     * The path exists but is a filetype that is not a directory 
     */
    struct path_is_not_a_directory  : public filename_exception {
      virtual void debug_print(){std::cerr<<"path exists but is not a directory\n";}
    };
    
    /** Cannot pop out of directory. You attempted to pop back into a directly without a push */
    struct push_list_empty : public filename_exception {};
  }
  using std::string;
  using std::vector;
  using std::list;
  class directory; //forward declaration

  /**A class to obtain and analyse filenames.
   * Often experimental variables written into filenames,
   * this class extracts that information */
  class file{
    boost::filesystem::path m_path;
  public:
    /** A constructor.
     *  The filename to analyse is provided as a string.
     * @param filename The filename to analyse.
     */
    file(const string filename) throw(exception::file_does_not_exist) ;

    /** A constructor.
     *  The filename to analyse is provided as a string.
     * @param dir  The directory in which to the filename is to be located.
     * @param filename The filename to analyse.
     */
    file(const directory& dir, const string filename);

    /** Pull out the numbers within the filename.
     * These often correspond to experimental variables.
     */
    vector<string>
    decode(void);

    /** Return the stub of the filename.
     * (i.e. before the final . (dot) marking an extension.
     * @return A string of the stub.
     */
    string 
    stub() const {return m_path.stem();};
    
    /** Remove - delete - the file from the hard disk.
     */
    void remove() {boost::filesystem::remove (m_path);}

    /** Return the extension of the filename.
     * (i.e. after the final . (dot) marking an extension.
     * @return A string of the extension.
     */
    string 
    extension() const {return m_path.extension();};

    /** Return the full directory of the filename.
     * @return A string of the directory.
     */
    string 
    get_directory() const {
      if (m_path.parent_path().file_string()  != "") {
	return m_path.parent_path().file_string() + "/";
      }
	
      return m_path.parent_path().file_string();
    };

    /** Return the full filename.
     * @return A string of the full path.
     */
    string 
    full() const {boost::filesystem::path p = boost::filesystem::system_complete(m_path); return p.file_string();};

    friend class directory;

  };
  /** A class to handle directory information.
   */
  class directory{
    boost::filesystem::path m_path;
    std::deque<std::string> m_push_list;
  public:
    
    /** A constructor.
     * An empty directory class.
     */
    directory(){};
    
    /** A constructor.
     * Go to the directory and throw if it doesn't exist.
     * @param directory The directory to consider.
     */
    directory(const string directory) throw(exception::directory_does_not_exist);
      
    /** Get all the files in the directory.
     * @return A list containing all the filenames
     */
    list<string>
    get_filenames() const;
    
    /** Get all the subdirectories.
     * @return A list containing all the filenames
     */
    list<string>
    get_subdirectories() const;
    
    /** Get all the files in the directory and subdirectories.
     * @return A list containing all the filenames
     */
    list<string>
    get_filenames_recursive() const;
    

    /** Get the pathname of the directory.
     */
    string
    get_directory() const {return m_path.file_string();};
      
    /** Goto directory.
     * @param path The directory to goto 
     * @exception exception::directory_not_exist The requested directory does not exist
     */
    void cd(const std::string path)  throw (exception::directory_does_not_exist,
					    exception::path_is_not_a_directory);
      
    /** Push into directory
     * @param path The directory to goto 
     * @exception exception::directory_not_exist The requested directory does not exist
     */
    void pushd(const std::string path)  throw (exception::directory_does_not_exist,
					       exception::path_is_not_a_directory);
      
    /** Pop back to previous directory.
     * @exception exception::push_list_empty No directory to pop back into.
     * (A pop without a push).
     */
    void popd() throw (exception::push_list_empty,
		       exception::directory_does_not_exist,
		       exception::path_is_not_a_directory);
      
    /** Make a directory.
     * Does nothing if the directory already exists.
     * @param path The directory to make.
     */
    void mkdir(const std::string path);


    /** Get the boost::filesystem::path.
     * @return The boost::filesystem::path.
     */
    boost::filesystem::path get_boost_path() const {return m_path;}
	
    friend class file;
  };

  
}



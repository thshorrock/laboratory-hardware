#pragma once

#include "lecroy_header.hpp"
#include <iostream>
#include <fstream>
#include <bitset>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/version.hpp>
namespace ICR{

    namespace exception{
      /** Alines are not the same size. */
      struct alines_not_of_same_size {};
      /** Aline does not exist.
       * This usually happens when the aline requested is greater than the number of sequences (sequence mode).
       */
      struct aline_does_not_exist {};
    }
  
  namespace lecroy{


    /** A class wrapping a single aline */
    class aline
    {
      
    protected: 
      boost::shared_array<double> m_t, m_trace;
      size_t m_size;

      friend class boost::serialization::access;
      template<class Archive>
      void save(Archive & ar, const unsigned int version) const;
      
      template<class Archive>
      void load(Archive & ar, const unsigned int version);
      
      BOOST_SERIALIZATION_SPLIT_MEMBER()
    public:
      
      /** Constructor.
       **/
      aline();
      
      /** Constructor.
       * @param time An array of times
       * @param trace An array of data
       * @param size The number of elements
       **/
      aline(const boost::shared_array<double>& time,
	    const boost::shared_array<double>& trace,
	    const size_t& size);

      /** Copy constructor. @param other the aline to copy */
      aline(const aline& other);

      /** Assignment operator. 
       * @param other the aline to copy 
       * @return The assigned aline.
       */
      aline& operator=(const aline& other);


      
      /** Get the size of the aline.
       * @return The number of elements.
       */
      size_t size() const {return m_size;};
      
      /** Get the data from the aline.
       * @param i The index of the data
       * @return The data at that index.
       */
      const double& operator[](const size_t& i) const { return m_trace[i]; }

      /** Get the data from the aline.
       * @param i The index of the data.
       * @return The data at that index.
       */
      double& operator[](const size_t& i){return m_trace[i];}



      /** Get a time point.
       * @param i The index of the time point
       * @return The data at that index.
       */
      const double& time(const size_t& i) const {return m_t[i];}


      /** Get a time point.
       * @param i The index of the time point
       * @return The data at that index.
       */
      double& time(const size_t& i)             {return m_t[i];}


      /** Add an aline to the present aline. 
       * @param other The aline to add to the preset.
       * @return The resultant aline.
       * @attention The two alines must be the same size.
       */
      aline&
      operator+=(const aline& other);
      
      
      /** Subtract an aline from the present aline. 
       * @param other The aline to add to the preset.
       * @return The resultant aline.
       * @attention The two alines must be the same size.
       */
      aline&
      operator-=(const aline& other);

      /** Multiply  by a scalar
       * @param other The aline to add to the preset.
       * @return The resultant aline.
       */
      aline&
      operator*=(const double& other);
      
      /** divide  by a scalar
       * @param other The aline to add to the preset.
       * @return The resultant aline.
       */
      aline&
      operator/=(const double& other) {return *this*=(1.0/other);}

    };

   /** Save the aline in binary format @param a The aline to save @param filename The filename to use */
    void save(const aline& a, const std::string filename);
    /** Load the aline in binary format  @param a The aline to save @param filename The filename to use*/
    void load(aline& a, const std::string filename);

    /** Save the aline in ascii format. @param a The aline to save @param filename The filename to use */
    void save_ascii(const aline& a, const std::string filename);

    /** Save the aline in gnuplot format. @param a The aline to save @param filename The filename to use */
    void save_gnuplot(const aline& a, const std::string filename);
    
    /** Load the aline from gnuplot format. @param a The aline to save @param filename The filename to use */
    void load_gnuplot( aline& a, const std::string filename);

    template<class Archive>
    void
    aline::save(Archive  & ar, const unsigned int version) const
    {
      ar & m_size;
      boost::serialization::array<double> sa_time = 
	boost::serialization::make_array<double>(m_t.get(), m_size);
      boost::serialization::array<double> sa_trace = 
	boost::serialization::make_array<double>(m_trace.get(), m_size);
      ar & sa_time;
      ar & sa_trace;
      // for(size_t i=0;i<m_size;++i){
      // 	ar& m_t[i];
      // 	ar& m_trace[i];
      // }

    }
  
    template<class Archive>
    void 
    aline::load(Archive & ar, const unsigned int version)
    {
      
      // char buffer;
      // std::ifstream pIn("/data/2011/01/09/water/imaging_on_first_burst/imaging_wave_only/./imaging_solo_00.dat");
      // pIn.seekg (30, std::ios::beg); 
      // pIn.read( &buffer ,1);
      // pIn.close();
      // std::cout<<"buffer = "<<(int) buffer<<std::endl;

      // if ( (int) buffer == 7) 
      // 	{
      // 	  char new_buff = 0x06;
      // 	  std::ofstream pOut("/data/2011/01/09/water/imaging_on_first_burst/imaging_wave_only/./imaging_solo_00.dat_test", std::ios::binary );
      // 	  pOut.seekp (30, std::ios::beg); 
      // 	  pOut.write(&new_buff,1);
      // 	  pOut.close();
	  
      // 	}

      // std::cout<<"size0 = "<<m_size<<std::endl;
      // char buffer2;
       // ar & buffer2;
       // ar & buffer2;
       // ar & buffer2;
      // std::cout<<"sizeof short = "<<sizeof(unsigned short)<<std::endl;
      // std::cout<<"sizeof int = "<<sizeof(unsigned int)<<std::endl;

      // std::cout<<"sizeof size_t = "<<sizeof(size_t)<<std::endl;
      // unsigned int size;
      ar & m_size;
      //m_size = size;
      //std::cout<<"size1 = "<<m_size<<std::endl;
      
       std::bitset<64> size_bytes(m_size);
       std::string string_bytes = size_bytes.to_string();
       //std::cout<<"bytes = "<<string_bytes<<std::endl;

       // for(size_t i=0;i<64;++i){
       // 	 std::cout<<"string bytes ["<<i<<"] = "<<string_bytes[i]<<std::endl;
       // }
       
       //std::cout<<"should be "<<std::endl;


       std::bitset<64> size_bytes_cor(250002);
       std::string string_bytes_cor = size_bytes_cor.to_string();
       //std::cout<<"bytes = "<<string_bytes_cor<<std::endl;
       // for(size_t i=0;i<64;++i){
       // 	 std::cout<<"string bytes ["<<i<<"] = "<<string_bytes[i]<<std::endl;
       // }
      // char* bits = new char[9];
      // for(size_t i=0;i<8;++i){
      //  	bits[i]=0;
      // }
      // std::cout<<"here0"<<std::endl;
      // //bits = (char*) m_size;
      // std::cout<<"here"<<std::endl;

      // for(size_t i=0;i<8;++i){
      //  	std::cout<<"string bytes ["<<i<<"] = "<<bits[i]<<std::endl;
      // }
      // delete[] bits;

      
      m_t     = boost::shared_array<double>(new double[m_size]);
      m_trace = boost::shared_array<double>(new double[m_size]);

      boost::serialization::array<double> sa_time = 
      	boost::serialization::make_array<double>(m_t.get(), m_size);
      boost::serialization::array<double> sa_trace = 
      	boost::serialization::make_array<double>(m_trace.get(), m_size);
      //std::cout<<"get times"<<std::endl;

      ar & sa_time;
      //std::cout<<"get data"<<std::endl;
      ar & sa_trace;
      //std::cout<<"done"<<std::endl;
      
      // for(size_t i=0;i<m_size;++i){
      // 	ar& m_t[i];
      // 	ar& m_trace[i];
      // }
    }
    
    /** Add two alines together
     * @param a The first aline.
     * @param b The second aline.
     * @return The resultant aline.
     */
    aline 
    operator+(const aline& a, const aline& b);

    /** Subtract two alines.
     * @param a The first aline.
     * @param b The second aline.
     * @return The resultant aline.
     */
    aline 
    operator-(const aline& a, const aline& b);

    /** Multiply  every element of an aline by a constant
     * @param a The  aline.
     * @param b The constant.
     * @return The resultant aline.
     */
    inline
    aline 
    operator*(const aline& a, const double& b)
    {
      aline tmp(a); tmp *= b; return tmp;
    }
    
    /** Multiply every element of  an aline by a constant
     * @param a The constant.
     * @param b The  aline.
     * @return The resultant aline.
     */
    inline
    aline 
    operator*(const double& a, const aline& b) {return b*a;}

    /** Divide every element of an aline by a constant
     * @param a The  aline.
     * @param b The constant.
     * @return The resultant aline.
     */
    inline
    aline 
    operator/(const aline& a, const double& b){return a*(1.0/b);}

    /** The lecroy_file class.
     * This class holds the data from the lecroy scope and enables you to pull individual alines.
     */
    class lecroy_file
    {
    private:
      lecroy_header m_header;
      boost::shared_array<double> m_time;
      std::vector<boost::shared_array<double> > m_data1;
      std::vector<boost::shared_array<double> > m_data2;

      

    public:
      /** Constructor. */
      lecroy_file();
      
      /** Constructor.
       *  @param lh The lecroy_header.
       * Use this constructor if you have the header but have not yet collected the data.
       * For example when you issue a "WF? DESC\n" command only.
       */
      lecroy_file(const lecroy_header& lh);
      
      /** Decode the entire file (stored in a string. 
       * The string is expected to contain the header and all the data.
       * For example, the string that results from a "WF? ALL\n" remote command
       * @param file The lecroy header.
       */
      void decode_string(const std::string& file);
      
      /** Save the file in lecroy binary format. @param filename The filename to use */
      void save(const std::string filename);

      
      // void load(const std::string filename);
      
      /** Set the data to array one. @param data1 The data array*/
      void set_data1( const double* data1);
      /** Set the data to array two. @param data2 The data array */
      void set_data2( const double* data2);
      
      /** The number of points in the first array. @return The number of points. */
      size_t size1() const {return m_header.wave_array_1();}
      /** The number of points in the second array. @return the number of points. */
      size_t size2() const {return m_header.wave_array_2();}
      
      /** Get data from array 1.
       * @param i the sequence number (if in sequence mode).
       * @return The ICR::lecroy::aline that contains the data.
       */
      aline get_data1(size_t i = 0) const;
      // const double& operator[](const size_t& i) const {return m_data1[i];}
      // double& operator[](const size_t& i){return m_data1[i];}
      // const double& operator()(const size_t& i) const {return m_data1[i];}
      // double& operator()(const size_t& i){return m_data1[i];}
      // aline get_data2() const
      // {
      // 	return m_data2.get();
      // }

    };

  }
}
BOOST_CLASS_VERSION(ICR::lecroy::aline , 1);
    

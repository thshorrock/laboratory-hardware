#pragma once

#include "lecroy_header.hpp"
#include <iostream>
#include <fstream>
#include <vector>
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

    /** Convert characters in a Gnuplot binary stream into floats, ready for processing */
    class GnuplotFloatConverter
    {
    public:
      // When the FloatConverterFromBinary object is assigned to a float value
      // i.e. When put into the vector<float> this method will be called
      //      to convert the object into a float.
      operator float() const { return m_data; }

      /** Grab chars from the stream and store as a float */
      friend 
      std::istream& 
      operator>>(std::istream& st, GnuplotFloatConverter& fc);

    private:
      float  m_data;
    };
    
    inline
    std::istream& 
    operator>>(std::istream& st, GnuplotFloatConverter& fc)
    {
      // Use reinterpret_cast to emphasis how dangerous and unportable this is.
      st.read(reinterpret_cast<char*>(&fc.m_data), sizeof(float));
      return st;
    }

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

      /** Constructor.
       * @param time An vector of times
       * @param trace An vector of data
       **/
      aline(const std::vector<double>& time,
	    const std::vector<double>& trace);

      /** Copy constructor. @param other the aline to copy */
      aline(const aline& other);

      const double* 
      begin_trace() const
      {
	return m_trace.get();
      }

      const double* 
      end_trace() const
      {
	return m_trace.get()+m_size;
      }

      double* 
      begin_trace() 
      {
	return m_trace.get();
      }

      
      double* 
      end_trace() 
      {
	return m_trace.get()+m_size;
      }
      
      const double* 
      begin_time() const
      {
	return m_t.get();
      }

      const double* 
      end_time() const
      {
	return m_t.get()+m_size;
      }

      double* 
      begin_time() 
      {
	return m_t.get();
      }

      
      double* 
      end_time() 
      {
	return m_t.get()+m_size;
      }

      const double* 
      _data() const
      {
	return m_t.get();
      }
      
      const double* 
      _time() const
      {
	return m_t.get();
      }

      double* 
      _data()
      {
	return m_trace.get();
      }
      
      double* 
      _time()
      {
	return m_t.get();
      }

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
       * @param other The constant to scale the voltages by
       * @return The resultant aline.
       */
      aline&
      operator*=(const double& other);
      
      /** Multiply the voltage by another aline.
       * @param other The aline to multiply to the preset.
       * @return The resultant aline.
       */
      aline&
      operator*=(const aline& other);
      
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
    
    /** Save a vector of alines in ascii format. @param a The aline to save @param filename The filename to use */
    void save_ascii(const std::vector<aline>& a, const std::string filename);

    /** Save the aline in gnuplot format. @param a The aline to save @param filename The filename to use */
    void save_gnuplot(const aline& a, const std::string filename);
    
    /** Save a vector of alinesin gnuplot format. @param a The vector of alines to save @param filename The filename to use */
    void save_gnuplot(const std::vector<aline>& a, const std::string filename);
    
    /** Load the aline from gnuplot format. @param a The aline to save @param filename The filename to use */
    void load_gnuplot( aline& a, const std::string filename);
    
    /** Load the aline from gnuplot format. @param a The aline to save @param data The data stored in a standard vector */
    void load_gnuplot( aline& a, const std::vector<float>& data );


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
    }
  
    template<class Archive>
    void 
    aline::load(Archive & ar, const unsigned int version)
    {
      
      ar & m_size;

      m_t     = boost::shared_array<double>(new double[m_size]);
      m_trace = boost::shared_array<double>(new double[m_size]);

      boost::serialization::array<double> sa_time = 
      	boost::serialization::make_array<double>(m_t.get(), m_size);
      boost::serialization::array<double> sa_trace = 
      	boost::serialization::make_array<double>(m_trace.get(), m_size);
      //std::cout<<"get times"<<std::endl;

      ar & sa_time;
      ar & sa_trace;
      
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

    /** Multiply the voltages of two alines together (element by element).
     * @param a The 1st aline.
     * @param b The 2nd aline.
     * @return The resultant aline.
     */
    inline
    aline 
    operator*(const aline& a, const aline& b) 
    {
      aline tmp(a); tmp *= b; return tmp;
    }

    aline 
    pow(const aline&, const double&d );
    
    inline
    aline 
    square(const aline& a) 
    {
      return a*a;
    }
    inline
    aline 
    sqrt(const aline& a) 
    {
      return pow(a,0.5);
    }

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
    

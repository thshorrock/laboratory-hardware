#pragma once

#include "lecroy_header.hpp"
#include <iostream>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/array.hpp>
namespace ICR{

  namespace lecroy{

    namespace exception{
      /** Alines are not the same size. */
      struct alines_not_of_same_size {};
      /** Aline does not exist.
       * This usually happens when the aline requested is greater than the number of sequences (sequence mode).
       */
      struct aline_does_not_exist {};
    }

    /** A class wrapping a single aline */
    class aline
    {
      
    private: 
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

      // double* get_times() const {return m_t.get();};
      // double* get_trace() const {return m_trace.get();};

      
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


      void save_ascii(const std::string filename);

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

      
    };
    
    void save(const aline& a, const std::string filename);
    void load(aline& a, const std::string filename);

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
      ar & m_size;

      m_t     = boost::shared_array<double>(new double[m_size]);
      m_trace = boost::shared_array<double>(new double[m_size]);

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

  aline 
    operator+(const aline& a, const aline& b);

    aline 
    operator-(const aline& a, const aline& b);

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
       */
      lecroy_file(const lecroy_header& lh);

      void decode_string(const std::string& file);

      void save(const std::string filename);
      // void load(const std::string filename);
      
      void set_data1( const double* data1);
      void set_data2( const double* data2);
      
      size_t size1() const {return m_header.wave_array_1();}
      size_t size2() const {return m_header.wave_array_2();}
      

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

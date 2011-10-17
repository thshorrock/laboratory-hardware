
#include "lecroy/lecroy_file.hpp"
#include <string.h>
#include <iterator>

ICR::lecroy::aline::aline()
  : m_t(0), m_trace(0), m_size(0) 
{}

ICR::lecroy::aline::aline(const boost::shared_array<double>& time,
			  const boost::shared_array<double>& trace,
			  const size_t& size)
  : m_t(time), m_trace(trace), m_size(size) 
{}

ICR::lecroy::aline::aline(const std::vector<double>& time,
			  const std::vector<double>& trace)
{
  m_size = time.size();
  m_t     = boost::shared_array<double>(new double[m_size]);
  m_trace = boost::shared_array<double>(new double[m_size]);
  
  std::copy(time.begin(), time.end(), m_t.get());
  std::copy(trace.begin(), trace.end(), m_trace.get());
}

ICR::lecroy::aline::aline(const aline& other)
  :  m_size(other.m_size) 
{
  m_t     = boost::shared_array<double>(new double[m_size]);
  m_trace = boost::shared_array<double>(new double[m_size]);
  
  std::copy(other.m_t.get(), other.m_t.get()+m_size, m_t.get());
  std::copy(other.m_trace.get(), other.m_trace.get()+m_size, m_trace.get());
}

ICR::lecroy::aline&
ICR::lecroy::aline::operator=(const aline& other)
{
  if (&other!=this)
    {
      m_size = other.m_size;
      m_t.reset(new double[m_size]);
      m_trace.reset(new double[m_size]);

      std::copy(other.m_t.get(), other.m_t.get()+m_size, m_t.get());
      std::copy(other.m_trace.get(), other.m_trace.get()+m_size, m_trace.get());
    } 
  return *this;
}

void 
ICR::lecroy::save_ascii(const aline& a, const std::string filename)
{
  boost::scoped_ptr<std::fstream> 
    pOut(new std::fstream(filename.c_str(), std::ios_base::out ));
	
  //char array1
  const size_t size1 =a.size();
  for(size_t i=0;i<size1;++i){
    *pOut  <<  a.time(i) <<"  "<< a[i] <<"\n";
  }
  pOut->close();
}


void 
ICR::lecroy::save_ascii(const std::vector<aline>& a, const std::string filename)
{
  boost::scoped_ptr<std::fstream> 
    pOut(new std::fstream(filename.c_str(), std::ios_base::out ));
	
  //char array1
  const size_t size0 = a.size();
  const size_t size1 =a[0].size();
  for(size_t j=0;j<size0;++j){
    for(size_t i=0;i<size1;++i){
      *pOut  << j<<"  "<< a[j].time(i) <<"  "<< a[j][i] <<"\n";
    }
    *pOut<<"\n";
    *pOut<<"\n";
  }

  pOut->close();
}

void 
ICR::lecroy::save_gnuplot(const aline& a, const std::string filename)
{
  boost::scoped_ptr<std::fstream> 
    pOut(new std::fstream(filename.c_str(), std::ios_base::out|std::ios_base::binary ));

  
  size_t count=0;
  const size_t sf= sizeof(float);
  const size_t vec_size = a.size();
  const size_t points_per_row = vec_size+1;
  const size_t no_rows = 1; //time
  const size_t no_bytes   = sf*(no_rows*points_per_row);

      
  float* buffer = new float[no_rows*points_per_row];//no_bytes
    
  buffer[0] = static_cast<float> (vec_size);
	
  for (size_t i = 0; i<vec_size; ++i)
    buffer[++count] = static_cast<float>(a.time(i));
	
  pOut->write(reinterpret_cast<char*>(&buffer[0]),no_bytes);
	
  //start buffer again
  count = 0;
  buffer[count] = static_cast<float> (0); //the y axis
  //add the offsets and the radius
  for (size_t i = 0; i<vec_size; ++i)
    buffer[++count] = static_cast<float> (a[i]);
  pOut->write(reinterpret_cast<char*>(&buffer[0]),no_bytes);
  
  delete[] buffer;

  pOut->close();
}

void 
ICR::lecroy::load_gnuplot( aline& a, const std::vector<float>& data)
{
  const size_t vec_size = (size_t)(data[0]);
  std::vector<double> t(vec_size);
  std::vector<double> trace(vec_size);

  std::copy(data.begin()+1, data.begin()+vec_size+1, t.begin());
  std::copy(data.begin()+vec_size+1, data.end(), trace.begin());
  
  a = aline(t,trace);
}

void 
ICR::lecroy::save_gnuplot(const std::vector<aline>& a, const std::string filename)
{
  boost::scoped_ptr<std::fstream> 
    pOut(new std::fstream(filename.c_str(), std::ios_base::out|std::ios_base::binary ));

  const size_t vec_size = a[0].size();
  const size_t points_per_row = vec_size+1;
  const size_t no_bytes   = sizeof(float)*(points_per_row);

      
  float* buffer = new float[points_per_row];//no_bytes
    
  buffer[0] = static_cast<float> (vec_size);
  std::copy(a[0]._time(),a[0]._time()+ vec_size, buffer+1);
  pOut->write(reinterpret_cast<char*>(&buffer[0]),no_bytes);
	
  //start buffer again
  //add the offsets and the radius
  for(size_t i=0;i<a.size();++i){
    buffer[0] = static_cast<float> (i); //the y axis
    std::copy(a[i]._data(),a[i]._data()+ vec_size, buffer+1);
    pOut->write(reinterpret_cast<char*>(&buffer[0]),no_bytes);
  }
  delete[] buffer;
  
  pOut->close();
}

void 
ICR::lecroy::load_gnuplot( aline& a, const std::string filename)
{
  std::ifstream input( filename.c_str(), std::ios::binary );
  std::vector<float> buffer;

  std::copy( 
    	    std::istream_iterator<GnuplotFloatConverter>(input), 
   	    std::istream_iterator<GnuplotFloatConverter>( ),
   	    std::back_inserter(buffer)); // copies all data into buffer
  
  load_gnuplot(a, buffer);


  // boost::scoped_ptr<std::fstream> 
  //   pIn(new std::fstream(filename.c_str(), std::ios_base::in |std::ios_base::binary ));

  // float size_buffer;
  // pIn->read(reinterpret_cast<char*>(&size_buffer),sizeof(float));
  
  // size_t vec_size = (size_t)(size_buffer);
  
  // const size_t sf= sizeof(float);
  // const size_t points_per_row = vec_size+1;
  // const size_t no_rows = 1; //time
  // const size_t no_bytes   = sf*(no_rows*points_per_row);
      
  // float* buffer = new float[no_rows*points_per_row];//no_bytes
  
  // boost::shared_array<double> t(new double[vec_size]);
  // boost::shared_array<double> trace(new double[vec_size]);
	
  // pIn->seekg (0, std::ios::beg); 
  
  // pIn->read(reinterpret_cast<char*>(&buffer[0]),no_bytes);
  
  // for(size_t i=1;i<points_per_row;++i)
  //   t[i-1] = buffer[i]; 
  
  // pIn->read(reinterpret_cast<char*>(&buffer[0]),no_bytes);
  
  // for(size_t i=1;i<points_per_row;++i)
  //   trace[i-1] = buffer[i]; 
  
       
  // delete[] buffer;
	    
  // pIn->close();
  // a = aline(t,trace,vec_size);
}


void 
ICR::lecroy::save(const aline& a, const std::string filename)
{
  std::ofstream ofs(filename.c_str(), std::ios::binary);
  boost::archive::binary_oarchive oa(ofs);
  oa<<a;
}

void 
ICR::lecroy::load( aline& a,const std::string filename)
{
  std::ifstream ifs(filename.c_str(), std::ios::binary );
  boost::archive::binary_iarchive ia(ifs);
  //  std::cout<<"filename = "<<filename<<std::endl;

  ia>>a;
}

ICR::lecroy::aline&
ICR::lecroy::aline::operator+=(const aline& other)
{
  if (other.size()!= m_size) throw exception::alines_not_of_same_size();
  for(size_t i=0;i<m_size;++i){
    m_trace[i]+=other[i];
  }
  return *this;
}
   
ICR::lecroy::aline&
ICR::lecroy::aline::operator*=(const double& other)
{
  for(size_t i=0;i<m_size;++i){
    m_trace[i]*=other;
  }
  return *this;
}   

ICR::lecroy::aline&
ICR::lecroy::aline::operator*=(const aline& other)
{
  for(size_t i=0;i<m_size;++i){
    m_trace[i]*=other.m_trace[i];
  }
  return *this;
}   
      
ICR::lecroy::aline
ICR::lecroy::pow(const aline& other, const double& exp)
{
  aline tmp(other);
  double* data = tmp._data();
  for(size_t i=0;i<other.size();++i){
    data[i]=std::pow(data[i], exp);
  }
  return tmp;
}   
      
      
ICR::lecroy::aline&
ICR::lecroy::aline::operator-=(const aline& other)
{
  if (other.size()!= m_size) throw exception::alines_not_of_same_size();
  for(size_t i=0;i<m_size;++i){
    m_trace[i]-=other[i];
  }
  return *this;
}

      
ICR::lecroy::aline
ICR::lecroy::operator+(const aline& a, const aline& b)
    {
      aline tmp(a);
      return tmp+=b;
    }

ICR::lecroy::aline
ICR::lecroy::operator-(const aline& a, const aline& b)
{
  
  aline tmp(a);
  return tmp-=b;
}


ICR::lecroy::lecroy_file::lecroy_file()
  : m_header() ,
    m_data1(),
    m_data2()
	  
{
}

ICR::lecroy::lecroy_file::lecroy_file(const lecroy_header& lh)
  : m_header(lh) ,
    m_data1(),
    m_data2()
	  
{
}

void
ICR::lecroy::lecroy_file::decode_string(const std::string& data)
{
  m_header.decode_string(data);
  const unsigned long array_size1 = m_header.wave_array_1();
  const unsigned long array_size2 = m_header.wave_array_2();
 
  const unsigned long array1_byte = m_header.byte.array_1;
  const unsigned long array2_byte = m_header.byte.array_2;
  
  const double vg = m_header.vertical_gain();
  const double vo = m_header.vertical_offset();
  if (array_size1>0){ 
    boost::scoped_array<double> data1(new double[array_size1]);
    for(size_t i=0;i<array_size1;++i){
      data1[i] =  ((double) data[array1_byte+i])*vg+vo; //+
    }
    set_data1(data1.get());
  }
  if (array_size2>0){ 
    boost::scoped_array<double> data2(new double[array_size2]);
    for(size_t i=0;i<array_size2;++i){
      data2[i] =  ((double) data[array2_byte+i])*vg+vo; //+
    }
    set_data2(data2.get());
  }

}

void
ICR::lecroy::lecroy_file::save(const std::string filename)
{
	
  const size_t alines = m_header.subarray_count();
  const size_t pnts_per_aline =  m_header.wave_array_1()/alines;


  boost::shared_ptr<std::fstream> pOut(new std::fstream(filename.c_str(), std::ios_base::out ));
  boost::shared_ptr<std::iostream> pStream(pOut);
  pStream<<m_header;
	
  //char array1
  const size_t size1   = m_header.wave_array_1();
  const double offset  = m_header.vertical_offset();
  const double igain   = 1.0/m_header.vertical_gain();
  boost::scoped_array<char> buffer(new char[size1]);
  //std::cout<<"size = "<<size1<<std::endl;

  for(size_t i=0;i<m_data1.size();++i){

    for(size_t j=0;j<size1;++j){
      // std::cout<<"j = "<<j+i*pnts_per_aline<<std::endl;

      buffer[j+i*pnts_per_aline] = (char) ( (m_data1[i][j]-offset) * igain );
    }
  }

	
  //char array2
  const size_t  size2 = m_header.wave_array_2();
  boost::scoped_array<char> buffer2(new char[size2]);
  for(size_t i=0;i<m_data1.size();++i){
    for(size_t j=0;j<size1;++j){
      buffer[j+i*pnts_per_aline] = (char) ( (m_data1[i][j]-offset) * igain );
    }
  }
  //write
  pOut->seekp(m_header.array_1_byte(),std::ios::beg);
  pOut->write( &buffer[0], size1);
	
  pOut->seekp(m_header.array_2_byte(),std::ios::beg);
  pOut->write( &buffer2[0], size2);
	
  //std::cout<<"size 1 = "<<size1<<std::endl;


  pOut->close();
}

      // void
      // ICR::lecroy::lecroy_header::load_z_block(std::string filename, char* buffer)
      // {
      // 	input_binary IN(filename);

      // 	IN->seekg (array_1_byte(), std::ios::beg);
      // 	long a1_size = array_2_byte() - array_1_byte(); 
      // 	//td::cout<<"a1_size "<<a1_size<<std::endl;
      // 	IN->read (buffer,a1_size);
 
      // }
// void
// ICR::lecroy::lecroy_file::load(const std::string filename)
// {
	
//   const size_t alines = m_header.subarray_count();
//   const size_t pnts_per_aline =  m_header.wave_array_1()/alines;


//   boost::shared_ptr<std::fstream> pOut(new std::fstream(filename.c_str(), std::ios_base::out ));
//   boost::shared_ptr<std::iostream> pStream(pOut);
//   pStream>>m_header;
	
//   //char array1
//   const size_t size1   = m_header.wave_array_1();
//   const double offset  = m_header.vertical_offset();
//   const double igain   = 1.0/m_header.vertical_gain();
//   boost::scoped_array<char> buffer(new char[size1]);
//   //std::cout<<"size = "<<size1<<std::endl;

//   for(size_t i=0;i<m_data1.size();++i){

//     for(size_t j=0;j<size1;++j){
//       // std::cout<<"j = "<<j+i*pnts_per_aline<<std::endl;

//       buffer[j+i*pnts_per_aline] = (char) ( (m_data1[i][j]-offset) * igain );
//     }
//   }

	
//   //char array2
//   const size_t  size2 = m_header.wave_array_2();
//   boost::scoped_array<char> buffer2(new char[size2]);
//   for(size_t i=0;i<m_data1.size();++i){
//     for(size_t j=0;j<size1;++j){
//       buffer[j+i*pnts_per_aline] = (char) ( (m_data1[i][j]-offset) * igain );
//     }
//   }
//   //write
//   pOut->seekp(m_header.array_1_byte(),std::ios::beg);
//   pOut->write( &buffer[0], size1);
	
//   pOut->seekp(m_header.array_2_byte(),std::ios::beg);
//   pOut->write( &buffer2[0], size2);
	
//   //std::cout<<"size 1 = "<<size1<<std::endl;


//   pOut->close();
// }


void 
ICR::lecroy::lecroy_file::set_data1( const double* data1)
{
  const size_t alines = m_header.subarray_count();
  const size_t pnts_per_aline =  m_header.wave_array_1()/alines;
	
  size_t count = 0;
  //fails if alines = 0 // will result in aline_does_not_exit_crash.
  while (count<alines){
    boost::shared_array<double> data(new double[pnts_per_aline]);
    memcpy(data.get(), &data1[pnts_per_aline*count], pnts_per_aline*sizeof(double));
    m_data1.push_back( data );
    ++count;
  }
	
  // std::cout<<"use_count in set_data1 = "<<m_data1[0].<<std::endl;
	
  boost::shared_array<double> time(new double[pnts_per_aline]);
  const double offset = m_header.horiz_offset();
  const double intv   = m_header.horiz_interval();
  for(size_t i=0;i<pnts_per_aline;++i){
    time[i]=offset + i*intv;
  }
	
  m_time = time;
}

void ICR::lecroy::lecroy_file::set_data2( const double* data2)
{
  // boost::shared_array<double> data(new double[m_header.wave_array_2()]);
  // memcpy(data.get(), data2, m_header.wave_array_2()*sizeof(double));
  // m_data2 = data;

  const size_t alines = m_header.subarray_count();
  const size_t pnts_per_aline =  m_header.wave_array_1()/alines;
	
  size_t count = 0;

  while (count<alines){
    boost::shared_array<double> data(new double[pnts_per_aline]);
    memcpy(data.get(), &data2[pnts_per_aline*count], pnts_per_aline*sizeof(double));
    m_data2.push_back( data );
    ++count;
  }
}

ICR::lecroy::aline 
ICR::lecroy::lecroy_file::get_data1(size_t i ) const
{
  if (i>= m_data1.size() ) throw ICR::exception::aline_does_not_exist();

  //std::cout<<"trigger time ("<<i<<")   = "<<m_header.trigger_time(i)<<std::endl;


  //std::cout<<"trigger offset ("<<i<<") = "<<m_header.trigger_offset(i)<<std::endl;

  aline a(m_time, m_data1[i], m_header.wave_array_1()/m_header.subarray_count() );
  return a;
}

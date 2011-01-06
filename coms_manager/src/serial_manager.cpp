#include "coms_manager/serial_manager.hpp"

#include <boost/bind.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/array.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

ICR::coms::serial_manager::serial_manager(const std::string& name,
					  const unsigned int& baud_rate,
					  const enum flow_control::type& flow_control,
					  const enum parity::type&       parity,
					  const enum stop_bits::type&    stop_bits,
					  const unsigned int& char_size
					  ) 
  : coms_manager(),
    m_io_service(),
    m_name(name),
    m_SerialPort(m_io_service,name)
    
{
  
  m_SerialPort.set_option(boost::asio::serial_port::baud_rate(baud_rate));
  m_SerialPort.set_option(boost::asio::serial_port::flow_control(flow_control));
  m_SerialPort.set_option(boost::asio::serial_port::parity(parity));
  m_SerialPort.set_option(boost::asio::serial_port::stop_bits(stop_bits));
  m_SerialPort.set_option(boost::asio::serial_port::character_size(char_size));
  
}

ICR::coms::serial_manager::~serial_manager()
{ 
  close();
}

void
ICR::coms::serial_manager::close()
{ 
  if (m_SerialPort.is_open()) 
    m_SerialPort.close();
}

void
ICR::coms::serial_manager::open()
{ 
  if (!m_SerialPort.is_open()) 
    m_SerialPort.open(m_name);
}
void
ICR::coms::serial_manager::send(const std::string& cmd)
	throw (boost::system::system_error) 
{
  // std::cout<<"send cmd = "<<cmd<<std::endl;

  // for(size_t i=0;i<cmd.size();++i){
  //   std::cout<<"["<<i<<"] = "<<(int) cmd[i]<<std::endl;
  // }
  if (!m_error) 
    boost::asio::write(m_SerialPort, boost::asio::buffer(cmd)); 
  else 
    throw boost::system::system_error(m_error);
  //  boost::this_thread::sleep(boost::posix_time::milliseconds(100)); 
}



std::string
ICR::coms::serial_manager::recv(const std::string& cmd, const unsigned long& buffsize, const bool& size_exactly) 
  throw(boost::system::system_error )
{
  //send quiery and await repsonse
    serial_manager::send(cmd);
    return recv(buffsize,size_exactly);
}

std::string
ICR::coms::serial_manager::timed_recv(const std::string& cmd, const unsigned long& buffsize, const double& seconds, const bool& size_exactly)  
  throw(exception::timeout_exceeded,
	boost::system::system_error )
{
  //send quiery and await repsonse
    serial_manager::send(cmd);
    return timed_recv(buffsize,seconds,size_exactly);
}





std::string
ICR::coms::serial_manager::recv( const unsigned long& buffsize, const bool& size_exactly) 
  throw(boost::system::system_error )
{
  char * buf = (char*) malloc(buffsize);
  std::string ret;
  size_t len;
  if (size_exactly){
    len = buffsize;
    boost::asio::read(m_SerialPort, boost::asio::buffer(buf, buffsize)); 
  }
  else
    len =m_SerialPort.read_some(boost::asio::buffer(buf, buffsize), m_error);
  for(size_t i=0;i<len;++i){
    ret.push_back(buf[i]);
  }

  if (m_error == boost::asio::error::eof)
    {
      //this is okay
    }
  else  if (m_error)
    throw boost::system::system_error(m_error); // Some other error.

  free(buf);
  return ret;
}

std::string
ICR::coms::serial_manager::timed_recv(const unsigned long& buffsize, const double& seconds, const bool& size_exactly )  
  throw(exception::timeout_exceeded,
	boost::system::system_error )
{
  char * buf = (char*) malloc(buffsize);
  boost::asio::deadline_timer timer(m_SerialPort.io_service());
  boost::optional<boost::system::error_code> timer_result; 
  timer.expires_from_now(boost::posix_time::seconds(seconds));
  timer.async_wait(boost::bind(&check_timeout, &timer_result, _1)); 
  
  boost::optional<boost::system::error_code> read_result; 

  unsigned int actually_read = 0;
  if (size_exactly){
    boost::asio::async_read(m_SerialPort, boost::asio::buffer(buf, buffsize),  
  boost::bind(&check_timeout_and_size, &read_result, _1,&actually_read,_2)); 
  }
  else
    m_SerialPort.async_read_some(boost::asio::buffer(buf, buffsize),   
 boost::bind(&check_timeout_and_size, &read_result, _1,&actually_read,_2) );
  
  m_SerialPort.io_service().reset(); 
  while (m_SerialPort.io_service().run_one()) 
    { 
      if (read_result) {
        timer.cancel(); 

      }
      else if (timer_result) 
	{
	  m_SerialPort.cancel(); 
	}
    }
  if (*read_result) 
    {
	  free(buf);
	  throw exception::timeout_exceeded();
    }
  std::string ret;
  for(size_t i=0;i<actually_read;++i){
    ret.push_back(buf[i]);
  }

  //ret.append(buf);
  free(buf);
  return ret;
}

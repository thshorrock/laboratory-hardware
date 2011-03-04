#include "coms_manager/serial_manager.hpp"

#include <boost/bind.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/array.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
//#include <boost/system/error_code.hpp>
ICR::coms::serial_manager::serial_manager(const std::string& name,
					  const unsigned int& baud_rate,
					  const enum flow_control::type& flow_control,
					  const enum parity::type&       parity,
					  const enum stop_bits::type&    stop_bits,
					  const unsigned int& char_size
					  ) 
  throw(exception::serial_port_does_not_exist)
  : coms_manager(),
    m_io_service(),
    m_name(name),
    m_baud_rate(baud_rate),
    m_flow_control(flow_control),
    m_parity(parity),
    m_stop_bits(stop_bits),
    m_char_size(char_size),
    m_SerialPort(m_io_service)
    
{
  open();
}

ICR::coms::serial_manager::~serial_manager()
{ 
  close();
}

void
ICR::coms::serial_manager::close()
{ 
  if (m_SerialPort.is_open()) {
    std::cout<<"closing"<<std::endl;

    m_SerialPort.close();
    std::cout<<"closed"<<std::endl;

  }
}

void
ICR::coms::serial_manager::open()
{ 
    std::cout<<"serial open"<<std::endl;

  if (!m_SerialPort.is_open()) 
    {
      std::cout<<" ... opening"<<std::endl;

      m_SerialPort.open(m_name, m_error);
  
      if (m_error == boost::system::errc::no_such_file_or_directory ) //boost::system::error_code::errc::
	{
	  throw exception::serial_port_does_not_exist(m_name);
	}

      m_SerialPort.set_option(boost::asio::serial_port::baud_rate(m_baud_rate));
      m_SerialPort.set_option(boost::asio::serial_port::flow_control(m_flow_control));
      m_SerialPort.set_option(boost::asio::serial_port::parity(m_parity));
      m_SerialPort.set_option(boost::asio::serial_port::stop_bits(m_stop_bits));
      m_SerialPort.set_option(boost::asio::serial_port::character_size(m_char_size));



      std::cout<<" ... open"<<std::endl;


    }
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
  
  if (m_error)
    { 
      std::cout<<"exception in serial send"<<std::endl;

      throw boost::system::system_error(m_error);
    }
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
  return serial_manager::timed_recv(buffsize,seconds,size_exactly);
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

  if (m_error) { //some kind of exception 
    free(buf);  //cleanup
    std::cout<<"recv error in serial :: erroc_code value = "<<m_error.value()<<std::endl;

    //try to recue
    close();
    open();//reopen port
    throw exception::exception_in_receive_you_must_resend_command();
  }
  else //no error
    free(buf);  //cleanup

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
    //   std::cout<<"size exactly!"<<std::endl;

    boost::asio::async_read(m_SerialPort, boost::asio::buffer(buf, buffsize),  
			    boost::bind(&check_timeout_and_size, &read_result, _1,&actually_read,_2)); 

  }
  else
    m_SerialPort.async_read_some(boost::asio::buffer(buf, buffsize),   
				 boost::bind(&check_timeout_and_size, &read_result, _1,&actually_read,_2) );
  
  m_SerialPort.io_service().reset(); 
  
  bool times_up = false;
  while (m_SerialPort.io_service().run_one()) 
    { 
      //std::cout<<"read res = "<<read_result<<"time_res = "<<timer_result<<std::endl;
      //ICR::coms::sleep(200);
      if (read_result)
	{
	  //std::cout<<"about to cancel timer"<<std::endl;

	  timer.cancel(); 
	  //std::cout<<"timer cancelled"<<std::endl;

	}
      else if (timer_result) 
	{
	  //std::cout<<"trying to cancel"<<std::endl;
	  m_SerialPort.cancel();
	  times_up=true;

	  //std::cout<<"timer has exceeded from serial manager"<<std::endl;
	 
	}
    }
  if (times_up) {
    free(buf); 
    ICR::coms::sleep(500);
    throw exception::timeout_exceeded();
  }
  //std::cout<<"read res"<<std::endl;

  if (*read_result) 
    {
      //some sort of exception.
      std::cout<<"timed_recv in serial erroc_code value = "<<m_error.value()<<std::endl;

      //try to recue
      close();
      open();//reopen socket
      std::cout<<"reopened"<<std::endl;
      std::cout<<"problem in timed_recv serial  - cleaning up ... "<<std::endl;
      //resend
      // try{
      // 	timed_recv(buffsize,seconds, false); //retry
      // } catch (...)  {}
      
      std::cout<<"  ... cleanup complete\n \n ****** you must now resend the command   ******* "<<std::endl;
      
      free(buf);
      throw exception::exception_in_receive_you_must_resend_command();
     
    }

  std::string ret;
  for(size_t i=0;i<actually_read;++i){
    ret.push_back(buf[i]);
  }

  // std::cout<<"read "<<actually_read <<" characters: ";
  // for(size_t i=0;i<actually_read;++i){

  //   std::cout<<(int) buf[i]<<" ";

  // }
  // std::cout<<std::endl;
  // std::cout<<"sizeof ret = "<<ret.size()<<std::endl;

  free(buf);  //cleanup
  //ret.append(buf);
  return ret;
}

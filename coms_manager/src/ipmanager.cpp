#include "coms_manager/ipmanager.hpp"
#include <boost/bind.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/array.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

void
ICR::coms::IPmanager::open() throw (boost::system::system_error)  
{
  
  m_error = boost::asio::error::host_not_found;
  int count = 0;
  bool connection_failed = true;
  while (connection_failed && count<5){
    try{
  
      boost::asio::ip::tcp::resolver resolver(m_io_service);
      boost::asio::ip::tcp::resolver::query query(m_address, m_port);
      boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
      boost::asio::ip::tcp::resolver::iterator end; // End marker.
      while (m_error && iter != end)
	{
	  m_socket.close();
	  m_socket.connect(*iter++, m_error);
	}
      if (m_error){
	throw boost::system::system_error(m_error);
      }
      connection_failed = false;
    } catch(boost::system::system_error& e){
      if (e.code().value() == boost::asio::error::broken_pipe) {
	std::cout<<"broken pipe, trying to fix..."<<std::endl;
	//go to sleep
	sleep(1000);
	++count;
      }
      else
	throw;
    }
  }
}
			


ICR::coms::IPmanager::IPmanager(const std::string& address, const std::string& port)
 throw (boost::system::system_error)
  : coms_manager(),
    m_io_service(),
    m_socket(m_io_service),
    m_error(boost::asio::error::host_not_found),
    m_address(address), m_port(port)
{
  open();
}

    
ICR::coms::IPmanager::~IPmanager(){m_socket.close();}


void
ICR::coms::IPmanager::no_delay(const bool& do_not_delay)
{
  boost::asio::ip::tcp::no_delay no_delay_option(do_not_delay);
  m_socket.set_option(no_delay_option); 
}

void
ICR::coms::IPmanager::keep_alive(const bool& do_keep_alive)
{
  boost::asio::socket_base::keep_alive keep_alive_option(do_keep_alive);
  m_socket.set_option(keep_alive_option); 
}

void
ICR::coms::IPmanager::send(const std::string& cmd) 
	throw (boost::system::system_error) 
{
  // std::cout<<"base cmd = "<<cmd<<std::endl;

  if (!m_error) 
    boost::asio::write(m_socket, 
		       boost::asio::buffer(cmd), 
		       boost::asio::transfer_all(),
		       m_error
		       ); 

  if ( m_error == boost::asio::error::connection_aborted) {
    std::cout<<"send connection aborted, retrying"<<std::endl;
    
    cancel();
    close();
    open();//reopen socket
    //resend
    send(cmd);

  }
  else if ( m_error == boost::asio::error::broken_pipe) {
    std::cout<<"broken pipe in send, trying to fix..."<<std::endl;
    cancel();
    close();
    open();//reopen socket
    //resend
    send(cmd);
  }
  else if(m_error) {
    
    std::cout<<"send error"<<std::endl;
    std::cout<<"erroc_code value = "<<m_error.value()<<std::endl;

    throw boost::system::system_error(m_error);

  }
  // boost::this_thread::sleep(boost::posix_time::milliseconds(100)); 
}


std::string
ICR::coms::IPmanager::recv( const unsigned long& buffsize, const bool& size_exactly ) 
  throw(boost::system::system_error )
{
  std::string ret;
  char * buf = (char*) malloc(buffsize);
 
  size_t len;
  if (size_exactly){
    len = buffsize;
    boost::asio::read(m_socket,
		      boost::asio::buffer(buf, buffsize)
		      ,
		      boost::asio::transfer_all(),
		      m_error
		      ); 
  }
  else
    len =m_socket.read_some(boost::asio::buffer(buf, buffsize), m_error);

  
  for(size_t i=0;i<len;++i){
    ret.push_back(buf[i]);
  }

  //When the server closes the connection, the ip::tcp::socket::read_some() function will exit with the boost::asio::error::eof m_error, which is how we know to exit the loop.
  

  // if (m_error == boost::asio::error::eof)
  //   {
  //     //this is okay
  //   }
  //else  		       ); 
  if ( m_error == boost::asio::error::connection_aborted) {
    std::cout<<"recv connection aborted, retrying"<<std::endl;
    
    cancel();
    close();
    open();//reopen socket
      std::cout<<"reopened"<<std::endl;
    //resend
      // recv(buffsize,size_exactly);

  }
  if (m_error == boost::asio::error::connection_reset)
    {
      std::cout<<"connection reset error in recv"<<std::endl;
      free(buf);
      cancel();
      close();
      //may need to reset the io_serice here
      open();
      std::cout<<"reopened"<<std::endl;
      
      //recv(buffsize,size_exactly);
    }
  else if (m_error == boost::asio::error::broken_pipe) {
    std::cout<<"broken pipe in recv, trying to fix..."<<std::endl;
    free(buf);
    cancel();
    close();
    open();//reopen socket
      std::cout<<"reopened"<<std::endl;
    //resend
    // recv(buffsize,size_exactly);
  }
  else if (m_error){
    free(buf);
    std::cout<<"recv error"<<std::endl;

    throw boost::system::system_error(m_error);
  }
  //std::cout<<"buff size = "<< boost::asio::buffer_size(buf)<<std::endl;
  // std::string ret(buf);

  free(buf);
  return ret;
}

std::string
ICR::coms::IPmanager::timed_recv(const unsigned long& buffsize, const double& seconds, const bool& size_exactly  ) 
  throw(exception::timeout_exceeded,
	boost::system::system_error )
{

  char * buf = (char*) malloc(buffsize);
  boost::asio::deadline_timer timer(m_socket.io_service());
  boost::optional<boost::system::error_code> timer_result; 
  timer.expires_from_now(boost::posix_time::seconds(seconds));
  timer.async_wait(boost::bind(&check_timeout, &timer_result, _1)); 
  
  boost::optional<boost::system::error_code> read_result; 
  
  unsigned int actually_read = 0;
  
  if (size_exactly){
    // std::cout<<"exactly"<<std::endl;

    boost::asio::async_read(m_socket, boost::asio::buffer(buf, buffsize),  
			    boost::asio::transfer_all(),  
			    boost::bind(&check_timeout_and_size, &read_result, _1,&actually_read,_2)); 

  }
  else
    m_socket.async_read_some(boost::asio::buffer(buf, buffsize),   
			     boost::bind(&check_timeout_and_size, &read_result, _1,&actually_read,_2) );
 
  
  m_socket.io_service().reset(); 
  while (m_socket.io_service().run_one()) 
    { 
      if (read_result) {
        timer.cancel(); 

      }
      else if (timer_result) 
	{
	  m_socket.cancel(); 
	}
    }
  if (*read_result) 
    {
     
	  free(buf);
	  std::cout<<"read result = "<<read_result<<std::endl;
	  if (!m_socket.is_open()) {
	    std::cout<<"socket closed trying to reopen"<<std::endl;
	    open();
	  }
	  // else{
	  //   std::cout<<"socket open, will try to reopen"<<std::endl;
	  //   cancel();
	  //   close();
	  //   open();
	  // }
	  ///if (*read_result == boost::asio::error::operation_aborted ) 
	    throw exception::timeout_exceeded();
	  // else if (
	  // 	   (*read_result == boost::asio::error::connection_reset)
	  // 	   || 
	  // 	   (*read_result == boost::asio::error::connection_refused)
	  // 	   )  {
	  //   std::cout<<"connection reset error in  timed_recv"<<std::endl;
	  //   cancel();
	  //   close();
	  //   //may need to reset the io_serice here
	  //   open();
	  //   timed_recv(buffsize,seconds, size_exactly);
	  // }
	  // else
	  //   { 
	  //     std::cout<<"timed recv error"<<std::endl;

	  //     throw boost::system::system_error(*read_result);
	      
	  //   }
	  
    }
  std::string ret;
  for(size_t i=0;i<actually_read;++i){
    ret.push_back(buf[i]);
  }

  //ret.append(buf);
  free(buf);
  return ret;
}


std::string
ICR::coms::IPmanager::recv(const std::string& cmd, const unsigned long& buffsize, const bool& size_exactly )  
throw(boost::system::system_error )
{
  //send quiery and await repsonse
  IPmanager::send(cmd);
  return recv(buffsize,size_exactly);
  
}

std::string
ICR::coms::IPmanager::timed_recv(const std::string& cmd, const unsigned long& buffsize, const double& seconds, const bool& size_exactly   ) 
 throw(exception::timeout_exceeded,
	boost::system::system_error )
{
  //send quiery and await repsonse
  IPmanager::send(cmd);
  return timed_recv(buffsize,seconds,size_exactly);
}

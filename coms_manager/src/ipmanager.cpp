#include "coms_manager/ipmanager.hpp"
#include <boost/bind.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/array.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

void
ICR::coms::IPmanager::open() 
{
  
  m_error = boost::asio::error::host_not_found;
  bool connection_failed = true;
  size_t count = 0;
  while (connection_failed){
      boost::asio::ip::tcp::resolver resolver(m_io_service);
      boost::asio::ip::tcp::resolver::query query(m_address, m_port);
      boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
      boost::asio::ip::tcp::resolver::iterator end; // End marker.
      while (m_error && iter != end)
	{
	  m_socket.close();
	  m_socket.connect(*iter++, m_error);
	}
      if (m_error){ //still
	if ( m_error == boost::asio::error::connection_refused) {
	  std::cout<<"open connection refused, please sort out the problem (try to reconnect to the network) and press any key followed by enter..."<<std::endl;
	    char c;
	    std::cin>> c;
	}
	else if (m_error == boost::asio::error::host_unreachable) 
	  {
	    std::cout<<"host unreachable, please check your wires, ping the devices,  reconnect to the network, make sure everything is okay  and press any key followed by enter..."<<std::endl;
	    char c;
	    std::cin>> c;
	  }
	//try to recue
	//sleep(200);//go to sleep
	++count;
	
      }
      else
	connection_failed = false;
 
      if (count>10) //could not rescue
	{
	  if ( m_error == boost::asio::error::connection_refused) {
	    std::cout<<"open connection refused, please sort out the problem (try to reconnect to the network) and press any key followed by enter..."<<std::endl;
	    char c;
	    std::cin>> c;
	  }
	  else if (m_error == boost::asio::error::host_unreachable) 
	    {
	      std::cout<<"host unreachable, please check your wires, ping the devices,  reconnect to the network, make sure everything is okay  and press any key followed by enter..."<<std::endl;
	      char c;
	      std::cin>> c;
	    }
	  else if ( m_error == boost::asio::error::connection_aborted) {
	    std::cout<<"open connection aborted, retrying"<<std::endl;
	  }
	  else if (m_error == boost::asio::error::connection_reset)
	    {
	      std::cout<<"connection reset  by peer error in open"<<std::endl;
	    }
	  else if (m_error == boost::asio::error::broken_pipe) {
	    std::cout<<"broken pipe in open, trying to fix..."<<std::endl;
	  }
	  else if (m_error == boost::asio::error::timed_out) 
	    {
	      std::cout<<"connection timed out , trying again"<<std::endl;
	    }
	  else{
	    std::cout<<"open erroc_code value = "<<m_error.value()<<std::endl;

	  }

	  std::cout<<"Something is wrong - I cannot open the IP address.  Please inspect the error messages and try to fix. I am going to sleep for 1 minute"<<std::endl;
	  boost::this_thread::sleep(boost::posix_time::seconds(60)); 
	} 
  }
}
    

			


ICR::coms::IPmanager::IPmanager(const std::string& address, const std::string& port)
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
{
  bool sent = false;
  size_t count = 0;
  while (!sent) 
    {
      if (!m_error) //should get reset with the open 
	boost::asio::write(m_socket, 
			   boost::asio::buffer(cmd), 
			   boost::asio::transfer_all(),
			   m_error
			   ); 

      if (m_error)
	{ 
	  //try to rescue
	  cancel();
	  close();
	  open();//reopen socket - should reset m_error if successful
	}
      else
	sent = true;
      // boost::this_thread::sleep(boost::posix_time::milliseconds(100)); 

      if (count>10) //could not rescue
	{
	  std::cout<<"send erroc_code value = "<<m_error.value()<<std::endl;
	  if ( m_error == boost::asio::error::connection_aborted) {
	    std::cout<<"send connection aborted, retrying"<<std::endl;
	  }
	  else if ( m_error == boost::asio::error::broken_pipe) {
	    std::cout<<"broken pipe in send, trying to fix..."<<std::endl;
	  }
	  std::string cmd_beg;
	  if (cmd.size()<100) 
	    for(size_t i=0;i<cmd.size();++i){
	      cmd_beg.push_back(cmd[i]);
	    }
	  else
	    for(size_t i=0;i<100;++i){
	      cmd_beg.push_back(cmd[i]);
	    }
	  

	  std::cout<<"Something is wrong - I cannot send the command '"
		   <<cmd_beg
		   <<"' over the IP address.  Please inspect the error messages and try to fix. I am going to sleep for 1 minute"<<std::endl;
	  
	  boost::this_thread::sleep(boost::posix_time::seconds(60)); 
	} 
      
    }
}


std::string
ICR::coms::IPmanager::recv( const unsigned long& buffsize, const bool& size_exactly ) 
  throw(exception::exception_in_receive_you_must_resend_command )
{
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

  std::string ret(buf,len ); //Note need this construction to make it a char array and NOT a c-string
  
  // for(size_t i=0;i<len;++i){
  //   ret.push_back(buf[i]);
  // }

  //When the server closes the connection, the ip::tcp::socket::read_some() function will exit with the boost::asio::error::eof m_error, which is how we know to exit the loop.
  

  if (m_error) { //some kind of exception 
    free(buf);  //cleanup
    if ( m_error == boost::asio::error::connection_aborted) {
      std::cout<<"recv connection aborted, retrying"<<std::endl;
    }
    else if (m_error == boost::asio::error::connection_reset)
      {
	std::cout<<"connection reset error in recv"<<std::endl;
      }
    else if (m_error == boost::asio::error::broken_pipe) {
      std::cout<<"broken pipe in recv, trying to fix..."<<std::endl;
    }
    else
      std::cout<<"recv error :: erroc_code value = "<<m_error.value()<<std::endl;

    //try to recue
    cancel();
    close();
    open();//reopen socket
    // //receive_some to cleanup spillage (excess characters this have not been sent).
    // std::cout<<"problem in recv - about to try to cleanup "<<std::endl;
    // try{
    //   timed_recv(buffsize,1, false); //retry
    // } catch (...)  {}
    // std::cout<<"  ... cleanup complete\n \n ****** you must now resend the command   ******* "<<std::endl;

    throw exception::exception_in_receive_you_must_resend_command();
  }
  else //no error
    free(buf);  //cleanup

  return ret;
}

std::string
ICR::coms::IPmanager::timed_recv(const unsigned long& buffsize, const double& seconds, const bool& size_exactly  ) 
  throw(exception::timeout_exceeded,
	exception::exception_in_receive_you_must_resend_command
	)
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

  bool times_up = false;

  while (m_socket.io_service().run_one()) 
    { 
      if (read_result) {
        timer.cancel(); 
      }
      else if (timer_result) 

	{
	  m_socket.cancel(); 
	  //std::cout<<"timer has exceeded from ip manager"<<std::endl;
	  times_up = true;
	}
    }
  
  if (times_up)
    {
      free(buf); 
      throw exception::timeout_exceeded();
    } 
  
  if (*read_result) 
    {
      free(buf); 
      //some sort of exception.
      if ( *read_result == boost::asio::error::connection_aborted) {
	std::cout<<" ip timed_recv connection aborted, retrying"<<std::endl;
      }
      else if (*read_result == boost::asio::error::connection_reset)
	{
	  std::cout<<"connection reset error in ip  timed_recv"<<std::endl;
	}
      else if (*read_result == boost::asio::error::broken_pipe) {
	std::cout<<"broken pipe in  ip timed_recv, trying to fix..."<<std::endl;
      }
      else if (*read_result  == boost::asio::error::operation_aborted) {
	std::cout<<"operation aborted, retrying"<<std::endl;
      }
      else if (*read_result  == boost::asio::error::bad_descriptor) {
	std::cout<<"bad file descriptor, retrying"<<std::endl;
      }
      else {
	std::cout<<"timed_recv ip erroc_code value = "<<*read_result<<std::endl;
	//throw boost::system::system_error(*read_result);
      }
      //try to recue
      cancel();
      close();
      open();//reopen socket
      std::cout<<"reopened"<<std::endl;
      //std::cout<<"problem in timed_recv - cleaning up ... "<<std::endl;
      //resend
      // try{
      // 	timed_recv(buffsize,seconds, false); //retry
      // } catch (...)  {}
      
      // std::cout<<"  ... cleanup complete\n \n ****** you must now resend the command   ******* "<<std::endl;
      
      throw exception::exception_in_receive_you_must_resend_command();
	  
    }
  std::string ret(buf,actually_read ); //Note need this construction to make it a char array and NOT a c-string
  // std::string ret;
  // for(size_t i=0;i<actually_read;++i){
  //   ret.push_back(buf[i]);
  // }
  free(buf);  //cleanup


  //ret.append(buf);
  return ret;
}


      // free(buf);
      // std::cout<<"read result = "<<read_result<<std::endl;
      // if (!m_socket.is_open()) {
      //   std::cout<<"socket closed trying to reopen"<<std::endl;
      //   open();
      // }


      // else{
      //   std::cout<<"socket open, will try to reopen"<<std::endl;
      //   cancel();
      //   close();
      //   open();
      // }
      ///if (*read_result == boost::asio::error::operation_aborted ) 
      //  throw exception::timeout_exceeded();
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

      //   throw boost::system::system_error(*read_result);
	      
      //   }


std::string
ICR::coms::IPmanager::recv(const std::string& cmd, const unsigned long& buffsize, const bool& size_exactly )  
throw( )
{
  //send quiery and await repsonse
  for(;;){
    try
      {
	IPmanager::send(cmd);
	return recv(buffsize,size_exactly);
      }
    catch(exception::exception_in_receive_you_must_resend_command& e)
      {
	std::cout<<"exception in recv caught - resending the command"<<std::endl;
      }
  }
  
}

std::string
ICR::coms::IPmanager::timed_recv(const std::string& cmd, const unsigned long& buffsize, const double& seconds, const bool& size_exactly   ) 
 throw(exception::timeout_exceeded )
{
  //send quiery and await repsonse
  for(;;){
    try
      {
	IPmanager::send(cmd);
	return timed_recv(buffsize,seconds,size_exactly);
      }
    catch(exception::exception_in_receive_you_must_resend_command& e)
      {
	std::cout<<"exception in timed_recv caught - resending the command"<<std::endl;
      }
  }
}

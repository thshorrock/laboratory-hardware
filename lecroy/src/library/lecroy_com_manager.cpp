#include "stringify.hpp"

#include <bitset>
#include <limits>
#include <vector>
#include "lecroy/lecroy_com_manager.hpp"
#include <boost/thread.hpp>
namespace L = ICR::lecroy;

void
L::lecroy_com_header::set_operation()
{
 
  
  // std::bitset<8> op; op.reset();
  
  // op.set(7, m_data);
  // op.set(6, m_remote);
  // op.set(5, m_lockout);
  // op.set(4, m_clear);
  // op.set(3, m_srq);
  // op.set(2, m_serial_poll);
  // op.set(0, m_eoi);
  // for(size_t i=0;i<8;++i){
  //   std::cout<<"op["<<i<<"]= "<<op.test(i)<<std::endl;

  // }

  // m_header[0] = op.to_string()[0]; 
  unsigned char       op = 0x00;
  if (m_data)         op |= ( 1 << 7 );

  if (m_remote)       op |= ( 1 << 6 );
  if (m_lockout)      op |= ( 1 << 5 );
  if (m_clear)        op |= ( 1 << 4 );
  if (m_srq)          op |= ( 1 << 3 );
  if (m_serial_poll)  op |= ( 1 << 2 );
  if (m_eoi)          op |= ( 1 << 0 );
  m_header[0] = op;
}

L::lecroy_com_header::lecroy_com_header(const bool& eol, const bool& lock)
  :  m_header(),
     m_data(true), m_remote(true), m_lockout(lock),
     m_clear(false), m_srq(false),
     m_serial_poll(false), m_eoi(eol),
     m_version(1),
     m_sequence_number(0)
{
  set_operation();
  set_sequence();
  set_version();
}


unsigned long 
L::lecroy_com_header::get_length(const std::string& header)
{

  std::bitset<32> bits; bits.reset();
  std::bitset<8> byte1(header[4]);
  std::bitset<8> byte2(header[5]);
  std::bitset<8> byte3(header[6]);
  std::bitset<8> byte4(header[7]);

	
  for(size_t i=0;i<8;++i){
    bits.set(i, byte4.test(i));
  }

  for(size_t i=0;i<8;++i){
    bits.set(i+8, byte3.test(i));
  }
  for(size_t i=0;i<8;++i){
    bits.set(i+16, byte2.test(i));
  }

  for(size_t i=0;i<8;++i){
    bits.set(i+24, byte1.test(i));
  }
	
  return bits.to_ulong();
}
bool
L::lecroy_com_header::is_eoi(const std::string& header) const
      {
	//operation bit
	char op = header[0];
	return op & ( 1 << 0 );
      }

void
L::lecroy_com_header::set_length(const std::string& cmd)
{
  int VICP_HEADER_LSB_OFFSET = 7;
  int VICP_HEADER_MSB_OFFSET = 4;
  int i = VICP_HEADER_LSB_OFFSET;
  unsigned long length = cmd.size();

  for ( ; i >= VICP_HEADER_MSB_OFFSET; length >>= 8, i-- )
        m_header[ i ] = length & 0xFF;

}

void
L::lecroy_com_header::set_sequence(){
  
  //  std::cout<<"sequence number   = "<<(int)m_sequence_number<<std::endl;
     if ( ++m_sequence_number == 0 )
       m_sequence_number++;
     
     //   std::cout<<"sequence number = "<<(int)m_sequence_number<<std::endl;

    m_header[2] =  m_sequence_number;
}

void
L::lecroy_com_header::set_version()
{
  /* The 'header_version' field is a number - currently only 1 is an acceptable
     value (protocol version 1A is treated as a subversion of version 1). */
  m_version = 1;
  m_header[1] = m_version;
}

std::string 
L::lecroy_com_header::add(const std::string& cmd)
{
  set_operation();
  set_version();
  set_sequence();
  // std::cout<<"sequence number 3 = "<<(int)m_sequence_number<<std::endl;
  set_length(cmd);
  
  
  std::string header(" ", 8);
  std::copy(m_header.begin(), m_header.end(), header.begin());
  header.append(cmd);
  //  std::cout<<"header length = "<<get_length(header)<<std::endl;

  //std::string full_cmd =  header+cmd;
  return header; //full_cmd;
}

//std::string
// L::lecroy_com_manager::serial_poll_byte() 
// {
//   //header.set_clear(true);
//   ///serialPoll=true;
//   //std::string ret = timed_recv("", 3, 3);
//   //serialPoll=false;
//   //return ret;

// }

template<class coms_method>
void
ICR::lecroy::lecroy_com_manager<coms_method>::send(const std::string cmd) 
  throw (boost::system::system_error) 
{
  // bool not_completed = true;
  // while(not_completed){
  // try{
  std::string full_command = header.add(cmd);
  coms_method::send(full_command);
  //wait for operation to complete.

  //  std::string ib= recv("INR?\n");
  // std::cout<<"IB = "<<ib<<std::endl;

  // if (!wait) {
  coms_method::send( header.add("*OPC?\n"));
  try{
    std::string opc_bit = coms_method::timed_recv(20,5,false);
  }
  catch (ICR::exception::exception_in_receive_you_must_resend_command& e)
    {
      
      std::cout<<"lecroy receive exception caught (in send), resending command"<<std::endl;
      send(cmd);
    }
  // }
  // not_completed = false;
  // }
  // catch(exception::timeout_exceeded& e){
  //   e.debug_print();
  //   clear();
  //   std::cout<<"cmd = "<<cmd<<std::endl;
  //   send(cmd); //retry
  // }
  //     std::cout<<"command not completed, retry..."<<std::endl;   
  //   }
  // }
}


template<class coms_method>
std::string
ICR::lecroy::lecroy_com_manager<coms_method>::recv_surp()
{ 
  std::string surp =coms_method::recv(1e6,false);
  return surp;


}

template<class coms_method>
void
ICR::lecroy::lecroy_com_manager<coms_method>::clean_string(std::string& str, const std::string& badchar)
{
  for(size_t i=0;i<badchar.size();++i){
    str.erase(std::remove( str.begin(), 
			   str.end(), badchar[i] )
	      , str.end() );
  }
  
  
}
template<class coms_method>
std::string
ICR::lecroy::lecroy_com_manager<coms_method>::recv(const std::string cmd, const unsigned int& buffsize, const bool& exact )  
  throw (boost::system::system_error) 
{

  //std::cout<<"start recv"<<std::endl;
  //boost::this_thread::sleep(boost::posix_time::milliseconds(500)); 

  try{
    std::string rec ;
  
    // for(;;){ //infinite loop
    // try{
    std::string full_command = header.add(cmd);
    coms_method::send(full_command);

    size_t attempts = 0;
    while (attempts < 500){
      //	std::cout<<"attempts = "<<attempts<<std::endl;

      std::string h = coms_method::timed_recv(8,5,true);
      //std::cout<<"h = "<<h<<std::endl;
      // if (attempts<=1) {
      //   for(size_t i=0;i<h.size();++i){
      //     std::cout<<"header["<<i<<"] = "<<(int) h[i]<<std::endl;
	    
      //   }
      // }

      size_t size = header.get_length(h);
      //	if (attempts<=1) 
      //	std::cout<<"size = "<<size<<std::endl;
      if (header.is_eoi(h)){
	// std::cout<<"eoi"<<std::endl;
	
	rec.append( coms_method::recv(size+1));
	
	// std::cout<<"ret = "<<rec<<std::endl;

	return rec;
      }
      //std::cout<<"not eoi"<<std::endl;
      rec.append(coms_method::recv(size, true));
      // if (attempts==1) 
      //   std::cout<<"rec = "<<rec<<std::endl;
      ++attempts;
    }
  } 
  catch (ICR::exception::exception_in_receive_you_must_resend_command& e)
    {
      
      std::cout<<"lecroy receive exception caught (in recv), resending command"<<std::endl;
      recv(cmd, buffsize,exact);
    }
   
  // }
    
  // catch(exception::timeout_exceeded& e){
  //   e.debug_print();
	
  //   std::cout<<"send = "<<cmd<<std::endl;
  //   clear();
  //   recv(cmd, buffsize, exact ) ; //try again
	
  //   //   std::cout<<"recv command not completed, retry..."<<std::endl;   
  // }
  throw("lecroy recv error: could not read\n") ;
  // }
}
   
//   // //first go
//   std::string h;
//   try{
//     //  std::cout<<"recv cmd = "<<cmd<<std::endl;
//     send(cmd);
//     h = coms_method::timed_recv(8,20, true);
//     // std::cout<<"h size = "<<h.size()<<std::endl;

//     unsigned long size = header.get_length(h);
//     // std::cout<<"size1 = "<<size<<std::endl;
//     //a repetition of the command.
//     //  std::cout<<"body1 = "<<body<<std::endl;
    
//     if (header.is_eoi(h)){
//       std::string body = coms_method::recv(size+1);
//       return body;
//     }
//     std::string body = coms_method::recv(size,true);

//     //erase the question mark
//     // std::string cmd_clean = cmd;
//     // clean_string(cmd_clean, "?,\n");
//     // clean_string(body, "?,\n");
//     // while (cmd_clean!= body){
//     //   std::cout<<"body = "<<body<<" cmd_clean = "<<cmd_clean<<std::endl;
      
//     //   clear(); //wait for everything to sort out.
//     //   try{
//     // 	recv_surp();  //clear up the surpless bytes
//     //   }
//     //   catch(...)
//     // 	{}
//     //   send(cmd); //resend
//     //   body = coms_method::recv(size,true);
//     //   cmd_clean = cmd;
//     //   clean_string(cmd_clean, "?,\n");
//     //   clean_string(body, "?,\n");
//     // }
    
//     //second go - don't repeat the command
//     h  = coms_method::recv(8,true );
//     size = header.get_length(h);
//     //  std::cout<<"size2 = "<<size<<std::endl;
//     if (header.is_eoi(h)){
//       body = coms_method::recv(size+1);
//       return body;
//     }
//     //the number of bytes to follow - ignore
//     body = coms_method::recv(size,true);
//     std::cout<<"body2 = "<<body<<std::endl;
    
//     //get the data
//     size_t attempts = 0;
//     while(!finished && attempts<4) {
//       h= coms_method::recv(cmd,8, true,false);
//       int size = header.get_length(h);
//       //      std::cout<<"size3 = "<<size<<std::endl;

//       //check to see if end of file.
//       if (header.is_eoi(h)){
// 	finished = true;
// 	body = coms_method::recv(size,true);
// 	//	std::cout<<"eoi = "<<std::endl;
//       }
//       else{
// 	//	std::cout<<"get body "<<std::endl;
// 	std::string body = coms_method::recv(size,true);
// 	//std::cout<<"body recved "<<std::endl;
// 	//attach to message
// 	rec.append(body);
//       }
//       // std::cout<<"attempts = "<<attempts<<std::endl;

//       ++attempts;
//     }
//   }
//   catch(...)
//     { 
//       clear();
//     }
//   //std::cout<<"end recv"<<std::endl;
//   send("WAIT\n");
//   return rec;
  
//   // std::string rec;
//   // try{
//   //   rec = coms_method::timed_recv(cmd,buffsize,3, exact);
//   // } catch(...){

//   // }

//   // std::string str_header = rec.substr(0,8);
//   // unsigned long size = header.get_length(str_header);
//   // std::cout<<"header size = "<<size<<std::endl;
//   // //skip the header
//   // std::string rec_no_head = rec.substr(8, rec.size()-8) ;
//   // return rec_no_head;
// }

// template<class coms_method>
// std::string
// ICR::lecroy::lecroy_com_manager<coms_method>::timed_recv(const std::string& cmd, const unsigned int& buffsize , const double& seconds, const bool& exact) 
// { 

//   std::string rec ;
//   std::string full_command = header.add(cmd);
//   coms_method::send(full_command);
//   std::cout<<"cmd = "<<cmd<<std::endl;

//   std::cout<<"cmd size = "<<header.get_length(full_command)<<std::endl;
//   size_t attempts = 0;
//   while (attempts < 500){
//     std::cout<<"attempts = "<<attempts<<std::endl;

//     std::string h = coms_method::timed_recv(8, 3, true);
//     std::cout<<"h = "<<h<<std::endl;

//     size_t size = header.get_length(h);
//    std::cout<<"size = "<<size<<std::endl;
//     if (header.is_eoi(h)){
//       std::cout<<"eoi"<<std::endl;
//       rec.append( coms_method::timed_recv(size+1, 3));
//       std::cout<<"ret = "<<rec<<std::endl;

//       return rec;
//     }
//     std::cout<<"not eoi"<<std::endl;
//     rec.append(coms_method::timed_recv(size, 3, true));
//     std::cout<<"rec = "<<rec<<std::endl;
//     ++attempts;
//   }
//   throw("could not read\n") ;
// }
// std::string rec ;

// // //first go
// std::string h= coms_method::timed_recv(cmd,8,seconds, true);
    // unsigned long size = header.get_length(h);
    // //a repetition of the command.
    // std::string body = coms_method::timed_recv(cmd,size,seconds,true, false);
  
  // //second go - don't repeat the command
  // h  = coms_method::timed_recv(cmd,8,seconds, true, false);
  // size = header.get_length(h);
  // //the number of bytes to follow - ignore
  // body = coms_method::timed_recv(cmd,size,seconds,true, false);

  // //get the data
  // bool finished = false;
  // while(!finished) {
  //   h= coms_method::timed_recv(cmd,8,seconds, true,false);
  //   int size = header.get_length(h);

  //   //check to see if end of file.
  //   if (header.is_eoi(h)){
  //     finished = true;
  //     body = coms_method::timed_recv(cmd,size,seconds,true, false);
  //   }
  //   else{
  //     std::string body = coms_method::timed_recv(cmd,size,seconds,true, false);
  //     //attach to message
  //     rec.append(body);
  //   }
  // }
  // return rec;
  

  
  // std::string rec = coms_method::timed_recv(cmd,buffsize,seconds, exact);
  // std::string str_header = rec.substr(0,8);
  // unsigned long size = header.get_length(str_header);
  // //std::cout<<"header size = "<<size<<std::endl;
  // //skip the header
  // std::string rec_no_head = rec.substr(8, rec.size()-8) ;
  // return rec_no_head;

template<class coms_method>
void
ICR::lecroy::lecroy_com_manager<coms_method>::clear()
{
  header.set_data(false);
  header.set_eoi(false);
  header.set_clear(true);
  coms_method::send(header.add(""));
  header.set_clear(false);
  header.set_data(true);
  header.set_eoi(true);

  coms_method::send(header.add("*CLS\n"));
  //deviceClear= true;
  //send("");
  //deviceClear= false;
}

template<class coms_method>
void
ICR::lecroy::lecroy_com_manager<coms_method>::wait(const double& seconds)
{
  std::string cmd = "WAIT "+stringify(seconds)+"\n";
  coms_method::send(header.add(cmd));
  
  coms_method::send( header.add("*OPC?\n"));
  try{
    std::string opc_bit = coms_method::timed_recv(20,5,false);
  }
  catch (ICR::exception::exception_in_receive_you_must_resend_command& e)
    {
      
      std::cout<<"lecroy receive exception caught (in wait), resending command"<<std::endl;
      wait(seconds);
    }
}

// template<class coms_method>
// void
// ICR::lecroy::lecroy_com_manager<coms_method>::calibrate()
// {
  
//   coms_method::send( header.add("*cal?\n"));
//   std::string opc_bit = coms_method::timed_recv(20,30,false);
// }


template class ICR::lecroy::lecroy_com_manager<ICR::lecroy::IP>;
template class ICR::lecroy::lecroy_com_manager<ICR::lecroy::SERIAL>;

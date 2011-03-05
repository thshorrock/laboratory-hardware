#include <boost/thread.hpp>

#include "DPR500/DPR500.hpp"
#include "DPR500/enums.hpp"

#include "stringify.hpp"


// ICR::pulser::DPR500_com::DPR500_com(const std::string& port)
//   : ICR::coms::serial_manager(port, 
// 			      4800, 
// 			      coms::flow_control::none, 
// 			      coms::parity::none, 
// 			      coms::stop_bits::one, 
// 			      8),
//     m_address(0x01)
// {

//   commands::D_command D;
//   std::cout<<"here1"<<std::endl;

//   commands::I_command I(0x00);
//   std::cout<<"here2"<<std::endl;

//   ICR::coms::serial_manager::send(D); 
//   std::cout<<"here3"<<std::endl;

//   try{
//     std::string ans = ICR::coms::serial_manager::recv(I,9,true); 
//     std::cout<<"ans"<<ans<<std::endl;
  
//     for(size_t i=0;i<ans.size();++i){
//       std::cout<<"["<<i<<"] = "<<(int) ans[i]<<std::endl;
//     }
//     m_address = ans[0];
//     std::cout<<"address = "<<(int) m_address<<std::endl;

//     commands::E_command E(m_address); 
//     ICR::coms::serial_manager::send(E);
//   }
//   catch (boost::system::system_error& e)
//     {}
  
  
// }

// ICR::pulser::DPR500_com::~DPR500_com() {};

// void ICR::pulser::DPR500_com::send(const std::string& cmd)
// {
//   char command = cmd[0];
//   char channel = cmd[1];
//   char rest = (char) atoi(cmd.substr(3,cmd.size()-3).c_str());
//   commands::Function_command* F;
//   if (command != 'b') 
//     F = new commands::Function_command (m_address, command, rest, channel);
//   else
//     F = new commands::Function_command (m_address, command, rest);
//   ICR::coms::serial_manager::send(*F);
//   delete F;
// }

// std::string
// ICR::pulser::DPR500_com::recv(const std::string& cmd, const unsigned int& buffsize )
// {
//   char command;
//   switch ((int) cmd[0])   {
//   case ((int) 'B'): command = 0xE2; break;
//   case ((int) 'F'): command = 0xE6; break;
//   case ((int) 'G'): command = 0xE7; break;
//   case ((int) 'H'): command = 0xE8; break;
//   case ((int) 'I'): command = 0xE9; break;
//   case ((int) 'L'): command = 0xEC; break;
//   case ((int) 'M'): command = 0xED; break;
//   case ((int) 'O'): command = 0xEF; break;
//   case ((int) 'P'): command = 0xF0; break;
//   case ((int) 'T'): command = 0xF4; break;
//   case ((int) 'V'): command = 0xF6; break;
//   default: throw ICR::exception::invalid_query(); break;
//   }
//   char channel = cmd[1];
//   if (cmd[0] != 'I'){ 
    
//     commands::Query_command F(m_address, command,  channel);
//     std::string answer =  ICR::coms::serial_manager::recv(F);
//     if (cmd[0] == 'B') return stringify(atoi(answer.substr(4,1).c_str()));
//     else return stringify(atoi(answer.substr(4,answer.size()-3).c_str()));
    
//   }
//   else {
//     char channel = cmd[1];
//     char channel_option = cmd[2];
//     //cmd[3] = :
    
//     char rest = (char) (atoi(cmd.substr(4,cmd.size()-4).c_str()));
//     commands::QueryI_command F(m_address, command, rest, channel, channel_option);
//     std::string answer =  ICR::coms::serial_manager::recv(F);
//     return stringify(atoi(answer.substr(4,answer.size()-4).c_str()));
//   }
  
// }



// ICR::pulser::RPL2(const enum channel::type& channel,
// 		  /* pulser */
// 		  const double& cal_fall_time = 6.0 /*ns*/,
// 		  const double& cal_min_neg_pulse_amp = 170 /*V*/,
// 		  const double& cal_pulse_width = 109 /*ns FWHM*/,
// 		  /* receiver */
// 		  const double& cal_gain = 5.3 /*db */,
// 		  const double& cal_high_freq_cut_off  = 118 /*MHz*/
// 		  )
//   :  m_gain(new command::gain((short)cal_gain, -13, 66, 1))
//      m_turn_on(),
//      m_turn_off()
// {
  
  
  
//   command::PulserReceiverCommandFactory f;
  
//   f::recv_command_ptr m_gain = f.();
//   f::recv_command_ptr m_turn_on = f.();
//   f::recv_command_ptr m_turn_off = f.instrument_();
//   f::recv_command_ptr m_high_pass_filter = f.();
//   f::recv_command_ptr m_low_pass_filter = f.();

//   send(init);
//   m_address = recv(addr).c_str();
//   send(close);



//   attach_cmd(m_gain);
  
// }

// void 
// ICR::pulser::RPL2::notify()
// {
//   for_each(m_cmds.begin(), m_cmds.end(), update_cmd);
// }
// void 
// ICR::pulser::RPL2::attach_cmd(command_ptr c)
// {
//   c->set_pulser_receiver(*this);
//   m_cmds.push_back(c);
// }

// char 
// ICR::pulser::RPL2::get_channel_char(){
  
//   if ( !attached_to_DPR500 ) throw ICR::exception::gain_command_not_attached_to_DPR500();
//   if (m_pr->get_channel_char() == channel::A) 
//     m_channel_char = 0x41;
//   else
//     m_channel_char = 0x42;
  
// }
      


 ICR::pulser::DPR500::DPR500(const std::string& port)
   :  comm(port, 
	   4800, 
	   coms::flow_control::software, 
	   coms::parity::none, 
	   coms::stop_bits::one, 
	   8),
      m_address(0x01),
      A_attached(false), B_attached(false),
      m_CF(m_address)
      // m_A(), m_B()
{
  init(); 
};
void
ICR::pulser::DPR500::init() throw(ICR::exception::failed_to_contact_DPR500_device)
{
  command::InitiationFactory f;

  typedef command::InitiationFactory::cmd_ptr cmd;
  typedef command::InitiationFactory::recv_cmd_ptr rcmd;
  

  cmd  init = f.initiate_address_assignment();
  //cmd  set_addr = f.set_address(0x10);
  rcmd addr = f.instrument_address();
  cmd  close;
  //bool failed_to_open = true;
  //unsigned int count = 0;
  send(init);

  m_address = timed_recv(addr,1)[0];
  close =  f.end_address_assignment(m_address);
  send(close);
  
  m_CF.set_address(m_address);


  

  m_hpf_A  = m_CF.high_pass_filter<filter::HPFA>(channel::A);
  m_lpf_A  = m_CF.low_pass_filter<filter::LPFA>(channel::A);
  m_hpf_B  = m_CF.high_pass_filter<filter::HPFB>(channel::B);
  m_lpf_B  = m_CF.low_pass_filter<filter::LPFB>(channel::B);

  cmds_A.push_back(m_hpf_A);
  cmds_A.push_back(m_lpf_A);
  cmds_B.push_back(m_hpf_B);
  cmds_B.push_back(m_lpf_B);


  // while (count<3 && failed_to_open){
  //   //send(set_addr);
  //   //boost::this_thread::sleep(boost::posix_time::seconds(1)); //need a pause here
  //   send(init);
  //   try{
  //     failed_to_open = false; 
  //   }
  //   catch (boost::system::system_error& e)
  //     {}
  //   catch (...)
  //     {
  // 	std::cout<<"Something went wrong trying to open serial port"<<std::endl;
  //     }
  //   ++count;
  // }
  // if(failed_to_open) 
  //   {
      
  //     throw ICR::exception::failed_to_contact_DPR500_device();
  //   }
}
void
ICR::pulser::DPR500::send(const boost::shared_ptr<command::cmd>& c)
{
   // std::cout<<"send"<<std::endl;
   // std::string cmd = *c;
   // for(size_t i=0;i<cmd.size();++i){
   //   std::cout<<"["<<i<<"] = "<<(int) cmd[i]<<std::endl;
   // }
  comm.send(*c);
}

std::string
ICR::pulser::DPR500::recv(const boost::shared_ptr<command::recv_cmd>& c)
{
  std::string ans = comm.recv((std::string)*c, c->reply_size(),true);
  
  // std::cout<<"ans = "<<std::endl;
  // for(size_t i=0;i<ans.size();++i){
  //   std::cout<<"["<<i<<"] = "<<(int) ans[i]<<std::endl;

  // }

  // std::string dans = c->decode(ans);
  //  std::cout<<" decoded to "<<dans<<std::endl;
  // for(size_t i=0;i<dans.size();++i){
  //   std::cout<<"["<<i<<"] = "<<(int) dans[i]<<std::endl;
    
  // }


  return c->decode(ans);
}


std::string
ICR::pulser::DPR500::timed_recv(const boost::shared_ptr<command::recv_cmd>& c, 
				const unsigned int& seconds)
{
  

   // std::string cmd = *c;
   // for(size_t i=0;i<cmd.size();++i){
   //   std::cout<<"["<<i<<"] = "<<(int) cmd[i]<<std::endl;
   // }

  std::string ans;
  size_t count = 0; 
  bool not_sent = true;
  while (not_sent){
    try{
      std::string cmd = *c;
      //std::cout<<"DPR500 timed recv, cmd = ";
      // for(size_t i=0;i<cmd.size();++i){
      // 	std::cout<<cmd[i]<<" ";
      // }
      //std::cout<<", seconds = "<<seconds<<std::endl;

      ans = comm.timed_recv((std::string)*c, c->reply_size(),seconds, true);
      //std::cout<<"recvd = "<<ans<<std::endl;

      not_sent=false;
      //A small pause here helps
      boost::this_thread::sleep(boost::posix_time::milliseconds(200)); 
    }
    catch (ICR::exception::timeout_exceeded& e)  {  //boost::system::system_error
      // std::cout<<"serial timeout in DPR500 tr"<<std::endl;

      //Failed, give it a large pause to sort itself out before trying again
      //  boost::this_thread::sleep(boost::posix_time::milliseconds(500)); 
    }
    ++count;
    if (count>20) {

      std::cout<<"The DPR500 is still having problems - you are probably going to have to do something..  Sleeping for 60 seconds."<<std::endl;

      boost::this_thread::sleep(boost::posix_time::seconds(60)); 

    }
    else if (count>10) {

      std::cout<<"The DPR500 is having a problem sending the command "<<std::string(*c)<<std::endl;
      std::cout<<"Please try to fix the problem.  The DPR500 is going to sleep for 2 seconds now, lets hope things sort themselves out"<<std::endl;

       boost::this_thread::sleep(boost::posix_time::milliseconds(2000)); 

    }
  
  }

  // std::cout<<"ans = "<<std::endl;
  // for(size_t i=0;i<ans.size();++i){
  //   std::cout<<"["<<i<<"] = "<<(int) ans[i]<<std::endl;

  // }
  
  // std::string dans = c->decode(ans);
  //  std::cout<<" decoded to "<<dans<<std::endl;
  // for(size_t i=0;i<dans.size();++i){
  //   std::cout<<"["<<i<<"] = "<<(int) dans[i]<<std::endl;
    
  // }

  return c->decode(ans);
}

void
ICR::pulser::DPR500::blink(const short& speed)
{

  command::DPR500CommandFactory::recv_cmd_ptr bc = m_CF.blink(speed);
  DPR500::timed_recv(bc,1);
}


void
ICR::pulser::DPR500::turn_on_A()
{
  if (A_attached) {
    command::DPR500CommandFactory::recv_cmd_ptr m_on = m_CF.turn_on(channel::A);
    DPR500::timed_recv(m_on,1);
  }
}

void
ICR::pulser::DPR500::turn_on_B()
{
  if (B_attached) {
    command::DPR500CommandFactory::recv_cmd_ptr m_on = m_CF.turn_on(channel::B);
    DPR500::timed_recv(m_on,1);
  }
}


void
ICR::pulser::DPR500::turn_off_A()
{
  if (A_attached) {
    command::DPR500CommandFactory::recv_cmd_ptr m_off = m_CF.turn_off(channel::A);
    DPR500::timed_recv(m_off,1);
  }
}

void
ICR::pulser::DPR500::turn_off_B()
{
  if (B_attached) {
    command::DPR500CommandFactory::recv_cmd_ptr m_off = m_CF.turn_off(channel::B);
    DPR500::timed_recv(m_off,1);
  }
}





// void 
// ICR::pulser::DPR500::gain(boost::shared_ptr<pulse_receiver> p)
// {
//   std::string cmd;
//   cmd.push_back(m_address);
//   cmd.push_back(0x67); //g
//   cmd.push_back(p->get_gain_char());
//   cmd.push_back(p->get_channel_char());
//   cmd.push_back(0x00);
//   ICR::coms::serial_manager::send(cmd);
// }

// void 
// ICR::pulser::DPR500::master(boost::shared_ptr<pulse_receiver> p)
// {
//   std::string cmd;
//   cmd.push_back(m_address);
//   cmd.push_back(0x01); 
//   cmd.push_back(0x6D); //m
//   if (p->is_master()) 
//     cmd.push_back(0);
//   else
//     cmd.push_back(1);
//   cmd.push_back(p->get_channel_char());
//   cmd.push_back(0x00);
//   ICR::coms::serial_manager::send(cmd);
// }


// void 
// ICR::pulser::DPR500::on_off(boost::shared_ptr<pulse_receiver> p)
// {
//   std::string cmd;
//   cmd.push_back(m_address);
//   cmd.push_back(0x01); 
//   cmd.push_back(0x6F); //o
//   if (p->is_on()) 
//     cmd.push_back(1);
//   else
//     cmd.push_back(0);
//   cmd.push_back(p->get_channel_char());
//   cmd.push_back(0x00);
//   ICR::coms::serial_manager::send(cmd);
//   if (p->is_on())
//     pause();
// }



// void 
// ICR::pulser::DPR500::prf(boost::shared_ptr<pulse_receiver> p)
// {
//   std::string cmd;
//   cmd.push_back(m_address);
//   cmd.push_back(0x01); 
//   cmd.push_back(0x6F); //o
//   if (p->is_on()) 
//     cmd.push_back(1);
//   else
//     cmd.push_back(0);
//   cmd.push_back(p->get_channel_char());
//   cmd.push_back(0x00);
//   ICR::coms::serial_manager::send(cmd);
//   if (p->is_on())
//     pause();
// }





// void 
// ICR::pulser::DPR500::send_and_confirm(boost::shared_ptr<command::recv_cmd> p)
// {
//    recv(*p, p->reply_size(), true );
// }



// void 
// ICR::pulser::DPR500::notify_hardware(pulser_receiver* p)
// {
//   std::cout<<"notify hardware"<<std::endl;

//   std::list<boost::shared_ptr<command::PulserReceiver_cmd> > recv_cmds= p->get_commands();
  
//   std::cout<<"number of commands = "<<recv_cmds.size()<<std::endl;
  
//   // size_t count = 0; 
//   // bool not_sent = true;
  
//   // while (count <5 && not_sent){
//   //   try{
    
//   //     not_sent=false;
//   //   }
//   //   catch (boost::system::system_error& e)  {
//   //     boost::this_thread::sleep(boost::posix_time::seconds(2)); //need a pause here before trying again
//   //   }
//   //   ++count;
//   // }
//   // if (not_sent) 
//   //   std::cout<<"could not communicate with pulser-receiver"<<std::endl;
  
// }

void 
ICR::pulser::DPR500::notify()
{
  std::list<boost::shared_ptr<command::recv_cmd> > recv_cmds;
  if ( A_attached ) {
    std::list<boost::shared_ptr<command::PulserReceiver_cmd> > recv_cmds_A 
      = m_A->get_commands();
    recv_cmds.insert(recv_cmds.end(), recv_cmds_A.begin(), recv_cmds_A.end());
    recv_cmds.insert(recv_cmds.end(), cmds_A.begin(), cmds_A.end());
    
  }
  if ( B_attached ) {
    std::list<boost::shared_ptr<command::PulserReceiver_cmd> > recv_cmds_B
      = m_B->get_commands();
    recv_cmds.insert(recv_cmds.end(), recv_cmds_B.begin(), recv_cmds_B.end());
    recv_cmds.insert(recv_cmds.end(), cmds_B.begin(), cmds_B.end());
  }
  for_each(recv_cmds.begin(), recv_cmds.end(), 
	       boost::bind(&DPR500::timed_recv,boost::ref(*this), _1, 1)  );
}

void 
ICR::pulser::DPR500::attach_A(pulser_receiver* p)
{
  std::string supposed_serial_number = p->get_serial_number();
  boost::shared_ptr<command::recv_cmd> 
    serial_request = m_CF.pulser_serial_number(channel::A);
  
  std::string actual_serial_number = timed_recv(serial_request);
  
  if (actual_serial_number!=supposed_serial_number) {
    throw ICR::exception::incorrect_serial_number_for_pulser();
  }
    
  A_attached = true; 
  m_A = p;
  m_A->set_state(channel::A, m_address);

  m_gain_A = m_CF.gain(channel::A,m_A->get_gain(),-13,66,1);
  cmds_A.push_back(m_gain_A);
  
}

void 
ICR::pulser::DPR500::attach_B(pulser_receiver* p)
{

  std::string supposed_serial_number = p->get_serial_number();
  boost::shared_ptr<command::recv_cmd> 
    serial_request = m_CF.pulser_serial_number(channel::B);
  
  std::string actual_serial_number = timed_recv(serial_request);
  
  if (actual_serial_number!=supposed_serial_number) {
    throw ICR::exception::incorrect_serial_number_for_pulser();
  }


  B_attached = true; 
  m_B = p; 
  m_B->set_state(channel::B, m_address);
  m_gain_B = m_CF.gain(channel::B,m_B->get_gain(), -22,50,1);
  cmds_B.push_back(m_gain_B);
}

void ICR::pulser::DPR500::detach_A()
{
  A_attached = false;
  m_A->detach();
  cmds_A.remove(m_gain_A);
};
void ICR::pulser::DPR500::detach_B()
{
  B_attached = false;
  m_B->detach();
  cmds_B.remove(m_gain_B);
}


// void 
// ICR::pulser::DPR500::high_pass_filter(const channel::type& c, const unsigned int& value)
// {
//   std::string cmd;
//   cmd.push_back('H');
//   if (c == channel::A) 
//     cmd.appemd("A:");
//   else cmd.append("B:");
//   cmd.push_back((char) value);
//   send(cmd);
// }

// void 
// ICR::pulser::DPR500::low_pass_filter(const channel::type& c, const unsigned int& value)
// {
//   std::string cmd;
//   cmd.push_back('L');
//   if (c == channel::A) 
//     cmd.appemd("A:");
//   else cmd.append("B:");
//   cmd.push_back((char) value);
//   send(cmd);
// }

// void 
// ICR::pulser::DPR500::triggering(const channel::type& c,const trigger::type t){
 
//   std::string cmd;
//   cmd.push_back('M');
//   if (c == channel::A) 
//     cmd.appemd("A:");
//   else cmd.append("B:");
//   if (t == trigger::MASTER) 
//     cmd.push_back(0);
//   else
//     cmd.push_back(1);
//   send(cmd);
// }
// void 
// ICR::pulser::DPR500::on(const channel::type& c)
// {
//   if (c == channel::A) 
//     send("OA:1");
//   else send("OB:1");
// }
// void 
// ICR::pulser::DPR500::off(const channel::type& c)
// {
//   if (c == channel::A) 
//     send("OA:0");
//   else send("OB:0");
// }

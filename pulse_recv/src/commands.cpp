#include "DPR500/commands.hpp"
#include "DPR500/exceptions.hpp"
#include "DPR500/basic_pulser_receiver.hpp"


#include<sstream>



boost::shared_ptr<ICR::pulser::command::initiate_address_assignment> 
ICR::pulser::command::InitiationFactory::initiate_address_assignment() const
{
  return boost::shared_ptr<ICR::pulser::command::initiate_address_assignment> ( new ICR::pulser::command::initiate_address_assignment() );
}


boost::shared_ptr<ICR::pulser::command::end_address_assignment> 
ICR::pulser::command::InitiationFactory::end_address_assignment(const char& address) const
{
  return boost::shared_ptr<ICR::pulser::command::end_address_assignment> ( new ICR::pulser::command::end_address_assignment(address) );
}

boost::shared_ptr<ICR::pulser::command::set_address> 
ICR::pulser::command::InitiationFactory::set_address(const char& address) const
{
  return boost::shared_ptr<ICR::pulser::command::set_address> ( new ICR::pulser::command::set_address(address) );
}


boost::shared_ptr<ICR::pulser::command::instrument_address> 
ICR::pulser::command::InitiationFactory::instrument_address() const
{
  return boost::shared_ptr<ICR::pulser::command::instrument_address> ( new ICR::pulser::command::instrument_address() );
}

boost::shared_ptr<ICR::pulser::command::instrument_type> 
ICR::pulser::command::InitiationFactory::instrument_type() const
{
  return boost::shared_ptr<ICR::pulser::command::instrument_type> ( new ICR::pulser::command::instrument_type() );
}


boost::shared_ptr<ICR::pulser::command::instrument_serial_number> 
ICR::pulser::command::InitiationFactory::instrument_serial_number() const
{
  return boost::shared_ptr<ICR::pulser::command::instrument_serial_number> ( new ICR::pulser::command::instrument_serial_number() );
}

boost::shared_ptr<ICR::pulser::command::instrument_firmware> 
ICR::pulser::command::InitiationFactory::instrument_firmware() const
{
  return boost::shared_ptr<ICR::pulser::command::instrument_firmware> ( new ICR::pulser::command::instrument_firmware() );
}

boost::shared_ptr<ICR::pulser::command::DPR500_serial_number> 
ICR::pulser::command::InitiationFactory::DPR500_serial_number() const
{
  return boost::shared_ptr<ICR::pulser::command::DPR500_serial_number> ( new ICR::pulser::command::DPR500_serial_number() );
}

ICR::pulser::command::blink::blink(const char& address, const short& speed)
  : m_cmd()
{
  short sp = speed;
  if (speed<50) sp = 50;
  if (speed>200) sp = 200;
  m_cmd.push_back(address);
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x62); //b
  m_cmd.push_back( (char) sp);
  m_cmd.push_back(0x00);
}



ICR::pulser::command::initiate_address_assignment::initiate_address_assignment()
{
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x44);
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x00);
}

ICR::pulser::command::end_address_assignment::end_address_assignment(const char& address)
{
  set_cmd(address);
}

void
ICR::pulser::command::end_address_assignment::set_cmd(const  char& address)
{
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x45);
  m_cmd.push_back(address);
  m_cmd.push_back(0x00);
}


ICR::pulser::command::set_address::set_address(const  char& address)
{
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x41);
  m_cmd.push_back(address);
  m_cmd.push_back(0x00);
}

ICR::pulser::command::instrument_address::instrument_address()
{
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x49);
  m_cmd.push_back(0x02);
  m_cmd.push_back(0x00);
}

std::string
ICR::pulser::command::instrument_address::decode(const std::string& ans)
{
  return ans.substr(0,1); //the address char
}


ICR::pulser::command::instrument_type::instrument_type()
{ 
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x49);
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x00);
}

std::string
ICR::pulser::command::instrument_type::decode(const std::string& ans)
{
  return ans.substr(3,7);
}


ICR::pulser::command::instrument_serial_number::instrument_serial_number()
{
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x49);
  m_cmd.push_back(0x01);
  m_cmd.push_back(0x00);
}


std::string
ICR::pulser::command::instrument_serial_number::decode(const std::string& ans)
{
  return ans.substr(3,7);
}


ICR::pulser::command::instrument_firmware::instrument_firmware()
{
 m_cmd.push_back(0x00);
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x49);
  m_cmd.push_back(0x02);
  m_cmd.push_back(0x00);
}


std::string
ICR::pulser::command::instrument_firmware::decode(const std::string& ans)
{
  return ans.substr(3,4);
}



ICR::pulser::command::DPR500_serial_number::DPR500_serial_number()
{
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x00);
  m_cmd.push_back(0x49);
  m_cmd.push_back(0x03);
  m_cmd.push_back(0x00);
}


std::string
ICR::pulser::command::DPR500_serial_number::decode(const std::string& ans)
{
  return ans.substr(3,7);
}



// ICR::pulser::command::
// {

// }


// std::string
// ICR::pulser::command::::decode(const std::string& ans)
// {

// }


// ICR::pulser::command::
// {

// }


// std::string
// ICR::pulser::command::::decode(const std::string& ans)
// {

// }



// // ICR::pulser::command::D_command::D_command()
// // {
// //   m_cmd.push_back(0x00);
// //   m_cmd.push_back(0x00);
// //   m_cmd.push_back(0x44);
// //   m_cmd.push_back(0x00);
// //   m_cmd.push_back(0x00);
// //   std::cout<<"D COMMAND"<<std::endl;

// //   for(size_t i=0;i<m_cmd.size();++i){
// //     std::cout<<"["<<i<<"] = "<<(int) m_cmd[i]<<std::endl;
// //   }
// // }



// ICR::pulser::command::I_command::I_command(const  char& i)
// {
//   m_cmd.push_back(0x00);
//   m_cmd.push_back(0x00);
//   m_cmd.push_back(0x49);
//   m_cmd.push_back(i);
//   m_cmd.push_back(0x00);
//   std::cout<<"I COMMAND"<<std::endl;

//   for(size_t i=0;i<m_cmd.size();++i){
//     std::cout<<"["<<i<<"] = "<<(int) m_cmd[i]<<std::endl;
//   }


// }

// ICR::pulser::command::A_command::A_command(const  char& address)
// {
//   m_cmd.push_back(0x00);
//   m_cmd.push_back(0x00);
//   m_cmd.push_back(0x41);
//   m_cmd.push_back( address);
//   m_cmd.push_back(0x00);
// }

// ICR::pulser::command::E_command::E_command(const  char& address)

// {
//   m_cmd.push_back(0x00);
//   m_cmd.push_back(0x00);
//   m_cmd.push_back(0x45);
//   m_cmd.push_back( address);
//   m_cmd.push_back(0x00);
// }


// ICR::pulser::command::Function_command::Function_command(const char& address, const char& command, const char& value, const char& channel)
// {
//   m_cmd.push_back( address);//bit 1
//   // if (command == "B") m_cmd.push_back(0x00);
//   // else if (command == "P" || command == "V") m_cmd.push_back(0x02);
//   // else m_cmd.push_back(0x01);
  

//   switch ((long) command) {//byte 2 //length
//   case ((long) 'B'): m_cmd.push_back(0x00); break;
//   case ((int) 'P'): m_cmd.push_back(0x02); break;
//   case ((int) 'V'): m_cmd.push_back(0x02); break;
//    default: m_cmd.push_back(0x01); break;
//   }
	  
//   m_cmd.push_back(command);//bit 3
//   m_cmd.push_back(value);//bit 4
//   m_cmd.push_back(channel); //bit 5
//   m_cmd.push_back(0x00);  //bit 6
	  
// }


// ICR::pulser::command::Query_command::Query_command(const char& address, const char& command,  const  char& channel)
// {
//   m_cmd.push_back(address);//bit 1
//   m_cmd.push_back(0x00); //bit 2
	  
//   m_cmd.push_back(command);//bit 3
//   m_cmd.push_back(channel); //bit 4
//   m_cmd.push_back(0x00);  //bit 5
	  
// }
// ICR::pulser::command::QueryI_command::QueryI_command(const char& address, const char& command, const char& value, const char& channel, const char& channel_option)
// {
//   m_cmd.push_back( address);//bit 1
//   m_cmd.push_back(0x00); //bit 2
	  
//   m_cmd.push_back(command);//bit 3
//   m_cmd.push_back(value); //bit 4
//   m_cmd.push_back(channel); //bit 5
//   m_cmd.push_back(channel_option); //bit 6
//   m_cmd.push_back(0x00);  //bit 7
	  
// }

ICR::pulser::command::PulserReceiver_cmd::PulserReceiver_cmd()
  : recv_cmd()
{}
void
ICR::pulser::command::PulserReceiver_cmd::update()
{
  m_channel_char = m_pr->get_channel_char();
  m_address = m_pr->get_address_char();
  set_cmd();
}



ICR::pulser::command::pulser_serial_number::pulser_serial_number(const char& addr, const enum channel::type& channel
				 )
  : DPR500_recv_cmd(),
    m_address(addr)
{
  if ( channel == channel::A) 
    m_channel_char =  0x41;
  else
    m_channel_char =  0x42;
  set_cmd();
}

void 
ICR::pulser::command::pulser_serial_number::set_cmd()
{
  m_cmd.clear();
  m_cmd.push_back(m_address);
  m_cmd.push_back(0x02);
  m_cmd.push_back(0xE9); //qi
  m_cmd.push_back(0x1); //serial
  m_cmd.push_back(m_channel_char);
  m_cmd.push_back('P');
  m_cmd.push_back(0x00);


}




std::string
ICR::pulser::command::pulser_serial_number::decode(const std::string& ans)
{
  return ans.substr(3,6);
}


ICR::pulser::command::gain::gain(const char& addr, const enum channel::type& channel,
				 const short& measured, 
				 const short& min, 
				 const short& max, 
				 const short step
				 )
  : DPR500_recv_cmd(),
    m_measured(measured), m_min(min), m_max(max), m_step(step),
    m_gain_char((short)(max-min)/2),
     m_address(addr)
{
  if (max<min) throw ICR::exception::incorrect_input_to_gain_command();
  if ( channel == channel::A) 
    m_channel_char =  0x41;
  else
    m_channel_char =  0x42;
  set_cmd();
}

void 
ICR::pulser::command::gain::set_cmd()
{
  // if ( !attached_to_receiver ) throw gain_command_not_attached_to_reveiver();
  // if ( !attached_to_DPR500 ) throw gain_command_not_attached_to_DPR500();
  m_cmd.clear();
  m_cmd.push_back(m_address);
  m_cmd.push_back(0x01);
  m_cmd.push_back(0x67); //g
  m_cmd.push_back(m_gain_char);
  m_cmd.push_back(m_channel_char);
  m_cmd.push_back(0x00);

  // std::cout<<"gain command = "<<std::endl;
  // for(size_t i=0;i<PulserReceiver_cmd::m_cmd.size();++i){
  //   std::cout<<"["<<i<<"]="<<(int)  PulserReceiver_cmd::m_cmd[i] <<std::endl;

  // }

}


void
ICR::pulser::command::gain::set(const short& g)
{
  const short actual_gain = g-m_measured;
  if (actual_gain<m_min || actual_gain>m_max) 
    throw ICR::exception::requested_gain_out_of_bounds();
  m_gain_char = (char) (actual_gain - m_min);
  set_cmd();
};


std::string
ICR::pulser::command::gain::decode(const std::string& ans)
{
  char m_gain_char = *ans.substr(3,1).c_str();
  short actual_gain = (int) m_gain_char + m_min;
  short requested_gain = actual_gain + m_measured;
  return stringify(requested_gain);
}



ICR::pulser::command::master::master(const bool& is_master)
  : PulserReceiver_cmd(), m_master(is_master)
{}
void 
ICR::pulser::command::master::set(const bool& is_master)
{
  if (is_master) m_master=true;
  else m_master=false;
  set_cmd();
}



void 
ICR::pulser::command::master::set_cmd()
{ 
  PulserReceiver_cmd::m_cmd.clear();
  PulserReceiver_cmd::m_cmd.push_back(m_address);
  PulserReceiver_cmd::m_cmd.push_back(0x01); 
  PulserReceiver_cmd::m_cmd.push_back(0x6D); //m
  if (m_master) 
    PulserReceiver_cmd::m_cmd.push_back(0);
  else
    PulserReceiver_cmd::m_cmd.push_back(1);
  PulserReceiver_cmd::m_cmd.push_back(m_channel_char);
  PulserReceiver_cmd::m_cmd.push_back(0x00);
}




 
ICR::pulser::command::turn_on::turn_on(const char& addr, const enum channel::type& channel)
  :  DPR500_recv_cmd(),
     m_cmd(),
     m_address(addr)
{
  if ( channel == channel::A) 
    m_channel_char =  0x41;
  else
    m_channel_char =  0x42;
  set_cmd();
}

void
ICR::pulser::command::turn_on::set_cmd()
{
  m_cmd.clear();
  m_cmd.push_back(m_address);
  m_cmd.push_back(0x01); 
  m_cmd.push_back(0x6F); //o
  m_cmd.push_back(1); //on
  m_cmd.push_back(m_channel_char);
  m_cmd.push_back(0x00);
}



ICR::pulser::command::turn_off::turn_off(const char& addr, const enum channel::type& channel)
  :  DPR500_recv_cmd(),
     m_cmd(),
     m_address(addr)
{
  if ( channel == channel::A) 
    m_channel_char =  0x41;
  else
    m_channel_char =  0x42;
  set_cmd();
}

void
ICR::pulser::command::turn_off::set_cmd()
{
  m_cmd.clear();
  m_cmd.push_back(m_address);
  m_cmd.push_back(0x01); 
  m_cmd.push_back(0x6F); //o
  m_cmd.push_back(0); //off
  m_cmd.push_back(m_channel_char);
  m_cmd.push_back(0x00);
}




ICR::pulser::command::prf::prf(const unsigned short& max_prf)
  : PulserReceiver_cmd(),
    m_prf(),
    m_max_prf(max_prf)
{}

double  
ICR::pulser::command::prf::set(const double& frequency)
{
  double f = frequency; //make a copy
  if (frequency>m_max_prf) 
    f = m_max_prf;
  if (frequency<183.108263) 
    f = 183.108263;  //smallest frequency stored in 16 bit word
  double value = 12000000.0/f;

   
  //round value to nearest integer
  unsigned short s = (short) (value+0.5);

  m_prf[1] = s; //least significant
  m_prf[0] = s>>8;  //most significant

  set_cmd();
  return float(12000000)/float(s);
}
std::string 
ICR::pulser::command::prf::decode(const std::string& ans)
{
  std::string a = ans.substr(3,2);
  unsigned short s = 0x00;
  s |= a[1];  //least significant
  s |= (a[0]<<8);//most significant
  return stringify(  float(12000000)/float(s)   );
}



void 
ICR::pulser::command::prf::set_cmd()
{
  PulserReceiver_cmd::m_cmd.clear();
  PulserReceiver_cmd::m_cmd.push_back(m_address);
  PulserReceiver_cmd::m_cmd.push_back(0x02); 
  PulserReceiver_cmd::m_cmd.push_back(0x70); //p
  PulserReceiver_cmd::m_cmd.push_back(m_prf[0]);
  PulserReceiver_cmd::m_cmd.push_back(m_prf[1]);
  PulserReceiver_cmd::m_cmd.push_back(m_channel_char);
  PulserReceiver_cmd::m_cmd.push_back(0x00);
}


 
ICR::pulser::command::external_trigger::external_trigger()
  : PulserReceiver_cmd()
{}

void 
ICR::pulser::command::external_trigger::set_cmd()
{
  PulserReceiver_cmd::m_cmd.clear();
  PulserReceiver_cmd::m_cmd.push_back(m_address);
  PulserReceiver_cmd::m_cmd.push_back(0x01); 
  PulserReceiver_cmd::m_cmd.push_back(0x74); //t
  PulserReceiver_cmd::m_cmd.push_back(1);
  PulserReceiver_cmd::m_cmd.push_back(m_channel_char);
  PulserReceiver_cmd::m_cmd.push_back(0x00);
}



ICR::pulser::command::internal_trigger::internal_trigger()
  : PulserReceiver_cmd()
{}

void 
ICR::pulser::command::internal_trigger::set_cmd()
{
  PulserReceiver_cmd::m_cmd.clear();
  PulserReceiver_cmd::m_cmd.push_back(m_address);
  PulserReceiver_cmd::m_cmd.push_back(0x01); 
  PulserReceiver_cmd::m_cmd.push_back(0x74); //t
  PulserReceiver_cmd::m_cmd.push_back(0);
  PulserReceiver_cmd::m_cmd.push_back(m_channel_char);
  PulserReceiver_cmd::m_cmd.push_back(0x00);
}




ICR::pulser::command::voltage::voltage(const double& min, const double& max)
  : PulserReceiver_cmd(),
    m_voltage(),
    m_min(min), m_max(max)
{
  voltage::set(m_min);
}

void
ICR::pulser::command::voltage::set_cmd()
{
  PulserReceiver_cmd::m_cmd.clear();
  PulserReceiver_cmd::m_cmd.push_back(m_address);
  PulserReceiver_cmd::m_cmd.push_back(0x02); 
  PulserReceiver_cmd::m_cmd.push_back(0x76); //v
  PulserReceiver_cmd::m_cmd.push_back(m_voltage[0]);
  PulserReceiver_cmd::m_cmd.push_back(m_voltage[1]);
  PulserReceiver_cmd::m_cmd.push_back(m_channel_char);
  PulserReceiver_cmd::m_cmd.push_back(0x00);
  
}
void
ICR::pulser::command::voltage::set(const double& voltage)
{
  //round value to nearest integer
  if (voltage<m_min || voltage>m_max)  throw ICR::exception::voltage_out_of_bounds();
  short s = (short) (voltage+0.5 );
  m_voltage[1] = s; //least significant
  m_voltage[0] = s>>8; //most significant
  // std::cout<<"v = "<<s<<std::endl;
  // std::cout<<"v[0] = "<<(int) m_voltage[0]<<std::endl;
  // std::cout<<"v[1] = "<<(int) m_voltage[1]<<std::endl;


  set_cmd();
}

std::string 
ICR::pulser::command::voltage::decode(const std::string& ans)
{
  // for(size_t i=0;i<ans.size();++i){
  //   std::cout<<"["<<i<<"] = "<<(int) ans[i]<<std::endl;

  // }

  std::string a = ans.substr(3,2);

  
  // for(size_t i=0;i<a.size();++i){
  //   std::cout<<"["<<i<<"] = "<<(int) a[i]<<std::endl;

  // }
  unsigned short s;
  
  std::bitset<8> bit_lsb(a[0]);
  std::bitset<8> bit_msb(a[1]); 
  std::bitset<16> bit_total;
  for(size_t i=0;i<8;++i){
    bit_total.set(i, bit_msb.test(i));
  }
  for(size_t i=8;i<16;++i){
    bit_total.set(i, bit_lsb.test(i-8));
  }

  // for(size_t i=0;i<10;++i){
  //   std::cout<<"bit_total = "<<bit_total.test(i)<<std::endl;

  // }

  s = bit_total.to_ulong();
  
  
   // std::cout<<"qv ans = "<<ans<<std::endl;
   // std::cout<<"qv dec = "<<stringify(s)<<std::endl;
  return stringify(s);
}


ICR::pulser::command::query_voltage::query_voltage()
  : PulserReceiver_cmd()
{
  query_voltage::set_cmd();
}

void
ICR::pulser::command::query_voltage::set_cmd()
{

  PulserReceiver_cmd::m_cmd.clear();
  PulserReceiver_cmd::m_cmd.push_back(m_address);
  PulserReceiver_cmd::m_cmd.push_back(0x00); 
  PulserReceiver_cmd::m_cmd.push_back(0xF6); //v
  PulserReceiver_cmd::m_cmd.push_back(m_channel_char);
  PulserReceiver_cmd::m_cmd.push_back(0x00);
  
}

std::string 
ICR::pulser::command::query_voltage::decode(const std::string& ans)
{


  std::string a = ans.substr(3,2);
  unsigned short s;
  
  std::bitset<8> bit_lsb(a[0]);
  std::bitset<8> bit_msb(a[1]); 
  std::bitset<16> bit_total;
  for(size_t i=0;i<8;++i){
    bit_total.set(i, bit_msb.test(i));
  }
  for(size_t i=8;i<16;++i){
    bit_total.set(i, bit_lsb.test(i-8));
  }


  s = bit_total.to_ulong();
  
  std::cout<<"query voltage = "<<stringify(s)<<std::endl;
  return stringify(s);
}




// // void
// // ICR::pulser::command::low_energy::set_cmd()
// // {
  

// // 	PulserReceiver_cmd::m_cmd.push_back(m_address);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x01);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x66); //f
// // 	PulserReceiver_cmd::m_cmd.push_back(0);
// // 	PulserReceiver_cmd::m_cmd.push_back(s>>8);
// // 	PulserReceiver_cmd::m_cmd.push_back(m_channel_char);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x00);

// // }
// // void
// // ICR::pulser::command::high_energy::set_cmd()
// // {
  

// // 	PulserReceiver_cmd::m_cmd.push_back(m_address);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x01);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x66); //f
// // 	PulserReceiver_cmd::m_cmd.push_back(1);
// // 	PulserReceiver_cmd::m_cmd.push_back(s>>8);
// // 	PulserReceiver_cmd::m_cmd.push_back(m_channel_char);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x00);

// // }


// // void
// // ICR::pulser::command::echo_mode::set_cmd()
// // {
  

// // 	PulserReceiver_cmd::m_cmd.push_back(m_address);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x01);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x66); //f
// // 	PulserReceiver_cmd::m_cmd.push_back(1);
// // 	PulserReceiver_cmd::m_cmd.push_back(s>>8);
// // 	PulserReceiver_cmd::m_cmd.push_back(m_channel_char);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x00);

// // }


// // void
// // ICR::pulser::command::pulser::set_cmd()
// // {
  

// // 	PulserReceiver_cmd::m_cmd.push_back(m_address);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x01);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x66); //f
// // 	PulserReceiver_cmd::m_cmd.push_back(1);
// // 	PulserReceiver_cmd::m_cmd.push_back(s>>8);
// // 	PulserReceiver_cmd::m_cmd.push_back(m_channel_char);
// // 	PulserReceiver_cmd::m_cmd.push_back(0x00);

// // }

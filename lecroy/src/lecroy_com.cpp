#include "lecroy/lecroy_com.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include "boost/date_time/posix_time/posix_time_types.hpp" //no i/o just types
#include <stdlib.h>
#include <stdio.h>
#include <bitset>
#include <boost/thread.hpp>
namespace L = ICR::lecroy;
std::string
L::coupling::get_string(const enum coupling::type& opt)
{
  switch (opt) {
  case coupling::DC: return std::string("DC"); break;
  case coupling::AC: return std::string("AC"); break;
  case coupling::A1M: return std::string("A1M"); break;
  case coupling::D1M: return std::string("D1M"); break;
  case coupling::D50: return std::string("D50"); break;
  case coupling::GND: return std::string("GND"); break;
  default: throw exception::coupling_option_not_recognised();
  }
}

std::string
L::trigger_mode::get_string(const enum trigger_mode::type& opt)
{
  switch (opt) {
  case trigger_mode::AUTO: return std::string("AUTO"); break;
  case trigger_mode::NORM: return std::string("NORM"); break;
  case trigger_mode::SINGLE: return std::string("SINGLE"); break;
  case trigger_mode::STOP: return std::string("STOP"); break;
  default: throw exception::trigger_mode_not_recognised();
  }
}

// std::string
// L::trigger_state::get_string(const enum ::type& opt)
// {
//   switch (opt) {
//   case coupling::LOW return std::string("L"); break;
//   case coupling::HIGH: return std::string("H"); break;
//   case coupling::L: return std::string("L"); break;
//   case coupling::H: return std::string("H"); break;
//   default: throw exception::trigger_state_not_recognised();
//   }
// }

std::string
L::trigger_type::get_string(const enum trigger_type::type& opt)
{
  switch (opt) {
  case DROPOUT: return std::string("DROP"); break;
  case EDGE: return std::string("EDGE"); break;
  case GLITCH: return std::string("GLIT"); break;
  case INTERVAL: return std::string("INTV"); break;
  case STD: return std::string("STD"); break;
  case SINGLE_SOURCE: return std::string("SNG"); break;
  case STATE_QUALIFIED: return std::string("SQ"); break;
  case EDGE_QUALIFIED: return std::string("TEQ"); break;
  default: throw exception::trigger_type_not_recognised();
  }
}

std::string
L::trigger_hold_type::get_string(const enum trigger_hold_type::type& opt)
{
  switch (opt) {
  case TIME_GREATER_THAN: return std::string("TI"); break;
  case TIME_WIDTH: return std::string("TL"); break;
  case EVENTS: return std::string("EV"); break;
  case PULSE_SMALLER_THAN: return std::string("PS"); break;
  case PULSE_GREATER_THAN: return std::string("PL"); break;
  case INTERVAL_SMALLER_THAN: return std::string("IS"); break;
  case INTEVAL_GREATER_THAN: return std::string("IL"); break;
  case PULSE_WIDTH: return std::string("P2"); break;
  case INTERVAL_WIDTH: return std::string("I2"); break;
  case NO_HOLD: return std::string("OFF"); break;
  default: throw exception::trigger_hold_type_not_recognised();
  }
}
std::string
L::location::get_string(const enum location::type& opt)
{
  switch(opt) {
  case C1: return std::string("C1"); break;
  case C2: return std::string("C2"); break;
  case C3: return std::string("C3"); break;
  case C4: return std::string("C4"); break;
  case F1: return std::string("F1"); break;
  case F2: return std::string("F2"); break;
  case F3: return std::string("F3"); break;
  case F4: return std::string("F4"); break;
  case F5: return std::string("F5"); break;
  case F6: return std::string("F6"); break;
  case F7: return std::string("F7"); break;
  case F8: return std::string("F8"); break;
  case M1: return std::string("M1"); break;
  case M2: return std::string("M2"); break;
  case M3: return std::string("M3"); break;
  case M4: return std::string("M4"); break;
  case ALL_DISPLAYED: return std::string("ALL_DISPLAYED"); break;
  case EX: return std::string("EX"); break;
  case EX10: return std::string("EX10"); break;
  case ETM10: return std::string("ETM10"); break;
  case FILE: return std::string("FILE"); break;
  case HDD: return std::string("HDD"); break;
  default: throw exception::channel_value_not_valid();
  }
  
}
// std::string
// L::destination::get_string(const enum ::type& opt)
// {
//   switch(opt) {
//   case M1: return std::string("M1"); break;
//   case M2: return std::string("M2"); break;
//   case M3: return std::string("M3"); break;
//   case M4: return std::string("M4"); break;
//   case FILE: return std::string("FILE"); break;
//   case HDD: return std::string("HDD"); break;
//   default: throw exception::destination_value_not_valid();
//   }
// }

std::string
L::store_mode::get_string(const enum store_mode::type& opt)
{
  switch(opt) {
  case NO_AUTOSAVE: return std::string("OFF"); break;
  case FILL: return std::string("FILL"); break;
  case WRAP: return std::string("WRAP"); break;
  default: throw exception::store_mode_not_valid();
  }
}

std::string
L::store_format::get_string(const enum store_format::type& opt)
{
  switch(opt) {
  case ASCII: return std::string("ASCII"); break;
  case BINARY: return std::string("BINARY"); break;
  case EXCEL: return std::string("EXCEL"); break;
  case MATHCAD: return std::string("MATHCAD"); break;
  case MATLAB: return std::string("MATLAB"); break;
  default: throw exception::store_format_not_valid();
  }
}

template<class coms_method>
L::lecroy_com<coms_method>::lecroy_com(const std::string& device)
  : lecroy_com_manager<coms_method>(device)
{
  
  clear();
  // wait();

}


template<class coms_method>
void
L::lecroy_com<coms_method>::set_coupling(const enum location::type& channel, const enum coupling::type& opt)
{
  std::string cmd;
  switch (opt) {
  case coupling::A1M: cmd = location::get_string(channel)+":CPL A1M\n"; break;
  case coupling::D1M: cmd = location::get_string(channel)+":CPL D1M\n"; break;
  case coupling::D50: cmd = location::get_string(channel)+":CPL D50\n"; break;
  case coupling::GND: cmd = location::get_string(channel)+":CPL GND\n"; break;
  default: throw exception::option_not_recognised();
  }
  send(cmd);
}

template<class coms_method> void
L::lecroy_com<coms_method>::set_timebase(const double& timebase)
{
  send("TDIV "+ stringify(timebase)+"\n");
}

template<class coms_method> void
L::lecroy_com<coms_method>::auto_calibrate(const bool& ac_on)
{
  if (ac_on) 
    send("ACAL ON\n");
  else 
    send("ACAL OFF\n");
}


template<class coms_method> void
L::lecroy_com<coms_method>::calibrate()
{
  std::string i = recv("*cal?\n");
  //result should be *CAL 0
  if (atoi(i.substr(5,1).c_str()) != 0) throw exception::calibration_failed_exception();
}

template<class coms_method> void
L::lecroy_com<coms_method>::clear_memory(const location::type& mem)
{
  // if (i<0 || i >4 ) throw exception::invalid_memory_option();
  send("CLM " + location::get_string(mem) +"\n");
}

template<class coms_method> void
L::lecroy_com<coms_method>::set_date()
{
  boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
  std::string day = stringify(t.date().day());
  std::string month = t.date().month().as_short_string();
  std::string year = stringify(t.date().year());
  std::ostringstream msg;
  boost::posix_time::time_facet*const f=
      new boost::posix_time::time_facet("%H,%M,%S\n");
  msg.imbue(std::locale(msg.getloc(),f));
  msg << "DATE "<<day <<","<<month<<","<<year<<","<< t<<"\n";
  
  send(msg.str());
  // wait();
}

template<class coms_method> void
L::lecroy_com<coms_method>::set_vertical_offset(const enum location::type& channel, const double& offset)
{
  std::string cmd;
  if (channel>4 || channel<1)  throw exception::channel_value_not_valid();
  cmd = location::get_string(channel)+":OFST "+stringify(offset)+"V\n";
  send(cmd);
}

template<class coms_method> void
L::lecroy_com<coms_method>::vertical_offset_constant(const bool& off_const)
{
  if (off_const) 
    send("OFCT DIV\n");
  else
    send("OFCT VOLTS\n");
}

template<class coms_method> void 
L::lecroy_com<coms_method>::persistence(const bool& persist)
{
  if (persist) 
    send("PERS ON\n");
  else
    send("PERS OFF\n");
}

template<class coms_method> void 
L::lecroy_com<coms_method>::sequence(const int& segments, const double& max_samples)
{
  std::string cmd;
  if (segments <= 1) 
    cmd = "SEQ OFF\n";
  else{
    cmd = "SEQ ON,"+stringify(segments) + ","+stringify(max_samples) +"\n";
  }
  //  std::cout<<"seq cmd = "<<cmd<<std::endl;
  send(cmd);
  // wait();
  
}

template<class coms_method> void
L::lecroy_com<coms_method>::store(const enum location::type& trace,const enum location::type& dest)
{
  send("STO "+location::get_string(trace) +","+ location::get_string(dest)+"\n");
}
template<class coms_method> void 
L::lecroy_com<coms_method>::store_setup(const enum location::type& trace,const enum location::type& dest, const enum store_mode::type& mode, const enum store_format::type& type
)
{
  send("STST "+location::get_string(trace) + "," 
       + location::get_string(dest)+",AUTO,"
       + store_mode::get_string(mode)+",FORMAT,"
       + store_format::get_string(type)+"\n");
}

template<class coms_method> void L::lecroy_com<coms_method>::toggle_trace(const enum location::type& trace)
{
  std::string ans = recv(location::get_string(trace)+":TRA?\n");
  //returned anwer is, for example "C3:TRA ON"
  if (ans.substr(7,2) =="ON")
    send(location::get_string(trace)+":TRA OFF\n");
  else
    send(location::get_string(trace)+":TRA ON\n");
}

template<class coms_method> void L::lecroy_com<coms_method>::trigger_coupling(const enum location::type& channel, const enum coupling::type& coupling)
{
  std::string str_coupling;
  switch (coupling) {
  case coupling::DC:  str_coupling = "DC"; break;
  case coupling::AC:  str_coupling = "AC"; break;
  case coupling::A1M: str_coupling = "AC1M"; break;
  case coupling::D1M: str_coupling = "DC1M"; break;
  case coupling::D50: str_coupling = "DC50"; break;
  default: throw exception::coupling_option_not_recognised();
  }
  send(location::get_string(channel)+":TRCP " + str_coupling +"\n");
}

template<class coms_method> void L::lecroy_com<coms_method>::trigger_delay(const double& delay)
{
  send("TRDL "+stringify(delay)+"\n");
}

template<class coms_method> void L::lecroy_com<coms_method>::trigger_level(const enum location::type& channel, const double& level)
{
  send(location::get_string(channel)+":TRLV "+stringify(level)+"\n");
}
template<class coms_method> void  L::lecroy_com<coms_method>::trigger_select(const trigger_type::type& type,const enum location::type& source,const enum trigger_hold_type::type& hold_type, const double& hold_value1, const double& hold_value2)
{
  std::string cmd;
  cmd = "TRSE " + trigger_type::get_string(type) 
    + ",SR," + location::get_string(source) 
    + ",HT," + trigger_hold_type::get_string(hold_type) 
    + ",HV," + stringify(hold_value1);
  //std::cout<<"cmd = "<<cmd<<std::endl;

  if (( hold_type == trigger_hold_type::PULSE_WIDTH) ||  (hold_type ==trigger_hold_type::INTERVAL_WIDTH) ) 
    cmd += "," +stringify(hold_value2);
  cmd +="\n";
  
  //std::cout<<"cmd 2= "<<cmd<<std::endl;
  send(cmd);
}

template<class coms_method>
void
L::lecroy_com<coms_method>::demand_fresh_aquisition()
{
  send("CHDR OFF\n");
  int INR_bit = 0;
  //wait for new waveform
  int count = 0;
  int count_max = 5;
  std::cout<<"demand fresh"<<std::endl;

  while (INR_bit == 0 && count <count_max){
    
    std::string ib= recv("INR?\n");
    std::bitset<16> inr(atoi(ib.c_str()));
    for(size_t i=0;i<16;++i){
      std::cout<<"inr["<<i<<"]= "<<inr[i]<<std::endl;
    }


    INR_bit = inr.test(0);
    std::cout<<"INR_bit = "<<INR_bit<<" count = "<<count<<std::endl;
    boost::this_thread::sleep(boost::posix_time::milliseconds(200)); 

    ++count;
  }
  
  if (count>=count_max)
    throw exception::could_not_get_fresh_aquisition();
  

}


template<class coms_method> 
L::lecroy_file
L::lecroy_com<coms_method>::get_waveform(const enum location::type& channel)
{

  
  lecroy_com_manager<coms_method>::wait(5);
  // demand_fresh_aquisition();
  
  // boost::this_thread::sleep(boost::posix_time::milliseconds(200)); 
  //stop the scope from aquiring
  // stop();
  try{
  std::string data = recv(location::get_string(channel)+":WF?\n");
  //std::cout<<"data = "<<data.size()<<std::endl;

  lecroy_file file;
  file.decode_string(data);
  return file;
  }
  catch(ICR::exception::timeout_exceeded& e){
    e.debug_print();
    throw;
  }
  // std::string times = recv(location::get_string(channel)+":WF? TIME\n");
  // header=header.substr(0, header.size()-1);
  // times= times.substr(16,times.size()-17);

  // std::cout<<"header size ="<<header.size()<<std::endl;

  // std::cout<<"times size ="<<times.size()<<std::endl;



  // header.append(times);

  // lecroy_header lh;
  // lh.decode_string(header);
  // while(lh.descriptor_name() != "WAVEDESC") {
  //   clear(); //wait for everything to sort out.
  //   recv_surp();  //clear up the surpless bytes
  //   std::cout<<"descriptor = "<<lh.descriptor_name()<<std::endl;
  //   header = recv(location::get_string(channel)+":WF?\n");

  //   std::string times = recv(location::get_string(channel)+":WF? TIME\n");
  //   header=header.substr(0, header.size()-1);
  //   times= times.substr(16,times.size()-17);


  //   header.append(times);

  //   lh.decode_string(header);
  // }
  
    
  

  // lecroy_file file(lh);
  

  // const unsigned long array_size1 = lh.wave_array_1();
  // const unsigned long array_size2 = lh.wave_array_2();

  // if (array_size1>0){ 
  //   std::string buffer1 = 
  //     recv(location::get_string(channel)+":WF? DAT1\n",array_size1,true);
  //   buffer1 = buffer1.substr(17,buffer1.size()-17);
      
  //   const double vg = lh.vertical_gain();
  //   const double vo = lh.vertical_offset();
  //   boost::scoped_array<double> data1(new double[array_size1]);
  //   //   std::cout<<"byte array1 = "<<lh.byte.array_1<<std::endl;

  //   for(size_t i=0;i<array_size1;++i){
  //     data1[i] =  ((double) buffer1[i])*vg+vo; //lh.byte.array_1+
  //   }
  //   file.set_data1(data1.get());
  // }
  
  // if (array_size2>0){ 
  //   std::string buffer2 = 
  //     recv(location::get_string(channel)+":WF? DAT2\n",array_size2,true);
  //   buffer2 = buffer2.substr(27,buffer2.size()-27);
      
  //   const double vg = lh.vertical_gain();
  //   const double vo = lh.vertical_offset();
  //   boost::scoped_array<double> data2(new double[array_size2]);
  //   //   std::cout<<"byte array2 = "<<lh.byte.array_2<<std::endl;

  //   for(size_t i=0;i<array_size2;++i){
  //     data2[i] =  ((double) buffer2[i])*vg+vo; //lh.byte.array_2+
  //   }
  //   file.set_data2(data2.get());
  // }
  // //check_everything_is_finished();
  // trigger_mode(trigger_mode::SINGLE);


  // wait();
  // int ready_bit = 0;
  // //wait for new waveform
  // count = 0;
  // while (ready_bit == 0 && count <2){
  //   std::string ib= recv("INR?\n");
  //   std::bitset<16> inr(atoi(ib.c_str()));
  //   for(size_t i=0;i<16;++i){
  //     std::cout<<"inr ("<<i<<") = "<<inr[i]<<std::endl;

  //   }

  //   ready_bit = inr.test(13);
  //   ++count;
  // }

}
template<class coms_method> 
std::string
L::lecroy_com<coms_method>::get_waveform_text(const enum location::type& channel)
{
  return recv(location::get_string(channel)+":WF? TEXT\n",160);

  //std::cout<<waveform<<std::endl;


}

template<class coms_method> 
void 
L::lecroy_com<coms_method>::waveform_text(const enum location::type& channel, const std::string& text) 
  throw(exception::waveform_description_too_long)
{
  if (text.size()>160)  throw exception::waveform_description_too_long();

  send(location::get_string(channel)+":WFTX '"+text+"'\n");
}

template<class coms_method> 
void 
L::lecroy_com<coms_method>::display(const bool& do_display)
{
  std::string cmd;
  if (do_display) 
    cmd = "XYDS ON\n";
  else 
    cmd = "XYDS OFF\n";
  send(cmd);
}

template class L::lecroy_com<ICR::lecroy::IP>;
template class L::lecroy_com<ICR::lecroy::SERIAL>;

#include "agilent/agilent_com.hpp"
#include <iostream>
#include "stringify.hpp"
#include <boost/thread.hpp>

#define BUF_LEN 100000
namespace WG = ICR::agilent;

template<class com_method> void
WG::agilent_com<com_method>::load(const load::type option)
{
  std::string cmd;
  switch (option) {
  case WG::load::OHM50: cmd = "OUTP:LOAD 50\n";  break;
  case WG::load::INFTY: cmd = "OUTP:LOAD INF\n"; break;
  default: throw ICR::exception::agilent_option_not_recognised();
  }
  send(cmd);
}

template<class com_method> void
WG::agilent_com<com_method>::turn_on()
{
  std::string cmd = "OUTP ON\n";
  send(cmd);
     
  boost::this_thread::sleep(boost::posix_time::milliseconds(500)); 
}

template<class com_method> void
WG::agilent_com<com_method>::turn_off()
{
  std::string cmd = "OUTP OFF\n";
  send(cmd);
     
  boost::this_thread::sleep(boost::posix_time::milliseconds(500)); 
}

template<class com_method> void
WG::agilent_com<com_method>::frequency(const double f)
{
  std::string cmd = "FREQ "+stringify(f)+"\n";
  send(cmd);
     
  boost::this_thread::sleep(boost::posix_time::milliseconds(100)); 
}


template<class com_method> void
WG::agilent_com<com_method>::voltage(const double v)
{
  std::string cmd = "VOLT "+stringify(v)+"\n";
  send(cmd);
  boost::this_thread::sleep(boost::posix_time::milliseconds(500)); 
}

template<class com_method> void
WG::agilent_com<com_method>::offset(const double o)
{
  std::string cmd = "VOLT:OFFS "+stringify(o)+"\n";
  std::cout<<cmd<<std::endl;

  send(cmd);
  boost::this_thread::sleep(boost::posix_time::milliseconds(500)); 
}

template<class com_method> void
WG::agilent_com<com_method>::duty_cycle(const double d)
{
  std::string cmd = "PULS:DCYC "+ stringify(d)+"\n";

  send(cmd);
  boost::this_thread::sleep(boost::posix_time::milliseconds(500)); 
}


template<class com_method> void
WG::agilent_com<com_method>::cycles(const unsigned int cycles)
{
  // std::string state = recv("BM:STAT?\n"); //need to turn off if already on
  // if (state == "1") burst_off();
  // else
  //   std::cout<<"state = "<<state<<std::endl;

  std::string cmd = "BM:NCYC "+ stringify(cycles)+"\n";
  send(cmd); 
  boost::this_thread::sleep(boost::posix_time::milliseconds(500)); 
  // // state = recv("BM:STAT?\n"); //need to turn off if already on
  // //   std::cout<<"state = "<<state<<std::endl;
  // send("BM:STAT ON\n");
}


template<class com_method> void
WG::agilent_com<com_method>::burst_on()
{
  send("BM:STAT ON\n");
  m_burst_mode = true;
}

template<class com_method> void
WG::agilent_com<com_method>::burst_off()
{
  send("BM:STAT OFF\n");
  m_burst_mode = false;
}


template<class com_method> void
WG::agilent_com<com_method>::phase(const double degrees)
{
  std::string cmd = "BM:PHAS "+ stringify(degrees)+"\n";
  send(cmd);
  boost::this_thread::sleep(boost::posix_time::milliseconds(500)); 
}

template<class com_method> void 
WG::agilent_com<com_method>::burst_int(const double rate)
{
  
  std::string cmd = "BM:INT:RATE "+ stringify(rate)+"\n";
  send(cmd);
}

template<class com_method> void 
WG::agilent_com<com_method>::burst_ext()
{
  std::string cmd = "BM:SOUR EXT\n";
  send(cmd);
}

template<class com_method> void 
WG::agilent_com<com_method>::burst_ext(const enum edge::type& edge)
{
  send("TRIG:SOUR EXT\n");
  std::string cmd; 
  
  switch(edge){
  case WG::edge::POSITIVE: cmd= "TRIG:SLOP POS\n"; break;
  case WG::edge::NEGATIVE: cmd= "TRIG:SLOP NEG\n"; break;
  }
  send(cmd);
  ICR::coms::sleep(300);
}

template<class com_method> void
WG::agilent_com<com_method>::shape (const shape::type option)
{

  std::string cmd;
  switch (option) {
  case WG::shape::SIN:    cmd = "FUNC:SHAP SIN\n"; break;
  case WG::shape::SQUARE: cmd = "FUNC:SHAP SQU\n"; break;
  case WG::shape::PULSE:  cmd = "FUNC:SHAP PULS\n"; break;
  case WG::shape::TRIANGLE: cmd = "FUNC:SHAP TRI\n"; break;
  case WG::shape::RAMP:    cmd = "FUNC:SHAP RAMP\n"; break;
  case WG::shape::NOISE: cmd = "FUNC:SHAP NOIS\n"; break;
  case WG::shape::DC: cmd = "FUNC:SHAP DC\n"; break;
  case WG::shape::USER: cmd = "FUNC:SHAP USER\n"; break;
  default: throw ICR::exception::agilent_option_not_recognised();
  }
  send(cmd);
  boost::this_thread::sleep(boost::posix_time::milliseconds(500)); 
}


template<class com_method> void 
WG::agilent_com<com_method>::apply(const shape::type shape, const double freq,const  double volts,const  double offset)
{
  std::string cmd;
  switch (shape) {
  case WG::shape::SIN:    cmd = "APPL:SIN "; break;
  case WG::shape::SQUARE: cmd = "APPL:SQU "; break;
  case WG::shape::PULSE:  cmd = "APPL:PULS "; break;
  case WG::shape::TRIANGLE: cmd = "APPL:TRI "; break;
  case WG::shape::RAMP:    cmd = "APPL:RAMP "; break;
  case WG::shape::NOISE: cmd = "APPL:NOIS "; break;
  case WG::shape::DC: cmd = "APPL:DC "; break;
  case WG::shape::USER: cmd = "APPL:USER "; break;
  default: throw ICR::exception::agilent_option_not_recognised();
  }
  cmd += stringify(freq)+", "+ stringify(volts) + ", " + stringify(offset) +"\n";
  send(cmd);
  boost::this_thread::sleep(boost::posix_time::milliseconds(500)); 
}

template<class com_method> void 
WG::agilent_com<com_method>::trigger(const trigger::type option)
{
  std::string cmd;
  switch (option) {
  case WG::trigger::IMMEDIATE: cmd = "TRIG:SOUR IMM\n"; break;
  case WG::trigger::EXTERNAL:  cmd = "TRIG:SOUR EXT\n"; break;
  case WG::trigger::BUS:       cmd = "TRIG:SOUR BUS\n"; break;
  default: throw ICR::exception::agilent_option_not_recognised();
  }
  send(cmd);
}

template<class com_method> 
void 
WG::agilent_com<com_method>::trigger_now() 
{
  send("*TRG\n");
  //  boost::this_thread::sleep(boost::posix_time::milliseconds(200)); 
}

template<class com_method>
std::string
WG::agilent_com<com_method>::error()
{
  std::string err = recv("SYST:ERR?\n");
  return err;

}

template<class com_method> void 
WG::agilent_com<com_method>::reset()
{
  send("*RST;*CLS\n");  //reset and clear error buffer
  // std::string i = recv("*OPC?\n"); //wait for errors to be cleared
  // if (atoi(i.c_str())!= 1) throw ICR::exception::failed_to_reset();
}

template<class com_method> void
WG::agilent_com<com_method>::arb_wave(const std::string& name, const float* data, const unsigned long& size)
{
  //std::cout<<"arb wave"<<std::endl;
  std::ostringstream str;
  str<<"DATA VOLATILE, ";
  for(size_t i=0;i<size-1;++i){
    str<< data[i]<<", ";
  }
  str<<data[size-1]<<"\n";
  send(str.str());
  send("DATA:COPY " +name +", VOLATILE\n");
  send("FUNC:USER "+name+"\n");
  send("FUNC:SHAP USER\n");


}

template<class com_method>
void 
WG::apply_waveform(agilent_com<com_method>* generator,const std::string& name, const  double& sampling_freq, const  float* voltages, const unsigned long& num_points)
{
  if (num_points<8) throw ICR::exception::too_few_points();
  if (num_points>16000) throw ICR::exception::too_many_points();

  float min;
  float max;
  //find the min and the max
  for(unsigned long i=0;i<num_points;++i){
    float curr = voltages[i];
    if (curr<min) min= curr;
    if (curr>max) max= curr;
  }
  float irange = 1.0/(max-min);
  //rescale to +- 1
  float* rsv = new float[num_points];
  for(unsigned long i=0;i<num_points;++i){
    rsv[i] = ((voltages[i])*irange);
  }
  
  
  
  generator->arb_wave(name, rsv, num_points);
  generator->voltage((max-min));
  generator->frequency((sampling_freq/num_points));
}


template void WG::apply_waveform<WG::IP>(agilent_com<WG::IP>* generator,const std::string& name, const double& sampling_freq,const  float* voltages, const unsigned long& num_points);
template void WG::apply_waveform<WG::SERIAL>(agilent_com<WG::SERIAL>* generator, const std::string& name,const double& sampling_freq, const  float* voltages, const unsigned long& num_points);

template class WG::agilent_com<WG::IP>;
template class WG::agilent_com<WG::SERIAL>;
// template<class com_method> void 
// WG::agilent_com<com_method>::arb_wave(const boost::array data)
// {
//   /* Download the waveform array to volatile memory.
//      The function generator expects to receive the arb waveform data as
//      one contiguous block. To do this, suppress the carriage return (CR)
//      and line feed (LF) before downloading the data. */
  
//   // send("DATA VOLATILE, ");
// }


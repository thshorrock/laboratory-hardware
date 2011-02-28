#include "DPR500/pulser_receivers.hpp"

boost::shared_ptr<ICR::pulser::RPL2> ICR::pulser::RPL2::m_instance; // = 0;

boost::shared_ptr<ICR::pulser::RPU2> ICR::pulser::RPU2::m_instance; // = 0;

ICR::pulser::RPL2::RPL2()
  : pulser_receiver()
{

  //receiver
  // m_gain = m_rf.gain(7, -13,66,1);
  // m_hpf  = m_rf.high_pass_filter<filter::HPF1>();
  // m_lpf  = m_rf.low_pass_filter<filter::LPF1>();
	
  m_pulser_mode = m_pf.pulser_mode<damping_RPL2>();
  m_master      = m_pf.master(true);
  // m_on = m_pf.turn_on();
  // m_off = m_pf.turn_off();
  m_int_trig    = m_pf.internal_trigger();
  m_ext_trig    = m_pf.external_trigger();
  m_prf         = m_pf.prf(5000);

  m_voltage  = m_pf.voltage(275,500);
  m_voltage->set(300);
	
  // m_internal_trig = m_pf.internal_trigger();
  // m_external_trig = m_pf.external_trigger();
	
  m_pulser_mode->set(damping_RPL2::fourtyfour_Ohm,
		    energy_policy::LOW,
		    receive_mode::PULSE_ECHO);
  
  
  // pulser_receiver::attach_cmd(m_gain);
  // pulser_receiver::attach_cmd(m_hpf);
  // pulser_receiver::attach_cmd(m_lpf);
  
  pulser_receiver::attach_cmd(m_pulser_mode);
  pulser_receiver::attach_cmd(m_master);
  pulser_receiver::attach_cmd(m_ext_trig );
  pulser_receiver::attach_cmd(m_prf);
  pulser_receiver::attach_cmd(m_voltage);
  

  // attach_cmd(m_hpf);
  // attach_cmd(m_lpf);
  // attach_cmd(m_master);
  // attach_cmd(m_prf);
  // attach_cmd(m_voltage);
  // attach_cmd(m_external_trig);
	
}

void
ICR::pulser::RPL2::set_internal_trigger()
{
  detach_cmd(m_ext_trig); 
  attach_cmd(m_int_trig);
}


void
ICR::pulser::RPL2::set_pulser_mode(const enum damping_RPL2::type& dp, 
				   const enum energy_policy::type& ep,
				   const enum receive_mode::type& rp)
{
  m_pulser_mode->set(dp, ep, rp);
}


void
ICR::pulser::RPL2::set_external_trigger()
{
  detach_cmd(m_int_trig); 
  attach_cmd(m_ext_trig);
}

// ICR::pulser::RPU2::RPU2()
//   : pulser_receiver()
// {
//   m_gain = m_rf.gain(7, -13,66,1);
//   // m_hpf  = rf.hpf<command::>();
//   // m_lpf  = rf.lpf<command::>();
	
//   // m_master = pf.master(true);
//   // m_prf    = pf.prf();
//   // m_voltage = pf.voltage(min,max);
	
//   // m_internal_trig = m_pf.internal_trigger();
//   // m_external_trig = m_pf.external_trigger();
	
//   pulser_receiver::attach_cmd(m_gain);
//   // attach_cmd(m_hpf);
//   // attach_cmd(m_lpf);
//   // attach_cmd(m_master);
//   // attach_cmd(m_prf);
//   // attach_cmd(m_voltage);
//   // attach_cmd(m_external_trig);
	
// }


ICR::pulser::RPL2*
ICR::pulser::RPL2::Instance()
{
  if (m_instance == 0) 
    m_instance =  boost::shared_ptr<RPL2>(new RPL2());
  return m_instance.get();
}





ICR::pulser::RPU2::RPU2()
  : pulser_receiver()
{

  //receiver
  // m_gain = m_rf.gain(7, -13,66,1);
  // m_hpf  = m_rf.high_pass_filter<filter::HPF1>();
  // m_lpf  = m_rf.low_pass_filter<filter::LPF1>();
	
  
  m_pulser_mode = m_pf.pulser_mode<damping_RPU2>();
  m_master      = m_pf.master(true);
  // m_on = m_pf.turn_on();
  // m_off = m_pf.turn_off();
  m_int_trig    = m_pf.internal_trigger();
  m_ext_trig    = m_pf.external_trigger();
  m_prf         = m_pf.prf(20000);

  m_voltage  = m_pf.voltage(140,145);
	
  // m_internal_trig = m_pf.internal_trigger();
  // m_external_trig = m_pf.external_trigger();
	
  m_pulser_mode->set(damping_RPU2::fifty_Ohm,
		    energy_policy::LOW,
		    receive_mode::PULSE_ECHO);
  
  m_voltage->set(143);
  
  // pulser_receiver::attach_cmd(m_gain);
  // pulser_receiver::attach_cmd(m_hpf);
  // pulser_receiver::attach_cmd(m_lpf);
  
  
  boost::shared_ptr<command::query_voltage> m_query_voltage 
    = boost::shared_ptr<command::query_voltage> (new command::query_voltage());
      


  //pulser_receiver::attach_cmd(m_query_voltage);


  pulser_receiver::attach_cmd(m_pulser_mode);
  pulser_receiver::attach_cmd(m_master);
  pulser_receiver::attach_cmd(m_ext_trig );
  pulser_receiver::attach_cmd(m_prf);
  pulser_receiver::attach_cmd(m_voltage);
  

  // attach_cmd(m_hpf);
  // attach_cmd(m_lpf);
  // attach_cmd(m_master);
  // attach_cmd(m_prf);
  // attach_cmd(m_voltage);
  // attach_cmd(m_external_trig);
	
}

void
ICR::pulser::RPU2::set_internal_trigger()
{
  detach_cmd(m_ext_trig); 
  attach_cmd(m_int_trig);
}


void
ICR::pulser::RPU2::set_pulser_mode(const enum damping_RPU2::type& dp, 
				   const enum energy_policy::type& ep)
{
  m_pulser_mode->set(dp, ep, receive_mode::PULSE_ECHO );
}


void
ICR::pulser::RPU2::set_external_trigger()
{
  detach_cmd(m_int_trig); 
  attach_cmd(m_ext_trig);
}

// ICR::pulser::RPU2::RPU2()
//   : pulser_receiver()
// {
//   m_gain = m_rf.gain(7, -13,66,1);
//   // m_hpf  = rf.hpf<command::>();
//   // m_lpf  = rf.lpf<command::>();
	
//   // m_master = pf.master(true);
//   // m_prf    = pf.prf();
//   // m_voltage = pf.voltage(min,max);
	
//   // m_internal_trig = m_pf.internal_trigger();
//   // m_external_trig = m_pf.external_trigger();
	
//   pulser_receiver::attach_cmd(m_gain);
//   // attach_cmd(m_hpf);
//   // attach_cmd(m_lpf);
//   // attach_cmd(m_master);
//   // attach_cmd(m_prf);
//   // attach_cmd(m_voltage);
//   // attach_cmd(m_external_trig);
	
// }


ICR::pulser::RPU2*
ICR::pulser::RPU2::Instance()
{
  if (m_instance == 0) 
    m_instance =  boost::shared_ptr<RPU2>(new RPU2());
  return m_instance.get();
}


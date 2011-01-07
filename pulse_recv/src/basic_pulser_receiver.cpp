#include "DPR500/basic_pulser_receiver.hpp"
ICR::pulser::pulser_receiver::pulser_receiver()
  : 
    m_cmds(),
    m_channel(channel::A),
    attached_to_DPR500(false)
{}

void
ICR::pulser::pulser_receiver::attach_cmd(cmd_ptr c)
{
  c->set_pulser_receiver(this);
  m_cmds.push_back(c);
  m_cmds.unique(); //remove duplicates
};

void
ICR::pulser::pulser_receiver::detach_cmd(cmd_ptr c)
{
  m_cmds.remove(c);
};

void ICR::pulser::pulser_receiver::set_state(const enum channel::type& channel, const char& address)
{
  m_channel = channel;
  m_address = address;
  attached_to_DPR500 = true;
  notify(); //update the channel on all the attached commands.
}

void ICR::pulser::pulser_receiver::detach()
{
  attached_to_DPR500 = false;
}

char
ICR::pulser::pulser_receiver::get_channel_char() const
{ 
  if ( !attached_to_DPR500 ) throw ICR::exception::pulser_receiver_not_attached_to_DPR500();
  if ( m_channel == channel::A) 
    return 0x41;
  else
    return 0x42;
};

char
ICR::pulser::pulser_receiver::get_address_char() const
{ 
  if ( !attached_to_DPR500 ) throw ICR::exception::pulser_receiver_not_attached_to_DPR500();
  return m_address;
};

void
ICR::pulser::pulser_receiver::notify()
{
  for_each(m_cmds.begin(), m_cmds.end(), command::update_cmd);
};

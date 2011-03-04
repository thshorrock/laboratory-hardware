#pragma once
#include "agilent_com.hpp"

namespace ICR {
  namespace agilent{
    class  WG33120A : public agilent_com<ICR::coms::serial_manager>
    {
    public:
      WG33120A(const std::string& device) : agilent_com<ICR::coms::serial_manager>(device) 
      {
      };
      using agilent_com<ICR::coms::serial_manager>::send;
      using agilent_com<ICR::coms::serial_manager>::recv;
      using agilent_com<ICR::coms::serial_manager>::apply;
      using agilent_com<ICR::coms::serial_manager>::load;
      using agilent_com<ICR::coms::serial_manager>::duty_cycle;
      using agilent_com<ICR::coms::serial_manager>::frequency;
      using agilent_com<ICR::coms::serial_manager>::voltage;
      using agilent_com<ICR::coms::serial_manager>::offset;
      using agilent_com<ICR::coms::serial_manager>::cycles;
      using agilent_com<ICR::coms::serial_manager>::burst_on;
      using agilent_com<ICR::coms::serial_manager>::burst_off;
      using agilent_com<ICR::coms::serial_manager>::phase;
      using agilent_com<ICR::coms::serial_manager>::burst_int;
      using agilent_com<ICR::coms::serial_manager>::burst_ext;
      using agilent_com<ICR::coms::serial_manager>::trigger;
      using agilent_com<ICR::coms::serial_manager>::trigger_now;
      using agilent_com<ICR::coms::serial_manager>::shape;
      using agilent_com<ICR::coms::serial_manager>::arb_wave;
      using agilent_com<ICR::coms::serial_manager>::reset;
      using agilent_com<ICR::coms::serial_manager>::error;
    };

  }
}

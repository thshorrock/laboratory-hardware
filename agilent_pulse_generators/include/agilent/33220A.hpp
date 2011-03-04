#pragma once

#include "agilent_com.hpp"

namespace ICR {
  namespace agilent{
    template<class com_method>
    class  WG33220A : public agilent_com<com_method>
    {
    public:
      WG33220A(const std::string& device) : agilent_com<com_method>(device) {};
      using agilent_com<com_method>::send;
      using agilent_com<com_method>::recv;
      using agilent_com<com_method>::apply;
      using agilent_com<com_method>::load;
      using agilent_com<com_method>::duty_cycle;
      using agilent_com<com_method>::turn_on;
      using agilent_com<com_method>::turn_off;
      using agilent_com<com_method>::frequency;
      using agilent_com<com_method>::voltage;
      using agilent_com<com_method>::offset;
      using agilent_com<com_method>::cycles;
      using agilent_com<com_method>::burst_on;
      using agilent_com<com_method>::burst_off;
      using agilent_com<com_method>::phase;
      using agilent_com<com_method>::burst_int;
      using agilent_com<com_method>::burst_ext;
      using agilent_com<com_method>::trigger;
      using agilent_com<com_method>::trigger_now;
      using agilent_com<com_method>::shape;
      using agilent_com<com_method>::arb_wave;
      using agilent_com<com_method>::reset;
      using agilent_com<com_method>::error;
    };
  }
}


#pragma once

namespace ICR{

  namespace pulser{

    struct channel{
      enum type {A, B};
    };
    
    struct damping_RPL2{
      enum type {threehundredthirty_Ohm,onehundredfour_Ohm,fourtyfour_Ohm,thirtyfour_Ohm};
    };

    struct damping_RPU2{
      enum type {onehundred_Ohm,fifty_Ohm,twentyone_Ohm,seventeen_Ohm};
    };
    
    struct energy_policy{
      enum type{LOW, HIGH};
    };
    struct receive_mode{
      enum type{PULSE_ECHO, RECEIVE_ONLY};
    };
      
  }
}

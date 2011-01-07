
#pragma once

namespace ICR{

  namespace pulser{

    /** The channels */
    struct channel{
      /** The valid options */
      enum type {A, //!< Channel A
		 B  //!< Channel B
      };
    };
    
    /** Damping policy for the RPL2 */
    struct damping_RPL2{
      /** The valid options */
      enum type {threehundredthirty_Ohm, //!< 330 Ohm
		 onehundredfour_Ohm,     //!< 104 Ohm
		 fourtyfour_Ohm,         //!< 44 Ohm
		 thirtyfour_Ohm          //!< 34 Ohm
      };
    };

    /** Damping policy for the RPU2 */
    struct damping_RPU2{
      /** The valid options */
      enum type {onehundred_Ohm,    //!< 100 Ohm
		 fifty_Ohm,         //!< 50 Ohm 
		 twentyone_Ohm,     //!< 21 Ohm
		 seventeen_Ohm      //!< 17 Ohm
      };
    };
    
    /** Energy policy */
    struct energy_policy{
      /** The valid options */
      enum type{LOW,  //!< Low energy (not sure what this means...)
		HIGH  //!< Low energy (not sure what this means...)
      };
    };

    /** Pulse receive policy */
    struct receive_mode{
      /** The valid options */
      enum type{PULSE_ECHO,  //!< Pulse echo mode
		RECEIVE_ONLY //!< Receive goes to through terminal (without the pulse)
      };
    };
      
  }
}

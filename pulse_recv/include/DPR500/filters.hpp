#pragma once


/** The Institute of Cancer Research Namespace */
namespace ICR{

  /** The pulse-receiver Namespace */
  namespace pulser{
    /** The filter Namespace */
    namespace filter{
      /** High pass filter of channel A */
      class HPFA{
      public:
	/** The valid options */
	enum type {ten_kHz, //!< 10 kHz
		   one_MHz, //!< 1 MHz
		   twopointfive_MHz,  //!< 2.5 MHz
		   five_MHz, //!< 5 MHz
		   sevenpointfive_MHz //!< 7.5MHz
	};
      };

      /** Low pass filter of channel A */
      class LPFA{
      public:
	/** The valid options */
      	enum type {three_MHz,  //!< 3 MHz
		   sevenpointfive_MHz, //!<7.5 MHz
		   ten_MHz, //!< 10 MHz
		   fifteen_MHz, //!< 15 MHz 
		   twentytwopointfive_MHz, //!< 22.5 MHz
		   fifty_MHz //!< 50 MHz
	};
      };
      /** High pass filter of channel B */
      class HPFB{
      public:
	/** The valid options */
	enum type {   five_MHz,  //!< 5 MHz
		      thirty_MHz //!< 30 MHz
	};
      };

      /** Low pass filter of channel B */
      class LPFB{
      public:
	/** The valid options */
      	enum type {fivehundred_MHz, //!< 500 MHz
		   threehundred_MHz //!< 300 MHz
	};
      };
    }
  }
}

#pragma once


/** The Institute of Cancer Research Namespace */
namespace ICR{

  /** The pulse-receiver Namespace */
  namespace pulser{
    /** The filter Namespace */
    namespace filter{
      class HPFA{
      public:
	enum type {ten_kHz, one_MHz, twopointfive_MHz, five_MHz, sevenpointfive_MHz};
      };

      class LPFA{
      public:
      	enum type {three_MHz,sevenpointfive_MHz, ten_MHz, fifteen_MHz, twentytwopointfive_MHz, fifty_MHz};
      };
      class HPFB{
      public:
	enum type {   five_MHz,thirty_MHz};
      };

      class LPFB{
      public:
      	enum type {fivehundred_MHz,threehundred_MHz};
      };
    }
  }
}

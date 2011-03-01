#pragma once
/** @defgroup Options Options
*/


/** ICR namespace */
namespace ICR {
  /** analogic  namespace */
  namespace analogic {

    
    /** The allowed trigger modes.
     *  @ingroup Options
     */
    struct trigmode
    {
      /** An enumeration of the trigger modes.
       *  @attention Modes 9 and 10 (START AND STOP) only work with GET (group execute trigger) response (GPIB)
       *  
       */
      enum type 
	{
	  FREE_RUN=0, //!< Output starts when computation is finsiheed, free of any trigger requirement
	  POS_START=1, //!< Output starts when next positive trigger edge occurs
	  NEG_START=2, //!< Output starts when next negative trigger edge occurs
	  POS_GATED=3, //!< Output starts with positive trigger edges, stops with negative edges and starts again with positive.
	  NEG_GATED=4, //!< Output starts with negative trigger edges, stops with positive edges and starts again with negative edges
	  POS_STOP=5, //!< Output stops when next positive trigger edge occurs
	  NEG_STOP=6, //!< Output stops when next negative edge occurs.
	  POS_TO_POS=7, //!< Output toggles from start to stop or vice-versa whenever a positive trigger edge occurs,
	  NEG_TO_NEG=8, //!< Output toggles from start to stop or vice-versa whenever a negative trigger edge occurs,
	  START=9, //!< A GET will start output
	  STOP=10, //!< A GET will stop output.
	};
    };
  }
}

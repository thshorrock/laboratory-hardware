
/**
 * @file   lecroy_com.hpp
 * @author Tom Shorrock (t.h.shorrock@gmail.com)
 * @date   December, 2010
 * @brief  Interface to communicate with the lecroy waverunner scope
 *
 * Example usage:
 *
 * @code
 * #include "lecroy/lecroy_com.hpp"
 * using namespace ICR::lecroy;
 * lecroy_com lc("ip_address");   //where ip_address is the IP address of the scope, 
                                  //for example 193.62.12.322 
				  * lc.set_coupling(ICR::lecroy::location::C1, ICR::lecroy::coupling::D50); //set C1 to 50 ohm DC coupling
				  * @endcode
				  */


#include "lecroy_com_manager.hpp"
#include "lecroy_file.hpp"
#include "stringify.hpp"


/** Institute of Cancer Research Namespace. */
namespace ICR{

  namespace exception{ 
    /** Option not recognised. */
    struct option_not_recognised : public lecroy_exception {};
      
    /** Scope failed to reset. */
    struct failed_to_reset : public lecroy_exception {};
      
    /**Channel value not valid. */
    struct channel_value_not_valid : public lecroy_exception {}; 

    /** store mode not valid. */
    struct store_mode_not_valid : public lecroy_exception {};  
      
    /** store format not valid. */
    struct store_format_not_valid : public lecroy_exception {};
      
    /** coupling option not recognised. */
    struct coupling_option_not_recognised : public lecroy_exception {};  
      
    /**trigger mode not recognised. */
    struct trigger_mode_not_recognised : public lecroy_exception {};

    /**  trigger state not recognised. */
    struct trigger_state_not_recognised : public lecroy_exception {}; 
      
    /**trigger type not recognised. */
    struct trigger_type_not_recognised : public lecroy_exception {};  
      
    /**  trigger hold type not recognised. */
    struct trigger_hold_type_not_recognised : public lecroy_exception {};

    /** Scope calibration failed. */
    struct calibration_failed_exception : public lecroy_exception {}; 

    /** Invalid memory option.*/
    struct invalid_memory_option : public lecroy_exception {};

    /** More than 160 characters in the waveform description */
    struct waveform_description_too_long : public lecroy_exception {};

    /** Could not get fresh aquisition. */
    struct could_not_get_fresh_aquisition : public lecroy_exception {};

    struct lecroy_file_corrupted_on_scope : public lecroy_exception {};
  } 

  /** Lecroy Namespace. */
  namespace lecroy{
    /** Locations of data  
     *  This contains both physical channels such as C1, C2, Ex, Ex10; but also memory channels such as M1, M2, etc. and the hard disk HDD*/
    struct location{
      /** An enumeration that contains channels. */
      enum type{C1, //!< Channel 1
		C2, //!< Channel 2
		C3, //!< Channel 3
		C4, //!< Channel 4
		F1, //!< Function 1
		F2, //!< Function 2
		F3, //!< Function 3
		F4, //!< Function 4
		F5, //!< Function 5
		F6, //!< Function 6
		F7, //!< Function 7
		F8, //!< Function 8
		M1, //!< Memory 1
		M2, //!< Memory 2
		M3, //!< Memory 3
		M4, //!< Memory 4
		TA, //!< Trace A
		TB, //!< Trace B
		TC, //!< Trace C
		TD, //!< Trace D
		ALL_DISPLAYED, //!< All displayed traces
		EX, //!< External
		EX10, //!< Ext 10
		ETM10, //!< Not sure
		FILE,  //!< File
		HDD    //!< Hard disk drive
      };
      /**  Return a string containing the channel.
       * @param code The location code. @return The location string.
       *   For example \code using namespace ICR::lecroy; std::string channel_str = ICR::lecroy::location::get_string(ICR::lecroy::location::C1); \endcode
       */
      static std::string get_string(const enum location::type& code);
    } ;
    /**The storage mode.  */
    struct store_mode{
      /** Enumeration of the storate mode.*/
      enum  type{NO_AUTOSAVE, //!< NO autosave
		 FILL,        //!< Fill
		 WRAP         //!< Wrap
      };
      /** The string variable of the option. @param code @return what the code represents */
      static std::string get_string(const enum store_mode::type& code);
    };

    /** The file format. */
    struct store_format{
      /** The valid options.*/
      enum  type{ASCII,   //!< Ascii format
		 BINARY,  //!< Binary  format
		 EXCEL,   //!< Excel format
		 MATHCAD, //!< Mathcad format
		 MATLAB   //!< Matlab format
      };
      /** The string variable of the option. @param code @return what the code represents */
      static std::string get_string(const enum store_format::type& code);
    };

    /** The couplings */
    struct coupling{
      /** The valid options.*/
      enum  type{DC,  //!< DC coupling
		 AC,  //!< AC coupling
		 A1M, //!< AC coupling (1 MOhm)
		 D1M, //!< DC coupling (1 MOhm)
		 D50, //!< DC coupling (50 Ohm)
		 GND  //!< Ground
      };
      /** The string variable of the option. @param code @return what the code represents */
      static std::string get_string(const enum coupling::type& code);
    };
    /** The trigger mode.*/
    struct trigger_mode{
      /** The valid options.*/
      enum type {AUTO,    //!< Auto aquisition.
		 NORM,    //!< Normal aquisition
		 SINGLE,  //!< Single aquisition
		 STOP     //!< Stop aquisitioning
      };
      /** The string variable of the option. @param code @return what the code represents */
      static std::string get_string(const enum trigger_mode::type& code);
    };
    // namespace trigger_state{
    //   enum {LOW, HIGH};
    //   static std::string get_string(const enum ::type);
    // }
    /** The trigger types */
    struct trigger_type{
      /** The valid options.*/
      enum type {DROPOUT,           //!< Dropout triggering
		 EDGE,              //!< Edge triggering 
		 GLITCH,            //!<Glitch triggering
		 INTERVAL,          //!<Interval triggering
		 STD,               //!<Std tringgering (not sure what this is)
		 SINGLE_SOURCE ,    //!<Single source
		 STATE_QUALIFIED,   //!<State qualified
		 EDGE_QUALIFIED     //!Edge qualified.
      };
      /** The string variable of the option. @param code @return what the code represents */
      static std::string get_string(const enum trigger_type::type& code);
    };
    
    /** The trigger hold options */
    struct trigger_hold_type{
      /** The valid options.*/
      enum type {TIME_GREATER_THAN,       //!< Greater than option
		 TIME_WIDTH,              //!< Time width option
		 EVENTS,                  //!< Number of events option
		 PULSE_SMALLER_THAN,      //!< Pulse smaller than option
		 PULSE_GREATER_THAN,      //!< Pulse greater than option
		 INTERVAL_SMALLER_THAN,   //!< Interval smaller than option
		 INTEVAL_GREATER_THAN,    //!< Inverval greater than option
		 PULSE_WIDTH,             //!< Pulse width option
		 INTERVAL_WIDTH,          //!< Interval width option
		 NO_HOLD                  //!< No hold option 
      };
      /** The string variable of the option. @param code @return what the code represents */
      static std::string get_string(const enum trigger_hold_type::type& code);
    };

    
 
    /** Interface for communicating with the Lecroy Waverunner scope.
     *
     *
     */
    template <class coms_method>
    class lecroy_com : public lecroy_com_manager<coms_method>
    {

    public:
      /** Constructor for interface to Lecroy Waverunner scope.
       *
       * @param IPaddress The IP address of the scope. (via VICP).
       * Example: 
       * @code
       * #include "lecroy/lecroy_com.hpp"
       * using namespace ICR::lecroy;
       * lecroy_com lc("ip_address"); //where ip_address is, for example 193.62.12.322.
       * @endcode
       */
      lecroy_com(const std::string& IPaddress);
      virtual ~lecroy_com(){};
      
      /** Send a command to the scope.
       * All command need to terminated by a new line character "\n".
       *
       * Example: Make the scope beep (emulates beep()).
       * @code
       * lecroy_com lc("ip_address");
       * lc.send("BUZZ BEEP\n");
       * @endcode
       */
      using lecroy_com_manager<coms_method>::send;

      
      /** Receive a command from the scope.
       * All command need to terminated by a new line character "\n".
       *
       * Example: Calibrate the scope (receiving a status signal)
       * @code
       * lecroy_com lc("ip_address");
       * std::string ans = lc.recv("*cal?\n"); //ans = "*CAL 0" if calibration successful.
       * @endcode
       */
      using lecroy_com_manager<coms_method>::recv;
      //       using lecroy_com_manager<coms_method>::timed_recv;
      
      //      using lecroy_com_manager<coms_method>::recv_surp;
      /** Set the coupling for a channel on the scope.
       *  @param channel  Acceptable values are ICR::lecroy::location::C1, ICR::lecroy::location::C2, ICR::lecroy::location::C3 and ICR::lecroy::location::C4.
       *  @param opt  Accepatable values are  
       *  - ICR::lecroy::coupling::A1M: AC 1 MOhm coupling,
       *  - ICR::lecroy::coupling::D1M, DC 1 MOhm coupling,
       *  - ICR::lecroy::coupling::D50, DC 50Ohm coupling,
       *  - ICR::lecroy::coupling::GND, coupled to ground.
       * 
       * Example: Setting channel 1 to 50 ohm coupling. 
       * @code  
       * lecroy_com lc("ip_address");
       * lc.set_coupling(ICR::lecroy::location::C1, ICR::lecroy::coupling::D50);
       * @endcode
       *
       * @todo check for errors in the input and throw appropriately.
       */
      virtual  void
      set_coupling(const enum location::type& channel, const enum coupling::type& opt);

      /** Set the timebase to the scope (seconds per division).
       *  @param timebase The seconds per divisiton
       * Example: Set timebase to be 50us per division
       * @code  
       * lecroy_com lc("ip_address");
       * lc.set_timebase(50e-6);
       * @endcode
       *
       * @see volts_per_div
       * @todo check for timebases that are out of possible range for the scope
       */
      virtual void set_timebase(const double& timebase);  //in seconds
      
      /** Average sweeps.
       * @param trace The trace to save to: for example TA,
       * @param channel The channel from which to collect the averages
       * @param points The maximum number of points to average over.
       * @param sweeps The number of sweeps
       */
      virtual void average(const enum location::type& trace, 
			   const enum location::type& channel,
			   const unsigned long& points,
			   const unsigned int& sweeps);
	  

      /** Arms the scope and forces a single acquisition if it is already armed.
       * Example:
       * @code  
       * lecroy_com lc("ip_address");
       * lc.arm();
       * @endcode
       */
      virtual void arm() {send("ARM\n");}
      
      /** Enable or disable the automatic calibration of the oscilloscope.
       * @param enable_ac true or false.
       *
       * Example:  
       * @code  
       * lecroy_com lc("ip_address");
       * lc.auto_calibrate(true);
       * @endcode
       */
      virtual void auto_calibrate(const bool& enable_ac);
      
      /** Calibrate the oscilloscope now.
       * Example:
       * @code  
       * lecroy_com lc("ip_address");
       * lc.calibrate();
       * @endcode
       */ 
      virtual void calibrate();
      
      /** Make the oscilloscope beep. 
       * Example:
       * @code  
       * lecroy_com lc("ip_address");
       * lc.beep();
       * @endcode
       */
      virtual void beep() {send("BUZZ BEEP\n");}
      
      /** Clear the memory of one of the memory slots.
       * @param mem An integer 1-4 indicating M1, M2, M3, or M4. 
       *
       * Example clear M3:
       * @code  
       * lecroy_com lc("ip_address");
       * lc.clear_memory(3);
       * @endcode
       */
      virtual void clear_memory(const location::type& mem);

      /** Restarts the cumulative processing functions.
       *  For example summed or continuous average, extrema, FFT power
       *  average, histogram, pulse parameter statistics, Pass/Fail counters,
       *  and persistence.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.clear_sweeps();
       * @endcode
       * @bug This fuction does not presently work even though it should from the documentation. Maybe registors in header need to be altered for this function case?
       */
      virtual void clear_sweeps() {send("CLEAR_SWEEPS\n");}

      /** Clear all status data registers.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.clear();
       * @endcode
       * @bug This fuction does not presently work even though it should from the documentation. Maybe registors in header need to be altered for this function case?
       */
      virtual void clear() {send("*CLS\n");}

      /** Copy the local date and time on the computer over to the scope.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.set_date();
       * @endcode
       */
      virtual void set_date();
      
      /** Aquire a single trace.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.aquire_single();
       * @endcode
       */
      virtual void aquire_single() {send("TRMD SINGLE;ARM;FRTR\n");}
      
      /** Print a message on the bottom of the screen.
       * @param message The message to be displayed
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.aquire_single("A message from me");
       * @endcode
       */
      virtual void message(const std::string& message) {send("MSG '"+message+"'\n");}

      /** Set vertical offset (in volts) to a channel.
       * Note this essentially a dc offset, it is not the volts per divisiton. 
       * @see volts_per_div(const enum location::type& channel,const double& v)
       *
       * Example: Set offset channel 3 by 2 volts
       * @param channel  Acceptable values are ICR::lecroy::location::C1, ICR::lecroy::location::C2, ICR::lecroy::location::C3 and ICR::lecroy::location::C4.
       * @param offset Vertical offset in volts.
       * @code  
       * lecroy_com lc("ip_address");
       * lc.set_vertical_offset(ICR::lecroy::location::C3, 2);
       * @endcode
       */
      virtual void set_vertical_offset(const enum location::type& channel, const double& offset);
      
      /**  Specifiy whether the vertical offset is scaled as you change the gain.
       * @param scale_with_gain true or false.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.vertical_offset_constant(true);
       * @endcode
       */
      virtual void vertical_offset_constant(const bool& scale_with_gain);

      /** Turn persisence on or off.
       * @param persistence_on true or false.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.peristence(true);
       * @endcode
       */
      virtual void persistence(const bool& persistence_on);

      /** Reset the scope
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.reset();
       * @endcode
       */
      virtual void reset(){send("*RST\n");}

      /** Turn on/off sequence mode.
       * @param segments The segments to the sequence. 
       * A value of less than or equal to 1 turns off sequence mode.  
       * Otherwise the stated number of sequences is set.
       * @param max_samples Request the maximum number of samples per segemnt.
       * Note that the scope will adjust this to the closest allowed value.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.sequence(10, 1e6);
       * @endcode
       */
      virtual void sequence(const int& segments, const double& max_samples);


      /** Stop aquiring waveforms.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.stop();
       * @endcode
       */
      virtual void stop() {send("STOP\n");}

      /** Store the contents of the specified trace in
       * one of the internal memories M1 to M4 or in the current directory
       * in mass storage.
       * @param trace Acceptable values are ICR::lecroy::location::C1,..., ICR::lecroy::location::C4, ICR::lecroy::location::F1, ... ICR::lecroy::location::F9, ICR::lecroy::location::ALL_DISPLAYED
       * @param dest Destination.  Acceptable values are 
       * - ICR::lecroy::location::M1, ..., ICR::lecroy::location::M4, The internal memory
       * - ICR::lecroy::location::FILE saves over the currently specied trace title, 
       * - ICR::lecroy::location::HDD  save over the incremented the filename.
       *       
       * Example: store C1 on hard disk (without incrementing filename)
       * @code  
       * lecroy_com lc("ip_address");
       * lc.store(ICR::lecroy::location::C1, ICR::lecroy::location::FILE);
       * @endcode
       * @see trigger_mode
       */
      virtual void store(const enum location::type& trace, const enum location::type& dest);
      
      /** Setup the way the data is stored.
       * @param trace  Acceptable values are ICR::lecroy::location::C1,..., ICR::lecroy::location::C4, ICR::lecroy::location::F1, ... ICR::lecroy::location::F9, ICR::lecroy::location::ALL_DISPLAYED.
       * @param dest Destination.  Acceptable values are 
       * - ICR::lecroy::location::M1, ..., ICR::lecroy::location::M4, The internal memory
       * - ICR::lecroy::location::FILE saves over the currently specied trace title, 
       * - ICR::lecroy::location::HDD  save over the incremented the filename.
       * @param mode Autosave mode
       * - store_mode::NO_AUTOSAVE  No autosave
       * - store_mode::FILL Fill hard-disk
       * - store_mode::WRAP Overwrite filename
       * @param type Format of the data. Acceptable values are
       * - store_format::ASCII 
       * - store_format::BINARY
       * - store_format::EXCEL
       * - store_format::MATHCAD
       * - store_format::MATLAB
       *
       * Example: store C1 on hard disk (with incrementing filename) autosaving on trigger, in binary format.
       * @code  
       * lecroy_com lc("ip_address");
       * lc.store(ICR::lecroy::location::C1, ICR::lecroy::location::HDD, store_mode::FILL, store_format::BINARY);
       * @endcode
       */
      virtual void store_setup(const enum location::type& trace, const enum location::type& dest, const enum store_mode::type& mode, const enum store_format::type& type);

      /** Toggle trace on/off.
       * @param trace  Acceptable values are ICR::lecroy::location::C1,..., ICR::lecroy::location::C4.
       *
       * Example:
       * @code  
       * lecroy_com lc("ip_address");
       * lc.toggle_trace(ICR::lecroy::location::C1); //if C1 is displayed turn it off, otherwise turn it on.
       * @endcode
       * @see trace_on
       * @see trace_off
       */
      virtual void toggle_trace(const enum location::type& trace);
      /** Turn trace on.
       *  @param trace  Acceptable values are ICR::lecroy::location::C1,..., ICR::lecroy::location::C4.
       *
       * Example:
       * @code  
       * lecroy_com lc("ip_address");
       * lc.trace_on(ICR::lecroy::location::C1); //Display channel 1
       * @endcode
       * @see toggle_trace
       * @see trace_off
       */
      virtual void trace_on(const enum location::type& trace) {send(ICR::lecroy::location::get_string(trace)+":TRA ON\n");}
       
      /** Turn trace off.
       *  @param trace  Acceptable values are ICR::lecroy::location::C1,..., ICR::lecroy::location::C4.
       *
       * Example:
       * @code  
       * lecroy_com lc("ip_address");
       * lc.trace_on(ICR::lecroy::location::C1); //Turn off channel 1
       * @endcode
       * @see toggle_trace
       * @see trace_on
       */
      virtual void trace_off(const enum location::type& trace) {send(ICR::lecroy::location::get_string(trace)+":TRA OFF\n");}

      //virtual void transfer_file(const std::string& filename){send("TRFL DISK,HDD,FILE,'"+filename+"'\n");}
      
      /* Set trigger channel.
       * @param channel The channel to trigger from
       */
      // void trigger(const enum location::type& channel) {send("TRSE SNG,SR," + location::get_string(channel)+"\n");}

      /** Set the trigger coupling.
       * @param channel  Acceptable values are ICR::lecroy::location::C1,..., ICR::lecroy::location::C4.
       * @param coupling Accepatable values are   for internal source
       *  - ICR::lecroy::coupling::DC 
       *  - ICR::lecroy::coupling::AC 
       * while for external sources the following can be used
       *  - ICR::lecroy::coupling::A1M: AC 1 MOhm coupling,
       *  - ICR::lecroy::coupling::D1M, DC 1 MOhm coupling,
       *  - ICR::lecroy::coupling::D50, DC 50Ohm coupling,
       *  - ICR::lecroy::coupling::GND, coupled to ground.
       *
       * Example: Trigger to channel 1 with 50 ohm coupling
       * @code  
       * lecroy_com lc("ip_address");
       * lc.trigger_coupling(ICR::lecroy::location::C1, ICR::lecroy::coupling::D50); 
       * @endcode
       * \see trigger_delay, trigger_level, trigger_mode
       */
      virtual void trigger_coupling(const enum location::type& channel, const enum coupling::type& coupling);
      /** Set the trigger delay.
       * Negative numbers indicate a post trigger.
       * @param delay Time in seconds. Negative numbers indicate a post trigger.
       *
       * Example: delay of 50us
       * @code  
       * lecroy_com lc("ip_address");
       * lc.trigger_delay(50e-6); 
       * @endcode
       * \see trigger_coupling, trigger_level, trigger_mode
       */
      virtual void trigger_delay(const double& delay);// negive numbers for post trigger
      /** Set the trigger level.
       * @param channel  Acceptable values are ICR::lecroy::location::C1,..., ICR::lecroy::location::C4.
       * @param level Level in volts.
       *
       * Example: trigger level of 1V
       * @code  
       * lecroy_com lc("ip_address");
       * lc.trigger_level(1); 
       * @endcode
       * @see trigger_coupling, trigger_delay, trigger_mode
       */
      virtual void trigger_level(const enum location::type& channel, const double& level);
      
      /** Set the trigger mode.
       * @param mode Acceptible values
       * - trigger_mode::AUTO,
       * - trigger_mode::NORM,
       * - trigger_mode::SINGLE,
       * - trigger_mode::STOP.
       *
       * Example: put triggering in normal mode
       * @code  
       * lecroy_com lc("ip_address");
       * lc.trigger_mode(trigger_mode::NORM); 
       * @endcode
       * @see stop
       */
      virtual void trigger_mode(const enum trigger_mode::type& mode) {send("TRMD "+trigger_mode::get_string(mode)+"\n");}

      /**  Define a trigger pattern,
       *  The command specifies the logic level of the pattern sources (Channel 1,
       * Channel 2, Channel 3, Channel 4, External), 
       * as well as the states under which a trigger can occur. 
       * This command can be used even if the Pattern trigger mode has not been activated.
       * Notation:
       * - L LOW H HIGH
       * - AND OR
       (const enum location::type& trace)       * - NAND NOR
       *
       * @param pattern The pattern string to send to the scope.
       *
       * Example:
       * The following instruction configures the logic state of the pattern as
       * HLXH (CH 1 = H, CH 2 = L, CH 3 = X, CH 4 = H) and defines the
       * state as NOR.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.trigger_pattern("C1,H,C2,L,C4,H,STATE,NOR"); 
       * @endcode
       */
      virtual void trigger_pattern(const std::string& pattern){send("TRPA " + pattern + "\n");}
      
      /** Selects the condition that will trigger the acquisition of waveforms. 
       * Depending on the trigger type, additional parameters may need to be specified.
       * Thes additional parameters are grouped in pairs. The first in the pair
       * names the variable to be modified, while the second gives the new
       * value to be assigned. Pairs may be given in any order and restricted
       * to only those variables to be changed.
       * @param type Accptable values
       * - ICR::lecroy::trigger_type::DROPOUT
       * - ICR::lecroy::trigger_type::EDGE
       * - ICR::lecroy::trigger_type::GLITCH
       * - ICR::lecroy::trigger_type::INTERVAL
       * - ICR::lecroy::trigger_type::STD
       * - ICR::lecroy::trigger_type::SINGLE_SOURCE
       * - ICR::lecroy::trigger_type::STATE_QUALIFIED
       * - ICR::lecroy::trigger_type::EDGE_QUALIFIED
       * @param source Acceptable values
       * - ICR::lecroy::location::C1, 
       * - ICR::lecroy::location::C2, 
       * - ICR::lecroy::location::C3, 
       * - ICR::lecroy::location::C4, 
       * - ICR::lecroy::location::LINE, 
       * - ICR::lecroy::location::EX, 
       * - ICR::lecroy::location::EX10, 
       * - ICR::lecroy::location::PA,
       * - ICR::lecroy::location::ETM10
       * @param hold_type Acceptable values:
       * - ICR::lecroy::trigger_hold_type::TIME_GREATER_THAN, 
       * - ICR::lecroy::trigger_hold_type::TIME_WIDTH, 
       * - ICR::lecroy::trigger_hold_type::EVENTS,
       * - ICR::lecroy::trigger_hold_type::PULSE_SMALLER_THAN, 
       * - ICR::lecroy::trigger_hold_type::PULSE_GREATER_THAN, 
       * - ICR::lecroy::trigger_hold_type::INTERVAL_SMALLER_THAN, 
       * - ICR::lecroy::trigger_hold_type::INTEVAL_GREATER_THAN, 
       * - ICR::lecroy::trigger_hold_type::PULSE_WIDTH,
       * - ICR::lecroy::trigger_hold_type::INTERVAL_WIDTH,
       * - ICR::lecroy::trigger_hold_type::NO_HOLD
       * @param hold_value1 Hold value1
       * @param hold_value2 Necessary if using ICR::lecroy::trigger_hold_type::PULSE_WIDTH or ICR::lecroy::trigger_hold_type::INTERVAL_WIDTH.
       *
       * Example: The following instruction selects the single-source trigger with
       * Channel 1 as trigger source. Hold type and hold value are chosen as
       * “pulse smaller” than 20 ns:
       * @code  
       * lecroy_com lc("ip_address");
       * lc.trigger_select(ICR::lecroy::trigger_type::EDGE,ICR::lecroy::location::C1, ICR::lecroy::trigger_hold_type::PULSE_SMALLER_THAN, 20e-9 ); 
       * @endcode
       * @see trigger_positive_edge
       * @see trigger_negative_edge
       */
      virtual void trigger_select(const enum trigger_type::type& type,const enum location::type& source,const enum trigger_hold_type::type& hold_type, const double& hold_value1 = 1.0, const double& hold_value2 = 1.0);
      
      /** Trigger on positive edge of selected channel.
       * @param channel  Acceptable values
       * - ICR::lecroy::location::C1, 
       * - ICR::lecroy::location::C2, 
       * - ICR::lecroy::location::C3, 
       * - ICR::lecroy::location::C4, 
       * - ICR::lecroy::location::LINE, 
       * - ICR::lecroy::location::EX, 
       * - ICR::lecroy::location::EX10.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.trigger_positive_edge(ICR::lecroy::location::C1); 
       * @endcode
       * @see trigger_negative_edge
       * @see trigger_select
       */
      virtual void trigger_positive_edge(const enum location::type& channel){send(ICR::lecroy::location::get_string(channel)+":TRSL POS\n");}
      /** Trigger on negative edge of selected channel.
       * @param channel  Acceptable values
       * - ICR::lecroy::location::C1, 
       * - ICR::lecroy::location::C2, 
       * - ICR::lecroy::location::C3, 
       * - ICR::lecroy::location::C4, 
       * - ICR::lecroy::location::LINE, 
       * - ICR::lecroy::location::EX, 
       * - ICR::lecroy::location::EX10.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.trigger_negative_edge(ICR::lecroy::location::C1); 
       * @endcode
       * @see trigger_positive_edge
       * @see trigger_select
       */
      virtual void trigger_negative_edge(const enum location::type& channel){send(ICR::lecroy::location::get_string(channel)+":TRSL NEG\n");}
      /** Set the volts per division.
       * @param channel  Acceptable values
       * - ICR::lecroy::location::C1, 
       * - ICR::lecroy::location::C2, 
       * - ICR::lecroy::location::C3, 
       * - ICR::lecroy::location::C4.
       * @param volts
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.volts_per_div(ICR::lecroy::location::C1,1); 
       * @endcode
       * @see set_timebase
       */
      virtual void volts_per_div(const enum location::type& channel,const double& volts) {send(ICR::lecroy::location::get_string(channel)+":VDIV "+stringify(volts)+"\n");}
      
      /* Wait for current aquisition to complete before processing new commands.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.wait(); 
       * @endcode
       */
      using lecroy_com_manager<coms_method>::wait;
      // virtual void wait(const unsigned int secs= 0) {
      // 	std::string cmd = "WAIT "+stringify(secs)+"\n";
      // 	send(cmd,true);
      // }


      virtual void demand_fresh_aquisition();

      /** Get a waveform trace.
       * Download a trace directly to your pc.
       * @param channel The trace that you want to download.
       * @return The lecroy_file that decodes the trace.
       */
      virtual lecroy_file get_waveform(const enum location::type& channel);

      virtual std::string get_waveform_text(const enum location::type& channel);

      /** Annotate waveform with text.
       * @param channel The channel to write the text to.
       * @param text Text description of the waveform. Upto 160 characters permitted.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.waveform_text(ICR::lecroy::location::C1, "Aquired on 21/09/2010"); 
       * @endcode
       */
      virtual void waveform_text(const enum location::type& channel,const std::string& text) 
	throw(exception::waveform_description_too_long);
      
      /** Display the XY plot on the scope
       * @param do_display Display the XY plot on the scope.
       *
       * @code  
       * lecroy_com lc("ip_address");
       * lc.display(false); 
       * @endcode
       */
      virtual void display(const bool& do_display);
      
    };



    /** lecroy_64Xi. 
     *  The interface for the lecroy_64Xi.
     */
    class lecroy_64Xi : public lecroy_com<IP>
    {
    public:
      lecroy_64Xi(const std::string device) : lecroy_com<IP>(device) {}
      using lecroy_com<IP>::send;
      using lecroy_com<IP>::recv;
      //      using lecroy_com<IP>::timed_recv;
      using lecroy_com<IP>::set_coupling;
      using lecroy_com<IP>::set_timebase;
      using lecroy_com<IP>::average;
      using lecroy_com<IP>::arm;
      using lecroy_com<IP>::auto_calibrate;
      using lecroy_com<IP>::calibrate;
      using lecroy_com<IP>::beep;
      using lecroy_com<IP>::clear_memory;
      using lecroy_com<IP>::clear_sweeps;
      using lecroy_com<IP>::clear;
      using lecroy_com<IP>::set_date;
      using lecroy_com<IP>::aquire_single;
      using lecroy_com<IP>::message;
      using lecroy_com<IP>::set_vertical_offset;
      using lecroy_com<IP>::vertical_offset_constant;
      using lecroy_com<IP>::persistence;
      using lecroy_com<IP>::reset;
      using lecroy_com<IP>::sequence;
      using lecroy_com<IP>::stop;
      using lecroy_com<IP>::store;;
      using lecroy_com<IP>::store_setup;
      using lecroy_com<IP>::toggle_trace;
      using lecroy_com<IP>::trace_on;
      using lecroy_com<IP>::trace_off;
      using lecroy_com<IP>::trigger_coupling;
      using lecroy_com<IP>::trigger_delay;
      using lecroy_com<IP>::trigger_level;
      using lecroy_com<IP>::trigger_select;
      using lecroy_com<IP>::trigger_mode;
      using lecroy_com<IP>::trigger_pattern;
      using lecroy_com<IP>::trigger_negative_edge;
      using lecroy_com<IP>::volts_per_div;
      using lecroy_com<IP>::wait;
      using lecroy_com<ICR::coms::IPmanager>::get_waveform;
      using lecroy_com<ICR::coms::IPmanager>::get_waveform_text;
      using lecroy_com<ICR::coms::IPmanager>::waveform_text;
      using lecroy_com<ICR::coms::IPmanager>::display;
    };
  }
}

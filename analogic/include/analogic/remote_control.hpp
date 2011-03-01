#include "coms_manager/serial_manager.hpp"
#include "stringify.hpp"
#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <string>
#include <map>


namespace ICR {
  namespace analogic {

    // /** A class that maps the allowed clock rates */
    // class clock_rate
    // {
    //   static std::map<const std::string, float> m_value;
    // public:
    //   /** Constructor */
    //   clock_rate();
    //   /** Get the clock rate from the provided string.
    //    * @param rate The desired clock rate.
    //    * Acceptable values are:
    //    *
    //    *  - 1.25ns 
    //    *  - 2.5ns
    //    *  - 5ns
    //    *  - 10ns
    //    *  - 20ns
    //    *  - 50ns
    //    *  - 100ns
    //    *  - 200ns
    //    *  - 500ns
    //    *  - 1us
    //    *  - 2us
    //    *  - 5us
    //    *  - 10us
    //    *  - 100us
    //    *  - 1ms
    //    *  - 2ms
    //    *  - 5ms
    //    *  - 10ms
    //    *  - 20ms
    //    *  - 50ms
    //    *  - 100ms
    //    *  - 1s
    //    */
    //   static
    //   float 
    //   value(const std::string rate)
    //   {
    // 	return m_value[rate];
    //   } 
    // };
    
    /** The allowed trigger modes */
    struct trigmode
    {
      /** An enumeration of the trigger modes.
       *  @attention Modes 9 and 10 (START AND STOP) only work with GET (group execute trigger) response (GPIB)
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

    using ICR::coms::flow_control;
    using ICR::coms::parity;
    using ICR::coms::stop_bits;
    

    /** A class for controlling the analogic by remote control.
     *  
     * This class allows you to access the "button" control of the analogic.
     * (That is everything that you can do by pressing the buttons on the analogic).
     * It does not allow you to download arbitrary waveforms.
     * To do that use the analogic_program class.
     * @see analogic_program.
     */
    class analogic_remote_control 
    {
    private:
      ICR::coms::serial_manager m_serial;
      
      
    public:
      /** Constructor.
       * @param address the address of the device. For example "COM1"  (windows, or "/dev/ttyS0" (linux).
       *  @param baud_rate The baud rate. 
       *  @param flow_control The flow control. Acceptable values are flow_control::none, flow_control::software, flow_control::hardware.
       *  @param parity The parity of the connection. Acceptable values are parity::none, parity::even, parity::odd.
       *  @param stop_bits The number of stop bits. Acceptable values are stop_bits::one, stop_bits::one_point_five, stop::bits::two
       */
      analogic_remote_control(const std::string& address,
			      const unsigned int& baud_rate                  = 9600,
			      const enum flow_control::type&   flow_control  = ICR::coms::flow_control::hardware,
			      const enum parity::type&         parity        = ICR::coms::parity::none,
			      const enum stop_bits::type&      stop_bits     = ICR::coms::stop_bits::one);
	
      /** Destructor */
      virtual ~analogic_remote_control() ;
      
      
      /** Send a command.
       *  @param cmd The comand to send. These must be appended with a carriage return character ("\r")
       */
      void
      send(const std::string& cmd)
	throw (boost::system::system_error) 
      {m_serial.send(cmd + "\r");}
      
      
      /** Set the output to be on.  */
      void turn_on()   {send("RUN");}
      /** Set the output to be off.  */
      void turn_off()  {send("STOP");} 
      /** Set the fequency.
       * @param frequency The frequency
       */
      void frequency(const float frequency) {send("FREQ="+stringify(frequency));}

      /** Set the voltage.
       * @param V The voltage requested
       */
      void voltage(const float V) {send("AMP="+stringify(V));}
      
      /** Set the voltage offset.
       * @param offset The offset (in volts)
       */
      void offset(const float offset) {send("OFST="+stringify(offset));}

      /** Set the delay.
       * @param delay The delay (in seconds)
       */
      void delay(const float delay) {send("DLT="+stringify(delay));}

      
      /** Set the pulse_width.
       * @param pw The pulse_width (in seconds)
       */
      void pulse_width(const float pw) {send("PLSW="+stringify(pw));}
      
      /* Select the active signal output
       *  @param option The output channel (A or B)
       */
      //void output_channel(const channel::type& option);  //OUTSEL

      /** Select the number of points to compute
        * @param points The approximate number of points to compute.
	* @attention The number of points calculated will be as close as possible to points.
	*  However, the exact number depends upon the druation of the math function in relationto ta clock rate, 
	*  amonst other things, so the actual number of points can for certian parameters be many orders
	*  of magnitudes different from points.
       */
      void computation_points(const unsigned long points)
      {
	send("TGTPNTS="+stringify(points));
      }

      
      /** Set the clock rate.
       *  @param rate The requested clock rate.
       * Accpetable values are:
       *  - 1.25ns 
       *  - 2.5ns
       *  - 5ns
       *  - 10ns
       *  - 20ns
       *  - 50ns
       *  - 100ns
       *  - 200ns
       *  - 500ns
       *  - 1us
       *  - 2us
       *  - 5us
       *  - 10us
       *  - 100us
       *  - 1ms
       *  - 2ms
       *  - 5ms
       *  - 10ms
       *  - 20ms
       *  - 50ms
       *  - 100ms
       *  - 1s
       */
      void clock_rate(const float& rate)
      {
	send("CLK="+stringify(rate)); //stringify(analogic::clock_rate::value(rate)));
      }
      
      /** Set the maximum of the standard waveform.
       *  Maximum of sin, square or triangular waves.
       *  @param V the max voltage of the wave.
       *  @attention max and min are alternatives to the voltage and offset 
       *  @see min
       *  @see voltage
       *  @see offset
       */
      void max(const float V) {send("HIGH="+stringify(V)); }

       /** Set the minimum of the standard waveform.
       *  Minimum of sin, square or triangular waves.
       *  @param V the min voltage of the wave.
       *  @attention max and min are alternatives to the voltage and offset 
       *  @see max
       *  @see voltage
       *  @see offset
       */
      void min(const float V)   {send("LOW="+stringify(V)); }

      /** Set phase angle between the leading edge of sync out and the first positve zero crossing of sine wave.
       * @param degree The phase angle in degrees 
       */
      void phase(const float& degree)
      {
	float frac = degree/360.0;
	send("PHS="+stringify(frac));
      }

      /** Select the trigger mode.
       *  @param trigmode The trigger mode selection, a choice from the trigmode::type enumation.
       */
      void
      trigmode(const enum trigmode::type& trigmode) {send("TRIGM="+stringify(trigmode));}
      
      /** Set the config.
       *  Display a list of special features on ther terminal screen.
       */
      void
      config() {send("CONFIG");}
      
      /** Display to moniter the action resulting from the remote commands.
       * @param is_echo Is in echo mode
       */
      void 
      echo(bool is_echo) {send("ECHO"+stringify(is_echo));}
      
      /** Return the control to the local state.
       *  Enables the front panel keys
       */
      void 
      local() {send("LOC");}

      /** Enter remote mode.
       *  All front panel keys will be disabled apart for REM/LOC key and reset key.
       */
      void
      remote() {send("REM");}

      /** Reset the machine. Simulating power off and on again. */
      void reset() {send("RESET");}

      /** Enter modied mode.
       */
      void
      modified() {send("MOD");}

      /** Marker pulse time delay.
       *  Set the time delay until the marker pulse.
       *  @param seconds The time in seconds
       */
      void
      marker(const float& seconds) 
      {
	send("MARK="+stringify(seconds));
      }


      /** Toggles standard sine wave function. */
      void sin()
      {
	send("SSIN");
      }
      
      /** Toggles standard square wave function. */
      void square()
      {
	send("SSQR");
      }
      
      /** Toggles standard triangle wave function. */
      void triangle()
      {
	send("STRI");
      }

      /** Restore.
       * Enter OP mode or Modified mode, suspends computations.
       * @attention Although this command emulates most restore key functions, it does not dupliate the actual key press.
       * It will not enter MODIFIED mode as the key will (see modified command)
       * @see modified
       */
      void
      restore() {send("RES");}

      /** Set they symmetry of a sine or triangle wave, in percent
       * @param percent The symmetry (50 by default)
       * @attention This command will aslo hcange the duty or pulse_width parameter in square wave function
       */
      void symmetry(const float percent)
      {
	send("SYM="+stringify(percent));
      }

	


      /** Set the period length of the output waveform 
       *  @param seconds The time in seconds
       *  @attention If echo is enabled then the display will change the FREQ field to the period_length format.
       */
      void
      period_length(float seconds){send("PER="+stringify(time));}
      
      /** Set a polynomial expression
       * @param expr The expression to enter
       */
      void 
      expression(const std::string& expr)
      {
	send("POLY");  //put into POLY mode
	send(expr);    //send the expression
	send("ENTER"); //enter the command
      }

      /** Recall a function from memory.
       *  @param filename The filename
       */
      void
      recall(const std::string& filename)
      {
	send("RCL="+filename);
      }

      /** Enter data download mode.
       *  Necessary before data can be accepted.
       */
      void
      data()
      {
	send("DATA");
      }

      
      /** Set the duty cycle.
       * The amount of time (in percent of one cycle or period) that a square wave will stay high.
       * @param dc The duty cycle (in percent)
       * @attention When duty cycle is changed, symmetry parameter for sine save and standard triangle
       *  will also change.  An alternative is pulse width
       *  @see pulse_width
       *  @see symmetry
       */
      void
      duty_cycle(const float& dc)
      {
	send("DUTY="+stringify(dc));
      }
      
    };

  }
}

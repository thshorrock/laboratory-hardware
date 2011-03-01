

namespace ICR {
  namespace analogic {

    class clock_rate
    {
      map<std::string, float> m_value;
    public:
      clock_rate()
	: m_value();
      {
	m_value["1.25ns"] = 1.25e-9;
	m_value["2.5ns"] = 2.5e-9;
	m_value["5ns"] = 5e-9;
	m_value["10ns"] = 10-9;
	m_value["20ns"] = 20e-9;
	m_value["50ns"] = 50e-9;
	m_value["100ns"] = 100e-9;
	m_value["200ns"] = 200e-9;
	m_value["500ns"] = 500e-9;
	m_value["1us"] = 1e-6;
	m_value["2us"] = 2e-6;
	m_value["5us"] = 5e-6;
	m_value["10us"] = 10e-6;
	m_value["100us"] = 100e-6;
	m_value["1ms"] = 1e-3;
	m_value["2ms"] = 2e-3;
	m_value["5ms"] = 5e-3;
	m_value["10ms"] = 10e-3;
	m_value["20ms"] = 20e-3;
	m_value["50ms"] = 50e-3;
	m_value["100ms"] = 100e-3;
	m_value["1s"] = 1.0;
      }
      float value(const string& rate) const {return m_value[rate];}
    
     
    };

    struct trigmode
    {
      /** The trigger modes.
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
    }

    class analogic_remote_control 
    {
    private:
      serial_manager serial;
      
      
    public:
      /** Constructor.
       * @param address the address of the device. For example "COM1" for a serial device, or the IP address for a TCP connection.
       */
      analogic_remote_control(const std::string& address);
	
      /** Destructor */
      virtual ~analogic_remote_control() ;
      
      
      /** Send a command.
       *  @param cmd The comand to send. These must be appended with a carriage return character ("\r")
       */
      send(const std::string& cmd)
	throw (boost::system::system_error) 
      {serial.send(cmd);}
      
      
      /** Set the output to be on.  */
      void turn_on()   {send("RUN\r");}
      /** Set the output to be off.  */
      void turn_off()  {send("STOP\r");} 
      /** Set the fequency.
       * @param frequency The frequency
       */
      void frequency(const double frequency) {send("FREQ="+stringify(frequency)+"\r");}

      /** Set the voltage.
       * @param V The voltage requested
       */
      void voltage(const double V) {send("AMP="+stringify(V)+"\r");}
      
      /** Set the voltage offset.
       * @param offset The offset (in volts)
       */
      void offset(const double offset) {send("OFST="+stringify(offset)+"\r");}

      /** Set the delay.
       * @param delay The delay (in seconds)
       */
      void delay(const double delay) {send("DLT="+stringify(delay)+"\r");}

      
      /** Set the pulse_width.
       * @param delay The pulse_width (in seconds)
       */
      void pulse_width(const double pulse_width) {send("PLSW="+stringify(pulse_width)+"\r");}
      
      /* Select the active signal output
       *  @param option The output channel (A or B)
       */
      //void output_channel(const channel::type& option);  //OUTSEL

      /** Select the number of points to compute
        * @param points The approximate number of points to compute.
	* @attension The number of points calculated will be as close as possible to points.
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
       * Accpetable values are
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
      void clock_rate(const string& rate)
      {
	send("CLK="+stringify(clock_rate.value(rate)));
      }
      
      /** Set the maximum of the standard waveform.
       *  Maximum of sin, square or triangular waves.
       *  @Param V the max voltage of the wave.
       *  @attention max and min are alternatives to the voltage and offset 
       *  @see min
       *  @see voltage
       *  @see offset
       */
      void max(const double V) {send("HIGH="+stingify(V)); }

       /** Set the minimum of the standard waveform.
       *  Minimum of sin, square or triangular waves.
       *  @Param V the min voltage of the wave.
       *  @attention max and min are alternatives to the voltage and offset 
       *  @see max
       *  @see voltage
       *  @see offset
       */
      void min(const double V)   {send("LOW="+stingify(V)); }

      /** Set phase angle between the leading edge of sync out and the first positve zero crossing of sine wave.
       * @param degree The phase angle in degrees 
       */
      void phase(const double& degree)
      {
	double frac = degree/360.0;
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
      echo(bool is_echo) {send{"ECHO"+stringify(is_echo));}
      
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
      void sin()
      {
	send("SSQR");
      }
      
      /** Toggles standard triangle wave function. */
      void sin()
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
      void symmetry(const double percent)
      {
	send("SYM="+stringify(percent));
      }

	


      /** Set the period length of the output waveform 
       *  @param seconds The time in seconds
       *  @attention If echo is enabled then the display will change the FREQ field to the period_length format.
       */
      void
      period_length(double time){send("PER="+stringify(time));}
      
      /** Set a polynomial expression
       * @param expr The expression to enter
       */
      void 
      expression(const string& expr)
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
       * @attension When duty cycle is changed, symmetry parameter for sine save and standard triangle
       *  will also change.  An alternative is pulse width
       *  @see pulse_width
       *  @see symmetry
       */
      void
      duty_cycle(const double& dc)
      {
	send("DUTY="+stringify(dc));
      }
      
    };

  }
}

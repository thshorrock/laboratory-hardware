
#include "coms_manager/serial_manager.hpp"
#include "stringify.hpp"
#include "trigger_mode.hpp"
#include "construct_data/analogic_data.hpp"


#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <string>
#include <map>


/** @defgroup Device Device Control.
*/

/** ICR namespace */
namespace ICR {
  /** analogic  namespace */
  namespace analogic {

    
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
     * @ingroup Device
     */
    class analogic_remote_control 
    {
    private:
      ICR::coms::serial_manager m_serial;
      
      bool m_poly;
      bool m_SFM; //standard function mode
      bool m_mod; //modify mode
      
      void send_no_cr(const std::string& cmd)
	throw (boost::system::system_error) 
      {
	m_serial.send(cmd);
	ICR::coms::sleep(400); //sleep to allow command to be set
      }
      
    public:
      //Constructors
      /** @name Constructors
       *///@{
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
	
      ///@}
      
      //Constructors
      /** @name Destructor
       *///@{
      /** Destructor */
      virtual ~analogic_remote_control() ;
      ///@}
      
      //Generic send
      
      /** @name Generic send
       *  Send a generic command.
       *///@{
      /** Send a command.
       *  All the other commands call this function. 
       *  If a call to the 2045 is not implemented (or is not working) then you may call this function directly.
       *  The required charage return is appended to the command for you - as is a short delay - to allow the 2045 time to process the command.
       *  @param cmd The comand to send. These must be appended with a carriage return character ("\r")
       */
      void
      send(const std::string& cmd)
	throw (boost::system::system_error) 
      {
	send_no_cr(cmd +"\r");
      }
      ///@}
      
      //expression mode
      /** @name Expression mode
       *  Enter an expression into the 2045.
       *///@{

      
      /** Set a polynomial expression.
       * Notice that the functions
       *  - CLK Override the auto clock rate
       *  - FIL Select frequency of waveform filter
       *  - MARK Sets occurance of marder pulse in seconds
       *  - NAMP sets amplitude of noise in volts rms
       *  - NBW selects bandwidth of noise
       *  - OFST selects vertical offset
       * can all be used within the expression mode.
       * Example:
       * @code 
       *   analogic_remote_control analogic("/dev/ttyUSB0");
       *   analogic.expression("AT TRIG RPT 1 ( FOR 30u 0.0 FOR 2u 0.5 FOR 1u 0) CLK = 1.25n MARK = 20u");
       *   analogic.turn_on();
       * @endcode
       * @param expr The expression to enter.
       */
      void 
      expression(const std::string& expr)
      {
	turn_off(); //must turn off otherwise 2045 crashes ...
	send("POLY");  //put into POLY mode
	send(expr);    //send the expression
	send("ENTER"); //enter the command
      }
      ///@}

      //commands
      /** @name General commands.
       *  These commands are called an be called at any time.
       *///@{
      
      /** Set the output to be on.  */
      void turn_on()   {
	send("RUN");
	send("ENTER"); //enter the command
	ICR::coms::sleep(4000);
      }
      /** Set the output to be off.  */
      void turn_off()  {send("STOP");} 

      /** Set the config.
       *  Display a list of special features on ther terminal screen.
       */
      void
      config() {send("CONFIG");}
      
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

      /** Enter polynomial mode.
       * @see mod
       */
      void
      polynomial() {
	send("POLY");
	m_SFM = false; m_poly = true; m_mod = false;
      }

      /** Enter modifify mode.
       * @see poly
       */
      void
      modify() 
      {
	send("MOD");
	m_SFM = false; m_poly = false; m_mod = true;
      }

      

      /** Toggles standard sine wave function. */
      void sin()
      {
	send("SSIN");
	m_SFM = true; m_poly = false; m_mod = false;
      }
      
      /** Toggles standard square wave function. */
      void square()
      {
	send("SSQR");
	m_SFM = true; m_poly = false; m_mod = false;
      }
      
      /** Toggles standard triangle wave function. */
      void triangle()
      {
	send("STRI");
	m_SFM = true; m_poly = false; m_mod = false;
      }

      /** Restore.
       * Enter OP mode or Modified mode, suspends computations.
       * @attention Although this command emulates most restore key functions, it does not dupliate the actual key press.
       * It will not enter MODIFIED mode as the key will (see modified command)
       * @see modified
       */
      void
      restore() {
	if (!m_poly) polynomial();
	send("RES");
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
      data(const analogic_data& data)
      {
	send("DATA");
	std::vector<std::string> commands = data.collect_data();
	for(size_t i=0;i<commands.size();++i){
	  std::cout<<"com "<<i<<" = ";
	  for(size_t j=0;j<commands[i].size();++j){
	    std::cout<<(int) commands[i][j]<<" ";

	  }
	  std::cout<<std::endl;
	  //std::cout<<"com "<<i<<" = "<<commands[i]<<std::endl;
	  // send_no_cr(commands[i]);
	}

	//for_each(commands.begin(), commands.end(), boost::bind(&analogic_remote_control::send, this, _1) );
      }

      
      ///@}

      //Standard Function Mode functions...
      /** @name Standard Function Mode commands.
       *  Modify standard output.
       *  These commands operate on the standard functions (eg. sin, square, triangular wave).
       *  If one of these waves has not been selected prior to the the use of these functions,
       *  then a sine wave is selected by default.
       *  @see sin
       *  @see triangle
       *  @see square.
       *///@{

      
      /** Set the fequency of the standard function.
       * @param frequency The frequency
       */
      void frequency(const float frequency) 
      {
	if (!m_SFM) sin(); //default to sine wave
	send("FREQ="+stringify(frequency));
      }

      /** Set the voltage.
       * @param V The voltage requested
       */
      void voltage(const float V) 
      {
	if (!m_SFM) sin(); //default to sine wave
	send("AMP="+stringify(V));
      }
      
      /** Set the voltage offset.
       * @param offset The offset (in volts)
       */
      void offset(const float offset) 
      {
	if (!m_SFM) sin(); //default to sine wave
	send("OFST="+stringify(offset));
      }

      /** Set the delay.
       * @param delay The delay (in seconds)
       */
      void delay(const float delay) {
	if (!m_SFM) sin(); //default to sine wave
	send("DLT="+stringify(delay));
      }

      
      /** Set the pulse_width.
       * @param pw The pulse_width (in seconds)
       */
      void pulse_width(const float pw) {
	if (!m_SFM) sin(); //default to sine wave
	send("PLSW="+stringify(pw));
      }

      
      
      /** Set the maximum of the standard waveform.
       *  Maximum of sin, square or triangular waves.
       *  @param V the max voltage of the wave.
       *  @attention max and min are alternatives to the voltage and offset 
       *  @see min
       *  @see voltage
       *  @see offset
       */
      void max(const float V) {
	if (!m_SFM) sin(); //default to sine wave
	send("HIGH="+stringify(V)); 
      }

       /** Set the minimum of the standard waveform.
       *  Minimum of sin, square or triangular waves.
       *  @param V the min voltage of the wave.
       *  @attention max and min are alternatives to the voltage and offset 
       *  @see max
       *  @see voltage
       *  @see offset
       */
      void min(const float V)   {
	if (!m_SFM) sin(); //default to sine wave
	send("LOW="+stringify(V));
      }

      /** Set phase angle between the leading edge of sync out and the first positve zero crossing of sine wave.
       * @param degree The phase angle in degrees 
       */
      void phase(const float& degree)
      {
	if (!m_SFM) sin(); //default to sine wave
	float frac = degree/360.0;
	send("PHS="+stringify(frac));
      }

      /** Select the trigger mode.
       *  @param trigmode The trigger mode selection, a choice from the trigmode::type enumation.
       */
      void
      trigmode(const enum trigmode::type& trigmode) {
	if (!m_SFM) sin(); //default to sine wave
	send("TRIGM="+stringify(trigmode));
      }
      
      /** Marker pulse time delay.
       *  Set the time delay until the marker pulse.
       *  @param seconds The time in seconds
       */
      void
      marker(const float& seconds) 
      {
	if (!m_SFM) sin(); //default to sine wave
	send("MARK="+stringify(seconds));
      }


      /** Set they symmetry of a sine or triangle wave, in percent
       * @param percent The symmetry (50 by default)
       * @attention This command will aslo hcange the duty or pulse_width parameter in square wave function
       */
      void symmetry(const float percent)
      {
	if (!m_SFM) sin(); //default to sine wave
	send("SYM="+stringify(percent));
      }

	


      /** Set the period length of the output waveform 
       *  @param seconds The time in seconds
       *  @attention If echo is enabled then the display will change the FREQ field to the period_length format.
       */
      void
      period_length(float seconds)
      {
	if (!m_SFM) sin(); //default to sine wave
	send("PER="+stringify(time));
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
	if (!m_SFM) sin(); //default to sine wave
	send("DUTY="+stringify(dc));
      }

      
      ///@}
      
      /* Select the active signal output
       *  @param option The output channel (A or B)
       */
      //void output_channel(const channel::type& option);  //OUTSEL


      //Independant modifiers
      /** @name Independant modifiers.
       *  These commands select general properties of the 2045.
       *///@{

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

      /** Display to moniter the action resulting from the remote commands.
       * @param is_echo Is in echo mode
       */
      void 
      echo(bool is_echo) {send("ECHO="+stringify(is_echo));}
      
      ///@}
      
      
    };

  }
}

      /* Set the modified clock rate.
       *  This sets the output clock frequency.  Altering it will scale the frequency of the outputted wave.
       *  @param rate The requested clock rate.
       * The 2045 will round the request clock rate to the following values
       *  - 1.25n 
       *  - 2.5n
       *  - 5n
       *  - 10n
       *  - 20n
       *  - 50n
       *  - 100n
       *  - 200n
       *  - 500n
       *  - 1u
       *  - 2u
       *  - 5u
       *  - 10u
       *  - 100u
       *  - 1m
       *  - 2m
       *  - 5m
       *  - 10m
       *  - 20m
       *  - 50m
       *  - 100m
       *  - 1
       */
      // void clock_rate(const std::string& rate)
      // {
      // 	modify(); //goto modify mode
      // 	send("CLK="+rate); //stringify(analogic::clock_rate::value(rate)));
      // }

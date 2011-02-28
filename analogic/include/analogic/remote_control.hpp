

namespace ICR {
  namespace analogic {

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
      void offset(const double delay) {send("DLT="+stringify(delay)+"\r");}

      
      /** Set the pulse_width.
       * @param delay The pulse_width (in seconds)
       */
      void pulse_width(const double pulse_width) {send("PLSW="+stringify(pulse_width)+"\r");}
      
      /** Select the active signal output
       *  @param option The output channel (A or B)
       */
      void output_channel(const channel::type& option);  //OUTSEL

      /** Select the number of points to compute
       *  @param points The approximate number of points to compute.
       */
      void computation_points(const unsigned long points); //TGTPNTS
      
      void clock_rate(const clock_rate::type& option);
      
      
      void square_max(const double V); //HIGH
      void square_min(const double V); //LOW
      //TRIGMODE
      //PHS
      //expression
      
      
    };

  }
}

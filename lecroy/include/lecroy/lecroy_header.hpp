#pragma once 

#include <boost/smart_ptr.hpp>
#include <sstream>

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>



namespace ICR{

    namespace exception
    {
      struct lecroy_file_is_corrupted{};
      
    }
  namespace lecroy{
    


    using std::string;
    using std::vector;

    //forward declaration
    class lecroy_file; 
  
    /** This stores the byte-locations of the various regions of the file */
    struct byte_locations
    {
      long begin,  //!< The begining of the file
	preamble,  //!< The begining of the preable
	trig,      //!< The begining of the trigger section
	ris,       //!< The begining of the RIS section
	array_1,   //!< The begining of the first array
	array_2;   //!< The begining of the second array
    };
  
#pragma pack(push, 4) // pack on 4-byte boundaries (Important!)
    
    

    /** The lecroy header class. 
     * This class contains all the information regarding the waveform */
    class lecroy_header{
    private:
      
      void rev(int siz, char val[]);
      void read(boost::shared_ptr<std::iostream>& pIn, char* variable, const short& bytes);
      
      void decode(boost::shared_ptr<std::iostream>& pIn);


      std::size_t size(void) const {return NOM_SUBARRAY_COUNT;};

    
      int begin_byte(void) const {return byte.begin;};
      int preamble_byte(void) const {return byte.preamble;};
      int trig_byte(void) const {return byte.trig;};
      int ris_byte(void) const {return byte.ris;};
      long array_1_byte(void) const {return byte.array_1;};
      long array_2_byte(void) const {return byte.array_2;};

      void begin_byte(long i){byte.begin = i;};
      void preamble_byte(long i){byte.preamble = i;};
      void trig_byte(long i){byte.trig = i;};
      void ris_byte(long i){byte.ris = i;};
      void array_1_byte(long i){byte.array_1 = i;};
      void array_2_byte(long i){byte.array_2 = i;};
      void set_byte_pos(void);

      void set_trigger(unsigned long n, double intv, double off);
      void set_gain(double, double);
      double trigger_time(unsigned long n) const {return trig_times[n].first;};
      double trigger_offset(unsigned long n) const { return trig_times[n].second;};
      double ris_time(int n) const {return ris_times[n];};

      char JUNK[256];
      char DESCRIPTOR_NAME[16];
      char TEMPLATE_NAME[16];
      short COMM_TYPE;
      short COMM_ORDER;
      long WAVE_DESCRIPTOR;
      long USER_TEXT;
      long RES_DESC1;
      long TRIGTIME_ARRAY;
      long RIS_TIME_ARRAY;
      long RES_ARRAY1;
      long WAVE_ARRAY_1;
      long WAVE_ARRAY_2;
      long RES_ARRAY2;
      long RES_ARRAY3;
      char INSTRUMENT_NAME[16];
      long INSTRUMENT_NUMBER;
      char TRACE_LABEL[16];
      short RESERVED1;
      short RESERVED2;
      long WAVE_ARRAY_COUNT;
      long PNTS_PER_SCREEN;
      long FIRST_VALID_PNT;
      long LAST_VALID_PNT;
      long FIRST_POINT;
      long SPARSING_FACTOR;
      long SEGMENT_INDEX;
      long SUBARRAY_COUNT;
      long SWEEPS_PER_ACQ;
      short POINTS_PER_PAIR;
      short PAIR_OFFSET;
      float VERTICAL_GAIN;
      float VERTICAL_OFFSET;
      float MAX_VALUE;
      float MIN_VALUE;
      short NOMINAL_BITS;
      short NOM_SUBARRAY_COUNT;
      float HORIZ_INTERVAL;
      double HORIZ_OFFSET;
      double PIXEL_OFFSET;
      char VERTUNIT[48];
      char HORUNIT[48];
      float HORIZ_UNCERTAINTY;
      double TRIGGER_TIME_seconds;
      char TRIGGER_TIME_minutes;
      char TRIGGER_TIME_hours;
      char TRIGGER_TIME_days;
      char TRIGGER_TIME_months;
      short TRIGGER_TIME_year;
      short TRIGGER_TIME_unused;
      float ACQ_DURATION;
      short RECORD_TYPE;
      short PROCESSING_DONE;
      short RESERVED5;
      short RIS_SWEEPS;
      short TIMEBASE;
      short VERT_COUPLING;
      float PROBE_ATT;
      short FIXED_VERT_GAIN;
      short BANDWIDTH_LIMIT;
      float VERTICAL_VERNIER;
      float ACQ_VERT_OFFSET;
      short WAVE_SOURCE;
      short ENDBLOCK;
      std::string PREAMBLE; //usertext at most 160 char
    public:

      lecroy_header();
      
      /** Input a header from the string.
       * @param header the header string.  It includes the DESC and the TIME information. */
      void operator>>(const std::string& header)
      {
	boost::shared_ptr<std::iostream> pIn(new std::stringstream(header));
	decode(pIn);
      }
      
      friend 
      boost::shared_ptr<std::iostream>&
      operator<<(boost::shared_ptr<std::iostream>& pOut, const lecroy_header& lc);

      friend class lecroy_file;
      
      /** Decode the header from an string.
       * This function is typically used in remote mode when a "WF? DESC\n" request has been made
       * @param header The header. It includes the DESC and the TIME information.
       */
      void decode_string(const std::string& header)
      {
	boost::shared_ptr<std::iostream> pIn(new std::stringstream(header));
	decode(pIn);
      }

      //operator const std::string();
      
      
      /** A destructor. */
      ~lecroy_header();
    

      /** Description - always WAVEDESC. 
       * @return The variable's value.*/
      std::string  descriptor_name()    const {return std::string(DESCRIPTOR_NAME);}
      /** Template name.
       * @return The variable's value. */
      std::string  template_name()      const {return std::string(TEMPLATE_NAME);}
      /**Communication format. Set by COMM_FORMAT command. 
       *   - 0:  byte
       *   - 1:  word 
       * @return The variable's value.*/
      short        comm_type()          const {return COMM_TYPE;}
      /**The Byte ordering. 
       * An enumeration:
       *   - 0: HIFIRST
       *   - 1: LOWFIRST
       * @return The variable's value.
       */
      short        comm_order()         const {return COMM_ORDER;}
      /** Number of bytes of descriptor name.
       * @return The variable's value.*/
      long         wave_descriptor()    const {return WAVE_DESCRIPTOR;}
      /** Number of bytes of user text.
       * @return The variable's value.*/
      long         user_text()          const {return USER_TEXT;}
      /** Number of bytes of trigger time array.
       * @return The variable's value.*/
      long         trigtime_array()     const {return TRIGTIME_ARRAY;}
      /** Number of bytes of random interleaved sampling array.
       * @return The variable's value.*/
      long         ris_time_array()     const {return RIS_TIME_ARRAY;}
      /** Number of bytes of first data array.  
	  In transmitted waveform represents the NP parameter of the WFSU command.
       * @return The variable's value.*/
      long         wave_array_1()       const {return WAVE_ARRAY_1;}
      /** Number of bytes of first data array.
       * @return The variable's value. */
      long         wave_array_2()       const {return WAVE_ARRAY_2;}
      /** The instrument name.
       * @return The variable's value. */
      std::string  instrument_name()    const {return std::string(INSTRUMENT_NAME);}
      /** The instrument number. 
       * @return The variable's value.*/
      long         instrument_number()  const {return INSTRUMENT_NUMBER;}
      /** The trace label. Identifies the waveform.
       * @return The variable's value. */
      std::string  trace_label()        const {return std::string(TRACE_LABEL);}
      /** The number of data points in the data array. 
       *  If there are two data arrays (FFT or Extrema), 
       *  this number applies to each array separately. 
       * @return The variable's value.*/
      long         wave_array_count()   const {return WAVE_ARRAY_COUNT;}
      /** Number of points on the screen. @return The variable's value.*/
      long         pnts_per_screen()    const {return PNTS_PER_SCREEN;}
      /** The number of points to skip untill the first good poing.
       *  This is usually zero for normal waveforms. 
       * @return The variable's value.*/
      long         first_valid_pnt()    const {return FIRST_VALID_PNT;}

      /** The index of last good data point in record before padding (blanking) was started.
       *  LAST_VALID_POINT = WAVE_ARRAY_COUNT-1 except for aborted sequence
       *  and rollmode acquisitions. 
       * @return The variable's value.*/
      long         last_valid_pnt()     const {return LAST_VALID_PNT;}

      /** For input and output, indicates the offset relative to 
       *  the beginning of the trace buffer.
       *  Value is the same as the FP parameter of the WFSU remote command.
       * @return The variable's value. */
      long         first_point  ()      const {return FIRST_POINT;}
      /** For input and output, indicates the sparsing into the transmitted data block.
       *  Value is the same as the SP parameter of the WFSU remote command. 
       * @return The variable's value.*/
      long         sparsing_factor ()   const {return SPARSING_FACTOR;}
 
     /** For input and output, indicates the; index of the transmitted segment.
       *  Value is the same as the SN parameter of the WFSU remote command. 
       * @return The variable's value.*/
      long         segment_index ()     const {return SEGMENT_INDEX;}

      /** The acquired segment count. Between 0 and NOM_SUBARRAY_COUNT.
       *  @see nom_subarray_count().
       * @return The variable's value. */
      long         subarray_count ()    const {return SUBARRAY_COUNT;}
      /** For Average or Extrema, number of sweeps accumulated;  else 1 
       * @return The variable's value.*/
      long         sweeps_per_acq()     const {return SWEEPS_PER_ACQ;}

      /** For Peak Dectect waveforms (which always include data points in DATA_ARRAY_1
       *  and min/max pairs in DATA_ARRAY_2).
       *  Value is the number of data points for each min/max pair.
       * @return Value is the number of data points for each min/max pair.
       * @return The variable's value. */
      short        points_per_pair()    const {return POINTS_PER_PAIR;}
      /** For Peak Dectect waveforms only. 
       *  Value is the number of data points by which the first min/max pair in
       *  DATA_ARRAY_2 is offset relative to the first data value in DATA_ARRAY_1.
       * @return The variable's value. */
      short        pair_offset()        const {return PAIR_OFFSET;}
      /** Vertical gain. 
       * @return The variable's value.*/
      float        vertical_gain()      const {return VERTICAL_GAIN;}
      /** Vertical Offset.
       *  To get values from raw data use: 
       *  true value = VERTICAL_GAIN * data - VERTICAL_OFFSET 
       * @return The variable's value.*/
      float        vertical_offset()    const {return VERTICAL_OFFSET;}
      /** Maximum value.
       * @return The variable's value.*/
      float        max_value()          const {return MAX_VALUE;}
      /** Minium value 
       * @return The variable's value.*/
      float        min_value()          const {return MIN_VALUE;}
      /** A measure of the intrinsic precision
       *  of the observation: ADC data is 8 bit; averaged data is 10-12 bit, etc. 
       * @return The variable's value.*/
      short        nominal_bits()       const {return NOMINAL_BITS;}
      /** For sequence, the nominal segment count, otherwise 1.  
       * \see subarray_count()
       * @return The variable's value. */
      short        nom_subarray_count() const {return NOM_SUBARRAY_COUNT;}
      /** Sampling interval for time domain waveforms. 
       * @return The variable's value.*/
      float        horiz_interval()     const {return HORIZ_INTERVAL;}
      /** Trigger offset for first sweep of trigger.
       * Seconds between the trigger and the first data point. 
       * @return The variable's value.*/
      double       horiz_offset()       const {return HORIZ_OFFSET;}
      /** Units of the vertical axis.
       * @return The variable's value. */
      std::string  vertunit()           const {return std::string(VERTUNIT);}
      /** Units of the horizontal axis.
       * @return The variable's value. */
      std::string  horunit()            const {return std::string(HORUNIT);}
      /** Uncertainty from one acquisition to the next, of the horizontal offset in seconds. 
       * @return The variable's value.*/
      float        horiz_uncertainty()  const {return HORIZ_UNCERTAINTY;}
      /** Duration of the acquisition (in sec) in multi-trigger waveforms.
       *  (e.g. sequence, RIS, or averaging)
       * @return The variable's value. */
      float        acq_duration()       const {return ACQ_DURATION;}
      /** Record type.
       *  An enumeration as follows 
       *    - 0: Single_sweep
       *    - 1: interleaved
       *    - 2: histogram
       *    - 3: graph
       *    - 4: filter_coefficient
       *    - 5: complex
       *    - 6: extrema
       *    - 7: sequence_obsolete
       *    - 8: centered_RIS
       *    - 9: peak_detect
       * @return The variable's value.
       */
      short        record_type()        const {return RECORD_TYPE;}
      /** Processing done.
       *  An enumeration:
       *    - 0: no_processing
       *    - 1: fir_filter
       *    - 2: interpolated
       *    - 3: sparsed
       *    - 4: autoscaled
       *    - 5: no_result
       *    - 6: rolling
       *    - 7: cumulative
       * @return The variable's value.
       */
      short        processing_done ()   const {return PROCESSING_DONE;}
      /** For RIS, the number of sweeps; else 1. * @return The variable's value.*/
      short        ris_sweeps()         const {return RIS_SWEEPS;}
      /** Timebase enumeration.
       *  Enumeration is as follows
       *    - 0: 1_ps/div
       *    - 1: 2_ps/div
       *    - 2: 5_ps/div
       *    - 3: 10_ps/div
       *    - 4: 20_ps/div
       *    - 5: 50_ps/div: 
       *    - 6: 100_ps/div
       *    - 7: 200_ps/div
       *    - 8: 500_ps/div
       *    - 9: 1_ns/div
       *    - 10: 2_ns/div
       *    - 11: 5_ns/div
       *    - 12: 10_ns/div
       *    - 13: 20_ns/div
       *    - 14: 50_ns/div
       *    - 15: 100_ns/div
       *    - 16: 200_ns/div
       *    - 17: 500_ns/div
       *    - 18: 1_us/div
       *    - 19: 2_us/div
       *    - 20: 5_us/div
       *    - 21: 10_us/div
       *    - 22: 20_us/div
       *    - 23: 50_us/div
       *    - 24: 100_us/div
       *    - 25: 200_us/div
       *    - 26: 500_us/div
       *    - 27: 1_ms/div
       *    - 28: 2_ms/div
       *    - 29: 5_ms/div
       *    - 30: 10_ms/div
       *    - 31: 20_ms/div
       *    - 32: 50_ms/div
       *    - 33: 100_ms/div
       *    - 34: 200_ms/div
       *    - 35: 500_ms/div
       *    - 36: 1_s/div
       *    - 37: 2_s/div
       *    - 38: 5_s/div
       *    - 39: 10_s/div
       *    - 40: 20_s/div
       *    - 41: 50_s/div
       *    - 42: 100_s/div
       *    - 43: 200_s/div
       *    - 44: 500_s/div
       *    - 45: 1_ks/div
       *    - 46: 2_ks/div
       *    - 47: 5_ks/div
       *    - 100 EXTERNAL
       * @return The variable's value.
       */
      short        timebase()           const {return TIMEBASE;}
      /** The vertical coupling.
       * An enumeration as follows.
       *    - 0: DC_50_Ohms
       *    - 1: ground
       *    - 2: DC_1MOhm
       *    - 3: ground
       *    - 4: AC_1MOhm
       * @return The variable's value.
       */
      short        vert_coupling()      const {return VERT_COUPLING;}
      /** The probe attenuation. 
       * @return The variable's value.*/
      float        probe_att()          const {return PROBE_ATT;}

      /** The vertical gain.
       * An enumeration as follows:
       *    - 0: 1_uV/div
       *    - 1: 2_uV/div
       *    - 2: 5_uV/div
       *    - 3: 10_uV/div
       *    - 4: 20_uV/div
       *    - 5: 50_uV/div
       *    - 6: 100_uV/div
       *    - 7: 200_uV/div
       *    - 8: 500_uV/div
       *    - 9: 1_mV/div
       *    - 10: 2_mV/div
       *    - 11: 5_mV/div
       *    - 12: 10_mV/div: 
       *    - 13: 20_mV/div
       *    - 14: 50_mV/div
       *    - 15: 100_mV/div
       *    - 16: 200_mV/div
       *    - 17: 500_mV/div
       *    - 18: 1_V/div
       *    - 19: 2_V/div
       *    - 20: 5_V/div
       *    - 21: 10_V/div
       *    - 22: 20_V/div
       *    - 23: 50_V/div
       *    - 24: 100_V/div
       *    - 25: 200_V/div
       *    - 26: 500_V/div
       *    - 27: 1_kV/div
       * @return The variable's value.
       */
      short        fixed_vert_gain()    const {return FIXED_VERT_GAIN;}

      /** The bandwidth.
       * An enumeration:
	 *    - 0: off
	 *    - 1: on
       * @return The variable's value.
       */
      short        bandwidth_limit()    const {return BANDWIDTH_LIMIT;}
      /** The vertical vernier scale
       * @return The variable's value. */
      float        vertical_vernier()   const {return VERTICAL_VERNIER;}
      /** The vertical offset. 
       * @return The variable's value. */
      float        acq_vert_offset()    const {return ACQ_VERT_OFFSET;}

      /** The channel.
       * An enumeration:
       *    - 0: CHANNEL_1
       *    - 1: CHANNEL_2
       *    - 2: CHANNEL_3
       *    - 3: CHANNEL_4
       *    - 9: UNKNOWN
       * @return The variable's value.
       */
      short        wave_source()        const {return WAVE_SOURCE;}
      /**  The text submitted by the user. 
       * At most 160 characters long.
       * @return The variable's value.
       */
      std::string  text()           const {return std::string(PREAMBLE);} //usertext at most 160 char

  
      /** Set the preamble text.
       * @param text The preamble text 
       */
      void set_preamble(const std::string& text)
      {
	PREAMBLE = text;
	USER_TEXT = text.size();
	set_byte_pos();
      }
      //first bytes of each block

      byte_locations byte;

      std::vector<
	std::pair<double,double>
	> trig_times;
    
    std::vector<double> ris_times;
      // vec get_times() const;
      // vec get_trace(size_t i = 0) const;

      // string get_filename() const {return m_filename;};
      //mat get_matrix();

    };


    // boost::shared_ptr<std::iostream>&
    // operator<<(boost::shared_ptr<std::iostream>& out, const lecroy_header& lc);
    
    /** Output the header to a stream.
     * This displays all of the header info in ascii format.
     * @param out The output stream
     * @param lc The lecroy header
     * @return A reference to the output stream
     */
    std::ostream&  
    operator<<(std::ostream& out, const lecroy_header& lc);

#pragma pack(pop)             // restore packing
  


  }   //namespace lecroy


} //namespace icr

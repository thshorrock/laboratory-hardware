#pragma once

namespace ICR {
  namespace analogic {
    
    /** The type of segment in a data header file.*/
    struct segment_type
    {
      enum value 
       	{
       	  BLOCK,     //!< a single set of data to be outputted with no repeated segments
       	  CONSTANT,  //!< a constant value to be outputted for a period of time and programmer does not want to fill memory with the same value
       	  END,       //!< this is the closing header at the end of the download sequence
       	  REPEAT,    //!< The data is going to repeated n times ( n is specifed in the segement length)
       	  REPEAT_END //!< Denotes the end of a repeated segment
       	};
    };
      
    /** The defualt segment length.
     *  The intension is that it gets specialised for a particular segment type (you must do this for your programs to compile!).
     */
    template<int type>
    class segment_length
    {
    public:
      /** The default segment_length is dot defined.  The compiler will not croak if you do not specialize this class. */
      segment_length(); 
    };
      
    /** The block type specialisation */
    template<>
    class segment_length<segment_type::BLOCK>
    {
      unsigned int m_length;
    public:
      /** Constructor.
       *  @param length An unsiged 32 bit integer representing the length of the data block in to 16 bit words.
       */
      segment_length(const unsigned int& length)
	: m_length(length)
      {}
	
      /** Get the block length 
       * @return The block length  of the data block in to 16 bit words.
       */
      unsigned int 
      block_length() const {return m_length;}
    };
	
    /** The constant type specialisation */
    template<>
    class segment_length<segment_type::CONSTANT>
    {
      unsigned int m_clock_pulses;
    public:
      /** Constructor 
       * @param clock_pulses The number of clock pulses that determine the duration of the constant portion.
       */
      segment_length(const unsigned int& clock_pulses)
	: m_clock_pulses(clock_pulses)
      {}

      /** Get the number of constant clock pulses
       * @return The number of clock pulses that determine the duration of the constant portion.
       */
      unsigned int 
      constant_clock_pulses() const {return m_clock_pulses;}
    };

	
    /** The end type specialisation */
    template<>
    class segment_length<segment_type::END>
    {
    public:
      /** The value for the end segment
       * @return 0.
       */
      unsigned int 
      end_value() const {return 0;}
    };
    /** The type of repeat loop. */
    struct repeat_loop
    {
      /** Enumeration specifying the type of repeat loop 
       *
       *  For example:
       *  \code  RPT 2 (RPT 5( FOR 10m SIN(1K*T) FOR 1m0) ) \endcode
       *
       *  The outer_loop is used to specify the RPT2, the nested_loop is used to specify the RPT 5.
       *  @attension The nested loop counter cannot be allowed to end on the same clock cycle as the outer loop counter when
       *  a clcok period faster than 15ns is being used.
       */
      enum type {
	OUTER  = 0,  //!< This specifies the number of repeats to the waveform or waveform segment is to be repeated
	NESTED = 1, //!< This loop specifies the repeats that are nested within the outer loop.
      };
    };

    /** The repeat type specialisation */
    template<>
    class segment_length<segment_type::REPEAT>
    {
      const enum repeat_loop::type& m_loop_type;
      unsigned short m_repeats;
    public:
      /** Constructor.
       *  The number of repeats  may be specified.
       *  There are two types of repeats: repeat_loop::OUTER and repeat_loop::NESTED.
       *  
       *  @param loop Whether the loop is of type  repeat_loop::OUTER or repeat_loop::NESTED.
       *  @param repeats The number of repeats; 
       */
      segment_length(const enum repeat_loop::type& loop, const unsigned short& repeats)
	: m_loop_type(loop), m_repeats(repeats)
      {}
	
      /** The outer loop value.
       * @return The outer loop count.
       */
      unsigned  short
      loop_type() const {return m_loop_type;}
	
      /** The neested loop value.
       * @return The nested loop count.
       */
      unsigned  short
      repeats() const {return m_repeats;}
    };
      
    /** The repeat_end type specialisation */
    template<>
    class segment_length<segment_type::REPEAT_END>
    {
      const enum repeat_loop::type& m_loop_type;
    public:
      /** Constructor.
       *  The end of a repeat block
       *  There are two types of repeats: repeat_loop::OUTER and repeat_loop::NESTED.
       *  
       *  @param loop Whether the loop is of type  repeat_loop::OUTER or repeat_loop::NESTED.
       */
      segment_length(const enum repeat_loop::type& loop)
	: m_loop_type(loop)
      {}
	
      /** The outer loop value.
       * @return The outer loop count.
       */
      unsigned int
      end_repeat_value() const {return m_loop_type;}
	
    };

    /** Get the constant value.
     *  This returns the value 0 unless the segment is specialised to type segment_type::CONSTANT.
     */
    template<int type>
    class constant_value
    {
    public:
      /** Get the constant value.
       *  @return Unless the type is segment_type::CONSTANT this will return 0.
       */
      float
      value() const {return 0.0;}
    };
      
    /** The segment_type::CONSTANT specialization. */
    template<>
    class constant_value<segment_type::CONSTANT>
    {
      const float m_constant;
    public:
      constant_value(const float& value) : m_constant(value) {}
      /** Get the constant value.
       *  @return The constant
       */
      float
      value() const {return m_constant;}
    };
      
      
      

    
  }
}

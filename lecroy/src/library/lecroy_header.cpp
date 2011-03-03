

#include "lecroy/lecroy_header.hpp"
#include "string.h"

void
ICR::lecroy::lecroy_header::rev(int siz, char val[]){
  //error::out("desc reverse called");
  char tmp;
  int ix;
  
  siz = siz - 1;
  if (siz < 1) return;
  for (ix=(siz-1)/2;ix>=0;ix=ix-1)
    {
      tmp = val[ix]; val[ix] = val[siz-ix]; val[siz-ix] = tmp;
    }
  
}

void 
ICR::lecroy::lecroy_header::read(boost::shared_ptr<std::iostream>& pIn, char* variable, const short& bytes) 
{
  if (COMM_ORDER ==0) {
    pIn->read( variable ,bytes);
    rev(bytes, variable);
  }
  else
     pIn->read( variable ,bytes);
  if (!pIn->good() ) {
    std::cout<<"READ ERR"<<std::endl;

    throw ICR::exception::lecroy_file_is_corrupted();
  }
}



ICR::lecroy::lecroy_header::lecroy_header()
  : 
  JUNK(),
  DESCRIPTOR_NAME(), 
  TEMPLATE_NAME(), 
  COMM_TYPE(0), 
  COMM_ORDER(0), 
  WAVE_DESCRIPTOR(0), 
  USER_TEXT(0), 
  RES_DESC1(0), 
  TRIGTIME_ARRAY(0), RIS_TIME_ARRAY(0), RES_ARRAY1(0), WAVE_ARRAY_1(0),
  WAVE_ARRAY_2(0), RES_ARRAY2(0), RES_ARRAY3(0), 
  INSTRUMENT_NAME(), INSTRUMENT_NUMBER(0), 
  TRACE_LABEL(), 
  RESERVED1(0), RESERVED2(0), 
  WAVE_ARRAY_COUNT(0), 
  PNTS_PER_SCREEN(0),
  FIRST_VALID_PNT(0), 
  LAST_VALID_PNT(0), 
  FIRST_POINT(0), 
  SPARSING_FACTOR(0), 
  SEGMENT_INDEX(0), 
  SUBARRAY_COUNT(0), 
  SWEEPS_PER_ACQ(0), 
  POINTS_PER_PAIR(0), 
  PAIR_OFFSET(0),
  VERTICAL_GAIN(0), 
  VERTICAL_OFFSET(0), 
  MAX_VALUE(0), 
  MIN_VALUE(0), 
  NOMINAL_BITS(0), 
  NOM_SUBARRAY_COUNT(0), 
  HORIZ_INTERVAL(0), HORIZ_OFFSET(0), 
  PIXEL_OFFSET(0), VERTUNIT(), HORUNIT(), 
  HORIZ_UNCERTAINTY(0), 
  TRIGGER_TIME_seconds(0), TRIGGER_TIME_minutes(), TRIGGER_TIME_hours(), TRIGGER_TIME_days(), TRIGGER_TIME_months(), TRIGGER_TIME_year(0), TRIGGER_TIME_unused(0),
  ACQ_DURATION(0), 
  RESERVED5(0), 
  RIS_SWEEPS(0), 
  PROBE_ATT(0), 
  VERTICAL_VERNIER(0), 
  ACQ_VERT_OFFSET(0), 
  ENDBLOCK(0), 
  PREAMBLE(), 
  byte()
{};


ICR::lecroy::lecroy_header::~lecroy_header()
{
  // try {
  //   //save(m_filename);
  // }
  // catch( error::LecroyException e){
  //   e.debug_print();
  // } 
}



void ICR::lecroy::lecroy_header::decode(boost::shared_ptr<std::iostream>& pIn) 
//throw (error::CannotOpenException)
{
  //std::cout<<"desc load called: "<<filename<<std::endl;
  //input_file_sentry pIn(filename);
  /*
   * Scan the file for the "WAVEDESC" string because in
   * some cases the .trc files may contain an initial def9 header before the
   * data and then read in the descriptor.
   */

  //std::cout<<"decode"<<std::endl;

  //std::cout<<"header = "<<pIn->str()<<std::endl;
    
    
  pIn->seekg (0, std::ios::beg); 
  bool not_read = true;
  size_t count = 0;
  size_t wavedesc_count=0;
  char* wavedesc = "WAVEDESC";
  pIn->read(JUNK,256);
  while(not_read && count<256)
    {
      if (JUNK[count] != wavedesc[wavedesc_count])
	++count;
      else{
	if (wavedesc_count == 8 ) not_read = false;
	++wavedesc_count;
	++count;
      }
    }
  byte.begin = count-9;
    //   std::string sjunk;
    //   for(size_t i=0;i<256;++i){
    // 	std::cout<<"c["<<i<<"] = "<<junk[i]<<std::endl;
    //   }
    //   std::cout<<"offset = "<<strcspn(junk,"WAVEDESC")<<std::endl;

    //   sjunk.append(junk);
    //   for(size_t i=0;i<sjunk.size();++i){
    // 	std::cout<<"["<<i<<"] = "<<(int) sjunk[i]<<std::endl;
	
    //   }
     

    //   if (!pIn->eof() ) 
    // 	{
    // 	  not_read = false;
	
    // 	}
    //   else
    // 	pIn->clear();
    //   std::cout<<"junk = "<<sjunk<<std::endl;
      
    //   byte.begin = sjunk.find("WAVEDESC");
    //   std::cout<<"begin_byte = "<<byte.begin<<std::endl;
    // }

  pIn->seekg (begin_byte(), std::ios::beg);  //not sure what these 11 chars are...
  pIn->read(DESCRIPTOR_NAME,16); //sizeof(char)*16
  pIn->read(TEMPLATE_NAME,16);
  pIn->read((char*) &COMM_TYPE ,2);
  pIn->read((char*) &COMM_ORDER,2);

  
  read(pIn, (char*) &WAVE_DESCRIPTOR,4);
  read(pIn, (char*) &USER_TEXT,4);
  read(pIn, (char*) &RES_DESC1,4);
  read(pIn, (char*) &TRIGTIME_ARRAY,4);
  read(pIn, (char*) &RIS_TIME_ARRAY,4);
  read(pIn, (char*) &RES_ARRAY1,4);
  read(pIn, (char*) &WAVE_ARRAY_1,4);
  read(pIn, (char*) &WAVE_ARRAY_2,4);
  read(pIn, (char*) &RES_ARRAY2,4);
  read(pIn, (char*) &RES_ARRAY3,4);
  //pIn->read((char*) &INSTRUMENT_NAME ,16);
  read(pIn, (char*) &INSTRUMENT_NAME,16);
  read(pIn, (char*) &INSTRUMENT_NUMBER,4);
  read(pIn, (char*) &TRACE_LABEL,16);
  read(pIn, (char*) &RESERVED1,2);
  read(pIn, (char*) &RESERVED2,2);
  read(pIn, (char*) &WAVE_ARRAY_COUNT,4);
  read(pIn, (char*) &PNTS_PER_SCREEN,4);
  read(pIn, (char*) &FIRST_VALID_PNT,4);
  read(pIn, (char*) &LAST_VALID_PNT,4);
  read(pIn, (char*) &FIRST_POINT,4);
  read(pIn, (char*) &SPARSING_FACTOR,4);
  read(pIn, (char*) &SEGMENT_INDEX,4);
  read(pIn, (char*) &SUBARRAY_COUNT,4);
  read(pIn, (char*) &SWEEPS_PER_ACQ,4);
  read(pIn, (char*) &POINTS_PER_PAIR,2);
  read(pIn, (char*) &PAIR_OFFSET,2);
  read(pIn, (char*) &VERTICAL_GAIN,4);
  read(pIn, (char*) &VERTICAL_OFFSET,4);
  read(pIn, (char*) &MAX_VALUE,4);
  read(pIn, (char*) &MIN_VALUE,4);
  read(pIn, (char*) &NOMINAL_BITS,2);
  read(pIn, (char*) &NOM_SUBARRAY_COUNT,2);
  read(pIn, (char*) &HORIZ_INTERVAL,4);
  read(pIn, (char*) &HORIZ_OFFSET,8);
  read(pIn, (char*) &PIXEL_OFFSET,8);
  read(pIn, (char*) &VERTUNIT,48);
  read(pIn, (char*) &HORUNIT,48);
  read(pIn, (char*) &HORIZ_UNCERTAINTY,4);
  read(pIn, (char*) &TRIGGER_TIME_seconds,8);
  read(pIn, (char*) &TRIGGER_TIME_minutes,1);
  read(pIn, (char*) &TRIGGER_TIME_hours,1);
  read(pIn, (char*) &TRIGGER_TIME_days,1);
  read(pIn, (char*) &TRIGGER_TIME_months,1);
  read(pIn, (char*) &TRIGGER_TIME_year,2);
  read(pIn, (char*) &TRIGGER_TIME_unused,2);
  read(pIn, (char*) &ACQ_DURATION,4);
  read(pIn, (char*) &RECORD_TYPE,2);
  read(pIn, (char*) &PROCESSING_DONE,2);
  read(pIn, (char*) &RESERVED5,2);
  read(pIn, (char*) &RIS_SWEEPS,2);
  read(pIn, (char*) &TIMEBASE,2);
  read(pIn, (char*) &VERT_COUPLING,2);
  read(pIn, (char*) &PROBE_ATT,4);
  read(pIn, (char*) &FIXED_VERT_GAIN,2);
  read(pIn, (char*) &BANDWIDTH_LIMIT,2);
  read(pIn, (char*) &VERTICAL_VERNIER,4);
  read(pIn, (char*) &ACQ_VERT_OFFSET,4);
  read(pIn, (char*) &WAVE_SOURCE,2);
  
  if (USER_TEXT > 0)
    read(pIn, (char*) &PREAMBLE, USER_TEXT);
  else
    PREAMBLE="";
  
  for (int i = 0; i<NOM_SUBARRAY_COUNT; i++) {
    double tmp1 = 1;
    double tmp2 = 1;
     read(pIn, (char*) &tmp1,8);
     read(pIn, (char*) &tmp2,8);

    std::pair<double,double> tmp(tmp1,tmp2);
    trig_times.push_back(tmp);
    // std::cout<<"tmp.first = "<<tmp.first<<"tmp.second = "<< tmp.second<<std::endl;

  }
  int ris = RES_DESC1; //The number of random interleaved sample/8?
  for (int i = 0; i<ris; i++) {
    std::cout<<"RIS IS UNTESTED..."<<std::endl<<"... CHECK CAREFULLY EVERYTHING"<<std::endl;
    double tmp1;
    read(pIn, (char*) &tmp1,8);
    ris_times.push_back(tmp1);
  }

  //std::cout<<"... here."<<std::endl;
  byte.preamble  = begin_byte()    + WAVE_DESCRIPTOR;
  byte.trig      = preamble_byte() + USER_TEXT;
  byte.ris       = trig_byte()     + TRIGTIME_ARRAY ;
  byte.array_1   = ris_byte()      + RIS_TIME_ARRAY;
  byte.array_2   = array_1_byte()  + WAVE_ARRAY_1;
  
  //std::cout<<"... here2."<<std::endl;

}

// ICR::lecroy::lecroy_header::operator const std::string() 
// {
  
//   std::string header;
    
//   boost::scoped_ptr<std::ostringstream> pOut(new std::ostringstream(header));

//   return header;
// }


boost::shared_ptr<std::iostream>&
ICR::lecroy::operator<<(boost::shared_ptr<std::iostream>& pOut, const lecroy_header& lc)
{
  /*
   * Scan the file for the "WAVEDESC" string because in
   * some cases the .trc files may contain an initial def9 header before the
   * data and then read in the descriptor.
   */ 
      
  pOut->seekp (0, std::ios::beg);  
  // std::string junk(begin_byte(),' ');
  pOut->write((char*) lc.JUNK,lc.begin_byte());
  pOut->write((char*) lc.DESCRIPTOR_NAME,16); //sizeof(char)*16
  pOut->write((char*) lc.TEMPLATE_NAME,16);
  pOut->write((char*) &lc.COMM_TYPE ,2);
  pOut->write((char*) &lc.COMM_ORDER,2);
  pOut->write((char*) &lc.WAVE_DESCRIPTOR,4);
  pOut->write((char*) &lc.USER_TEXT,4);
  pOut->write((char*) &lc.RES_DESC1,4);
  pOut->write((char*) &lc.TRIGTIME_ARRAY,4);
  pOut->write((char*) &lc.RIS_TIME_ARRAY,4);
  pOut->write((char*) &lc.RES_ARRAY1,4);
  pOut->write((char*) &lc.WAVE_ARRAY_1,4);
  pOut->write((char*) &lc.WAVE_ARRAY_2,4);
  pOut->write((char*) &lc.RES_ARRAY2,4);
  pOut->write((char*) &lc.RES_ARRAY3,4);
  pOut->write((char*) &lc.INSTRUMENT_NAME,16);
  pOut->write((char*) &lc.INSTRUMENT_NUMBER,4);
  pOut->write((char*) &lc.TRACE_LABEL,16);
  pOut->write((char*) &lc.RESERVED1,2);
  pOut->write((char*) &lc.RESERVED2,2);
  pOut->write((char*) &lc.WAVE_ARRAY_COUNT,4);
  pOut->write((char*) &lc.PNTS_PER_SCREEN,4);
  pOut->write((char*) &lc.FIRST_VALID_PNT,4);
  pOut->write((char*) &lc.LAST_VALID_PNT,4);
  pOut->write((char*) &lc.FIRST_POINT,4);
  pOut->write((char*) &lc.SPARSING_FACTOR,4);
  pOut->write((char*) &lc.SEGMENT_INDEX,4);
  pOut->write((char*) &lc.SUBARRAY_COUNT,4);
  pOut->write((char*) &lc.SWEEPS_PER_ACQ,4);
  pOut->write((char*) &lc.POINTS_PER_PAIR,2);
  pOut->write((char*) &lc.PAIR_OFFSET,2);
  pOut->write((char*) &lc.VERTICAL_GAIN,4);
  pOut->write((char*) &lc.VERTICAL_OFFSET,4);
  pOut->write((char*) &lc.MAX_VALUE,4);
  pOut->write((char*) &lc.MIN_VALUE,4);
  pOut->write((char*) &lc.NOMINAL_BITS,2);
  pOut->write((char*) &lc.NOM_SUBARRAY_COUNT,2);
  pOut->write((char*) &lc.HORIZ_INTERVAL,4);
  pOut->write((char*) &lc.HORIZ_OFFSET,8);
  pOut->write((char*) &lc.PIXEL_OFFSET,8);
  pOut->write((char*) &lc.VERTUNIT,48);
  pOut->write((char*) &lc.HORUNIT,48);
  pOut->write((char*) &lc.HORIZ_UNCERTAINTY,4);
  pOut->write((char*) &lc.TRIGGER_TIME_seconds,8);
  pOut->write((char*) &lc.TRIGGER_TIME_minutes,1);
  pOut->write((char*) &lc.TRIGGER_TIME_hours,1);
  pOut->write((char*) &lc.TRIGGER_TIME_days,1);
  pOut->write((char*) &lc.TRIGGER_TIME_months,1);
  pOut->write((char*) &lc.TRIGGER_TIME_year,2);
  pOut->write((char*) &lc.TRIGGER_TIME_unused,2);
  //pOut->write((char*) &lc.TRIGGER_TIME,6);
  pOut->write((char*) &lc.ACQ_DURATION,4);
  pOut->write((char*) &lc.RECORD_TYPE,2);
  pOut->write((char*) &lc.PROCESSING_DONE,2);
  pOut->write((char*) &lc.RESERVED5,2);
  pOut->write((char*) &lc.RIS_SWEEPS,2);
  pOut->write((char*) &lc.TIMEBASE,2);
  pOut->write((char*) &lc.VERT_COUPLING,2);
  pOut->write((char*) &lc.PROBE_ATT,4);
  pOut->write((char*) &lc.FIXED_VERT_GAIN,2);
  pOut->write((char*) &lc.BANDWIDTH_LIMIT,2);
  pOut->write((char*) &lc.VERTICAL_VERNIER,4);
  pOut->write((char*) &lc.ACQ_VERT_OFFSET,4);
  pOut->write((char*) &lc.WAVE_SOURCE,2);
  pOut->write((char*) &lc.ENDBLOCK,2);
      
  // pOut->write((char*) &lc.PREAMBLE, USER_TEXT );
      
  
 // *out <<"Descriptor              = "<< lc.descriptor_name()
 //      <<"\ntemplate name         = "<< lc.template_name() 
 //      <<"\ncomm type             = "<< lc.comm_type()    
 //      <<"\ncomm order            = "<< lc.comm_order()
 //      <<"\nwave descriptor       = "<< lc.wave_descriptor()
 //      <<"\nuser_text             = "<< lc.user_text()
 //      <<"\ntrigtime_array        = "<< lc.trigtime_array()
 //      <<"\nris_time_array        = "<< lc.ris_time_array()
 //      <<"\nwave_array_1          = "<< lc.wave_array_1()
 //      <<"\nwave_array_2          = "<< lc.wave_array_2()
 //      <<"\ninstrument_name       = "<< lc.instrument_name()
 //      <<"\ninstrument_number     = "<< lc.instrument_number()
 //      <<"\ntrace_label           = "<< lc.trace_label()
 //      <<"\nwave_array_count      = "<< lc.wave_array_count()
 //      <<"\npnts_per_screen       = "<< lc.pnts_per_screen()
 //      <<"\nfirst_valid_pnt       = "<< lc.first_valid_pnt()
 //      <<"\nlast_valid_pnt        = "<< lc.last_valid_pnt()
 //      <<"\nfirst_point           = "<< lc.first_point()
 //      <<"\nsparsing_factor       = "<< lc.sparsing_factor()
 //      <<"\nsegment_index         = "<< lc.segment_index()
 //      <<"\nsubarray_count        = "<< lc.subarray_count()
 //      <<"\nsweeps_per_acq        = "<< lc.sweeps_per_acq()
 //      <<"\npoints_per_pair       = "<< lc.points_per_pair()
 //      <<"\npair_offset           = "<< lc.pair_offset()
 //      <<"\nvertical_gain         = "<< lc.vertical_gain()
 //      <<"\nmax_value             = "<< lc.max_value()
 //      <<"\nmin_value             = "<< lc.min_value()
 //      <<"\nnominal_bits          = "<< lc.nominal_bits()
 //      <<"\nnom_subarray_count    = "<< lc.nom_subarray_count()
 //      <<"\nhoriz_interval        = "<< lc.horiz_interval()
 //      <<"\nhoriz_offset          = "<< lc.horiz_offset()
 //      <<"\nvertunit              = "<< lc.vertunit()
 //      <<"\nhorunit               = "<< lc.horunit()
 //      <<"\nhoriz_uncertainty     = "<< lc.horiz_uncertainty()
 //      <<"\nacq_duration          = "<< lc.acq_duration()
 //      <<"\nrecord_type           = "<< lc.record_type()
 //      <<"\nprocessing_done       = "<< lc.processing_done()
 //      <<"\nris_sweeps            = "<< lc.ris_sweeps()
 //      <<"\ntimebase              = "<< lc.timebase()
 //      <<"\nvert_coupling         = "<< lc.vert_coupling()
 //      <<"\nprobe_att             = "<< lc.probe_att()
 //      <<"\nfixed_vert_gain       = "<< lc.fixed_vert_gain()
 //      <<"\nbandwidth_limit       = "<< lc.bandwidth_limit()
 //      <<"\nvertical_vernier      = "<< lc.vertical_vernier()
 //      <<"\nacq_vert_offset       = "<< lc.acq_vert_offset()
 //      <<"\nwave_source           = "<< lc.wave_source()
 //      <<std::endl;
 //  *out   <<"\nPreamble   = "<< lc.text()<<std::endl;
 //  *out<<"lc.nom_subarray_count() = "<<lc.nom_subarray_count()<<std::endl;
 //  //  out  <<"\nAline size = "<< lc.wave_array_count()/ lc.nom_subarray_count()<<std::endl;
 //  // out   <<"\nNumber of alines = "<<lc.subarray_count()<<std::endl;

					     
      
  return pOut;

}
std::ostream&  
ICR::lecroy::operator<<(std::ostream& out, const lecroy_header& lc)
{
  out <<"Descriptor              = "<< lc.descriptor_name()
      <<"\ntemplate name         = "<< lc.template_name() 
      <<"\ncomm type             = "<< lc.comm_type()    
      <<"\ncomm order            = "<< lc.comm_order()
      <<"\nwave descriptor       = "<< lc.wave_descriptor()
      <<"\nuser_text             = "<< lc.user_text()
      <<"\ntrigtime_array        = "<< lc.trigtime_array()
      <<"\nris_time_array        = "<< lc.ris_time_array()
      <<"\nwave_array_1          = "<< lc.wave_array_1()
      <<"\nwave_array_2          = "<< lc.wave_array_2()
      <<"\ninstrument_name       = "<< lc.instrument_name()
      <<"\ninstrument_number     = "<< lc.instrument_number()
      <<"\ntrace_label           = "<< lc.trace_label()
      <<"\nwave_array_count      = "<< lc.wave_array_count()
      <<"\npnts_per_screen       = "<< lc.pnts_per_screen()
      <<"\nfirst_valid_pnt       = "<< lc.first_valid_pnt()
      <<"\nlast_valid_pnt        = "<< lc.last_valid_pnt()
      <<"\nfirst_point           = "<< lc.first_point()
      <<"\nsparsing_factor       = "<< lc.sparsing_factor()
      <<"\nsegment_index         = "<< lc.segment_index()
      <<"\nsubarray_count        = "<< lc.subarray_count()
      <<"\nsweeps_per_acq        = "<< lc.sweeps_per_acq()
      <<"\npoints_per_pair       = "<< lc.points_per_pair()
      <<"\npair_offset           = "<< lc.pair_offset()
      <<"\nvertical_gain         = "<< lc.vertical_gain()
      <<"\nmax_value             = "<< lc.max_value()
      <<"\nmin_value             = "<< lc.min_value()
      <<"\nnominal_bits          = "<< lc.nominal_bits()
      <<"\nnom_subarray_count    = "<< lc.nom_subarray_count()
      <<"\nhoriz_interval        = "<< lc.horiz_interval()
      <<"\nhoriz_offset          = "<< lc.horiz_offset()
      <<"\nvertunit              = "<< lc.vertunit()
      <<"\nhorunit               = "<< lc.horunit()
      <<"\nhoriz_uncertainty     = "<< lc.horiz_uncertainty()
      <<"\nacq_duration          = "<< lc.acq_duration()
      <<"\nrecord_type           = "<< lc.record_type()
      <<"\nprocessing_done       = "<< lc.processing_done()
      <<"\nris_sweeps            = "<< lc.ris_sweeps()
      <<"\ntimebase              = "<< lc.timebase()
      <<"\nvert_coupling         = "<< lc.vert_coupling()
      <<"\nprobe_att             = "<< lc.probe_att()
      <<"\nfixed_vert_gain       = "<< lc.fixed_vert_gain()
      <<"\nbandwidth_limit       = "<< lc.bandwidth_limit()
      <<"\nvertical_vernier      = "<< lc.vertical_vernier()
      <<"\nacq_vert_offset       = "<< lc.acq_vert_offset()
      <<"\nwave_source           = "<< lc.wave_source()
      <<std::endl;
  out   <<"\nPreamble   = "<< lc.text()<<std::endl;
  out<<"lc.nom_subarray_count() = "<<lc.nom_subarray_count()<<std::endl;
  //  out  <<"\nAline size = "<< lc.wave_array_count()/ lc.nom_subarray_count()<<std::endl;
  // out   <<"\nNumber of alines = "<<lc.subarray_count()<<std::endl;

					     
      
  return out;
}

// void ICR::lecroy::lecroy_header::increase_size(const int n)
// {  
//   //set the appropriate variables for the new size
  //   // std::cout<<"TRIGTIME_ARRAY "<<TRIGTIME_ARRAY<<std::endl;
  //   //std::cout<<"WAVE_ARRAY_COUNT "<<WAVE_ARRAY_COUNT<<std::endl;
  //   // std::cout<<"NOM_SUBARRAY_COUNT "<<NOM_SUBARRAY_COUNT<<std::endl;
  //   // std::cout<<"WAVE_ARRAY_1 "<<WAVE_ARRAY_1<<std::endl;
  //   // std::cout<<"WAVE_ARRAY_2 "<<WAVE_ARRAY_2<<std::endl;
  //   // std::cout<<"COMM_TYPE "<<COMM_TYPE<<std::endl;

  //   TRIGTIME_ARRAY += 16*n;
  //   WAVE_ARRAY_COUNT += n*( WAVE_ARRAY_COUNT/NOM_SUBARRAY_COUNT) ;
  //   NOM_SUBARRAY_COUNT += n;//space for one
 
  //   WAVE_ARRAY_1 = WAVE_ARRAY_COUNT*(COMM_TYPE+1); //+
  //   if (WAVE_ARRAY_2>0) WAVE_ARRAY_2 = WAVE_ARRAY_1; //+

  //   set_byte_pos();
  //   // std::cout<<"TRIGTIME_ARRAY "<<TRIGTIME_ARRAY<<std::endl;
  //   //   std::cout<<"WAVE_ARRAY_COUNT "<<WAVE_ARRAY_COUNT<<std::endl;
  //   //   std::cout<<"NOM_SUBARRAY_COUNT "<<NOM_SUBARRAY_COUNT<<std::endl;
  //   //   std::cout<<"WAVE_ARRAY_1 "<<WAVE_ARRAY_1<<std::endl;
  //   //   std::cout<<"WAVE_ARRAY_2 "<<WAVE_ARRAY_2<<std::endl;
  //   //
  // }

  // void ICR::lecroy::lecroy_header::resize(const int n)
  // {  
  //   //set the appropriate variables for the new size
  //   TRIGTIME_ARRAY = 16*n;
  //   WAVE_ARRAY_COUNT = n*( WAVE_ARRAY_COUNT/NOM_SUBARRAY_COUNT) ;
  //   NOM_SUBARRAY_COUNT =n;//space for one
  
  //   WAVE_ARRAY_1 = WAVE_ARRAY_COUNT*(COMM_TYPE+1);
  //   if (WAVE_ARRAY_2>0) WAVE_ARRAY_2 = WAVE_ARRAY_1;

  //   set_byte_pos();
 
  // }

  // //clear the descriptor of its size characteristics only
  // void ICR::lecroy::lecroy_header::clear(void){
  //   byte.begin = 0;
  
  //   TRIGTIME_ARRAY = 16;
  //   RIS_TIME_ARRAY = 0;
  
  //   WAVE_ARRAY_COUNT = WAVE_ARRAY_COUNT/ NOM_SUBARRAY_COUNT;
  //   //WAVE_ARRAY_COUNT = WAVE_DESCRIPTOR/ NOM_SUBARRAY_COUNT;
  //   WAVE_ARRAY_1 = WAVE_ARRAY_COUNT;
  //   if (WAVE_ARRAY_2>0) WAVE_ARRAY_2 = WAVE_ARRAY_1;
  //   NOM_SUBARRAY_COUNT =1;//space for one
  
  //   set_byte_pos();
  //   //   std::cout<<"byte.preamble "<<byte.preamble<<std::endl;
  //   //   std::cout<<"byte.trig "<<byte.trig<<std::endl;
  //   //   std::cout<<"byte.ris  "<<byte.ris <<std::endl;
  //   //   std::cout<<"byte.array_1 "<<byte.array_1<<std::endl;
  //   //   std::cout<<"byte.array_2 "<<byte.array_2<<std::endl;
  // };




  void
  ICR::lecroy::lecroy_header::set_byte_pos(void)
  {
    byte.preamble    = begin_byte()    + WAVE_DESCRIPTOR;
    byte.trig        = preamble_byte() + USER_TEXT;
    byte.ris         = trig_byte()     + TRIGTIME_ARRAY ;
    byte.array_1     = ris_byte()      + RIS_TIME_ARRAY;
  
    byte.array_2     = array_1_byte()  + WAVE_ARRAY_1;

  }

//   void
//   ICR::lecroy::lecroy_header::set_trigger(unsigned long n, double intv, double off)
//   {
//     std::pair<double,double> tmp(intv,off);
//     trig_times[n] = tmp;
//   }

// void
// ICR::lecroy::lecroy_header::set_gain( double zintv, double zoff)
// {
//   VERTICAL_GAIN   = zintv;
//     VERTICAL_OFFSET = zoff;
//   }

// vec 
// ICR::lecroy::lecroy_header::get_times( ) const
// {
//   double trace_size = WAVE_ARRAY_COUNT/NOM_SUBARRAY_COUNT;
  
//   vec data;
//   data.set_size(trace_size);
//   for(size_t i=0;i<trace_size;++i){
//     data[i] = i*HORIZ_INTERVAL + HORIZ_OFFSET;
//   }
//   return data;

// }


// vec 
// ICR::lecroy::lecroy_header::get_trace( size_t i ) const
// {
//   input_binary IN(m_filename);
//   long trace_size = WAVE_ARRAY_COUNT/NOM_SUBARRAY_COUNT;
//   IN->seekg (array_1_byte()+ i *trace_size , std::ios::beg);
//   char* buffer = new char[trace_size];
//   IN->read (buffer,trace_size); 

//   //convert buffer to double.

//   vec data;
//   data.set_size(trace_size);
//   for(size_t i=0;i<trace_size;++i){
//     data[i] = ((double) buffer[i])*VERTICAL_GAIN+VERTICAL_OFFSET;
//     // data[i] *= VERTICAL_GAIN;
//     // data[i] += VERTICAL_OFFSET;
//   }
//   delete[] buffer;

//   return data;

// }




// // ICR::lecroy::lecroy_header_traces::lecroy_traces(const lecroy lcy)
// //   : m_times(), m_traces()
// // {
// //   push_back(lcy.get_filename());
// // }

 
// ICR::lecroy::lecroy_header_traces::lecroy_traces(const string filename)
//   : m_times(), m_traces()
// {
//   push_back(filename);
// }

// void 
// ICR::lecroy::lecroy_header_traces::push_back(const string filename)
// {
//   lecroy l(filename);
//   m_times = l.get_times();
//   for(size_t i=0;i<l.SUBARRAY_COUNT;++i){
//     m_traces.push_back(l.get_trace(i));
//   }
  
  
// }

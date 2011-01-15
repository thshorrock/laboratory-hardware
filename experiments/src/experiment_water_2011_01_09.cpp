#include "lecroy.hpp"
#include "agilent.hpp"
#include "DPR500.hpp"
#include "filename.hpp"
#include "readme.hpp"
#include "stringify.hpp"
#include "boost/progress.hpp"
#include <sstream>


#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
using namespace ICR;
using namespace ICR::lecroy;
using namespace ICR::pulser;
using namespace ICR::agilent;


struct description{
  std::string intro;
  std::string setup;
  std::string exp_proceedures;
  description()
    : intro(),setup(), exp_proceedures()
  { 
    intro="LEVOVIST EXPERIMENT 17ml and 4 g - 4ml -2011-01-09 - This time with the imaging wave on the second pulse PRE taken at 6 V";
    std::stringstream setup_stream;
    setup_stream
      <<"The following equipment is used. \n"
      << "  - A computer,\n"
      << "  - An Agilent 33120A,\n"
      << "  - An agilent 33220A,\n"
      << "  - Two thundar TGP110,\n"
      << "  - A Lecroy Waverunner 64Xi\n"
      << "  - A DPR500 pulser with an PRL2 pulser-receiver,\n"
	    
      <<"The Agilent 33120A is controlled by serial connection.  Its role is to trigger the rest of the equipment when commanded to by the computer.  The two Thundars trigger in are connected to the sync of the 33120A.\n\n"
      <<"The first Thundar is set on double pulse mode.  Its role is to trigger the two low frequency driving pulses.  The output is connecteced to channel 1 of the scope (used for triggering), the gate of the Tomco Poweramp and the trigger in of the Agilent 33220A.\n\n"
      <<"The second Thundar set to delayed pulse. Its rold is to trigger the imaginging wave. Its output goes to the trigger in of the DPR500 and to channel 3 of the scope (for triggering wiht the imaging wave).\n\n"
      <<"The Agilent 33220A is controlled by LAN connection.  Its role is to produce the driving wave signal.  Its output goes to the RF input of the Tomco power amp.\n\n"
      <<"The DPR500 is controlled via serial connection.  It controls the imaging spike.\n\n"
      <<"The scope is controlled by LAN. Waveforms are grabbed directly from the scope.\n\n"
      <<"There is a single sheet of plastic film within acoustic window.  This is at the far end of the imaging waves focal region (in fact, a little beyond this).  This plastic sheet enables the pulse echo time of the imaging wave (when triggered with the imaging trigger) to be mapped onto the pulse echo time of the driving wave (as triggerred with the driving trigger).\n\n "
      <<"The water tap water that has been left to stand for several days";
    setup=setup_stream.str();

    std::stringstream exp_stream;
    exp_stream
      <<"---------------------------------------------------------\n"
      <<"Before the experiment begins the pulse-echo time of the imaging transducer to the plastic film is measured.  This is done by using channel 3 as the trigger.  The 33200A is turned off for this experiment. The repetitions are placed in the 'imaging_wave_only' folder.\n\n"
      <<"Next the driving wave is turned on at a low amplitude and imaged with a 10kHz high pass filter.  This enables the phase of the driving wave to be measured.  While the bandwidth of the imaging tranducer should be taken into account - sufficient flow frequency signal is present for the results to be meaningful.  The repetitions are placed in the 'phase_profile' folder. \n\n"
      <<"Finally the experiment in ernest begins. The voltage applied to the driving wave is varied.  The results for every voltage is placed in its own folder which is labelled accordingly\n\n"
      <<"---------------------------------------------------------";
    exp_proceedures = exp_stream.str();

  }
      
      
};

double driving_voltage = 20e-3;

double driving_min_voltage = 20e-3;
double driving_max_voltage = 0.3;
bool failed =false;

struct equipment{
  //The equipment
  lecroy_64Xi            scope;
  WG33120A               bus_sync;
  WG33220A<agilent::IP>  driving_pulse;
  DPR500                 pulser;
  PulserReceiverFactory  factory; 
  RPL2*                  imaging_pulse;

  //The Important variables in this experiment;
  double lecroy_timebase;
  double lecroy_trigger_delay;
  double lecroy_volts_per_div;
  unsigned long lecroy_samples;
  double lecroy_alines;
  
  //double driving_voltage;
  double driving_voltage_steps;
  double driving_phase;
  double driving_frequency;
  double driving_offset;
  unsigned int driving_cycles;
  
  
  double imaging_voltage;
  int    imaging_gain;
  
  unsigned int experimental_repeats;

  //default setup
  equipment()
    : scope("10.0.0.10"),
      bus_sync("/dev/ttyUSB0"),
      driving_pulse("10.0.0.20"),
      pulser("/dev/ttyUSB1"),
      factory(),
      imaging_pulse(),
      //Lecroy Defaults variables
      lecroy_timebase(10e-6),
      lecroy_trigger_delay(-40e-6),
      lecroy_volts_per_div(0.6),
      lecroy_samples(250000),
      lecroy_alines(1),
      //Driving Defaults
      driving_voltage_steps(100),
      driving_phase(0),
      driving_frequency(0.5e6),
      driving_offset(0),
      driving_cycles(10),
      //imaging defualts
      imaging_voltage(300),
      imaging_gain(50),
      //misc
      experimental_repeats(25)
  {
    //Commit these changes.
    
    RPL2* imaging_pulse = factory.make_RPL2();
   
    //SETUP
    //BUS
    bus_sync.shape(shape::SQUARE);
    bus_sync.trigger(trigger::BUS);
    bus_sync.burst_on();
    
    //DRIVING PULSE
    driving_pulse.turn_off();
    driving_pulse.shape(shape::SIN);
    driving_pulse.frequency(driving_frequency);
    driving_pulse.burst_ext(edge::POSITIVE);
    driving_pulse.cycles(driving_cycles);
    driving_pulse.phase(driving_phase);
    driving_pulse.voltage(driving_voltage); 
    driving_pulse.offset(driving_offset);
    driving_pulse.burst_on();

    //IMAGING PULSE
    pulser.blink(200);  
    imaging_pulse->set_external_trigger();
    imaging_pulse->set_pulser_mode(damping_RPL2::fourtyfour_Ohm,
				   energy_policy::HIGH,
				   receive_mode::PULSE_ECHO);
    imaging_pulse->set_voltage(imaging_voltage);
    pulser.attach_A(imaging_pulse);
    pulser.notify();
    //)  filter::HPFA::   ten_kHz    sevenpointfive_MHz
    pulser.set_high_pass_filter_A(filter::HPFA::sevenpointfive_MHz);
    pulser.set_low_pass_filter_A(filter::LPFA::fifty_MHz);
    pulser.set_gain_A(imaging_gain);
  
    pulser.notify();
    pulser.turn_on_A();

    std::cout<<"ON"<<std::endl;


    //LECROY SETUP
    //scope.calibrate() ; //calibrate now
    scope.auto_calibrate(false); //turn calibration off
    scope.set_date();
    scope.trigger_mode(trigger_mode::NORM);
    scope.trace_off(location::C1);
    scope.trace_off(location::C2);
    scope.trace_on(location::C3);
    scope.trace_on(location::C4);

    scope.set_coupling(location::C1, coupling::D50);
    scope.set_coupling(location::C3, coupling::D50);
    scope.set_coupling(location::C4, coupling::D50);

    scope.trigger_level(location::C1, 1.0);
    scope.trigger_level(location::C3, 1.0);
    scope.trigger_coupling(location::C1, coupling::DC);
    scope.trigger_coupling(location::C3, coupling::DC);
    scope.trigger_positive_edge(location::C1);
    scope.trigger_positive_edge(location::C3);
    scope.trigger_select(trigger_type::EDGE,location::C3,trigger_hold_type::NO_HOLD);

    scope.trigger_delay(lecroy_trigger_delay);
    scope.set_timebase(lecroy_timebase);
    scope.volts_per_div(location::C1,1.0);
    scope.volts_per_div(location::C3,1.0);
    scope.volts_per_div(location::C4,lecroy_volts_per_div);
    scope.sequence(lecroy_alines,lecroy_samples);

  }
  //clean up
  ~equipment()
  {
    pulser.turn_off_A();  //make sure the pulser perminantely running
    scope.auto_calibrate(true); //turn calibration on as people expect this to be set.
  }
  
};

int
main  (int ac, char **av)
{
  
  //wile( driving_voltage<driving_max_voltage) {

    std::cout<<"driving voltage = "<<driving_voltage<<std::endl;
    ///ry{

      //loadup descriptions
      description desc;
      //loadup equipment
      equipment eq;
      
      ICR::directory dir("/home/tommy/2011/01/09/levovist/second");  //the current directory

      if (!failed){ 

	//PRE EXPERIMENT
  

	/* We want to take an image as seen by the imaging transducer so that we can compare 
	 * the recoded time (as recorded from the driving transducer)
	 * with the depth from the imaging transducer.
	 * 
	 * equipment defaults for this experiment.
	 *
	 */

	//create a new directory for this prerecord
	std::string subdir = "/imaging_wave_only";
	std::cout<<"subdir = "<< dir.get_directory()+subdir <<std::endl;

	dir.mkdir(dir.get_directory()+subdir); //create the subdirectory
	dir.pushd(dir.get_directory()+subdir);  //goto subdirectory

	readme README_IMAG(dir.get_directory());
	README_IMAG
	  (desc.intro)
	  (desc.setup)
	  (desc.exp_proceedures)
	  ("-----------------------")
	  ("IMAGING WAVE SOLO")
	  ("-----------------------") 
	  ("BUS pulse details")
	  ("-----------------------") 
	  ("gate (approx) (us) ", 20)
	  ("time between driving gates (us)", 150.35)
	  ("-----------------------")
	  ("Driving pulse details")
	  ("-----------------------")
	  ("driving pulse","OFF")
	  ("-----------------------")
	  ("Imaging pulse details")
	  ("-----------------------")
	  ("Imaging Receiver", "RPL2")
	  ("Imaging damping", "damping_RPL2::fourtyfour_Ohm")
	  ("Imaging energy_policy", "HIGH")
	  ("Imaging receive_mode","PULSE_ECHO")
	  ("Imaging voltage (need to check no bug here)",eq.imaging_voltage)
	  ("imaging gain", eq.imaging_gain)
	  ("Imaging high pass filter","sevenpointfive")
	  ("Imaging low pass filter", "fifty_MHz")   
	  ("-----------------------")   
	  ("Lecroy setup")
	  ("-----------------------")
	  ("Lecroy sequence", eq.lecroy_alines)
	  ("Lecroy samples ", eq.lecroy_samples)
	  ("Lecroy timebase",eq.lecroy_timebase )
	  ("Lecroy trigger_delay", eq.lecroy_trigger_delay)
	  ("Lecroy volts per div", eq.lecroy_volts_per_div)
	  ("-----------------------")   
	  ("MISC")
	  ("-----------------------")
	  ("Repeats", eq.experimental_repeats)
	  ;
  
	boost::progress_display pd_imag(eq.experimental_repeats);
	for(size_t i=0;i<eq.experimental_repeats;++i){
	  eq.bus_sync.trigger_now();
	  lecroy_file file =  eq.scope.get_waveform(location::C4);

	  aline a = file.get_data1();

	  std::string index = stringify_with_zeros(i,2); //pad the index with zeros
      
	  ICR::file Filename(dir,"imaging_solo_"+index+".dat");
	  ICR::file Filename_GP(dir,"imaging_solo_"+index+"_GP.dat");
      
	  //save alines;
	  ICR::lecroy::save(a, Filename.full()    );
	  //save gnuplot
	  ICR::lecroy::save_gnuplot(a,   Filename_GP.full());
	  ++pd_imag;
	}
	dir.popd(); //leave the subdir.

	//---------------------

	/* Next we want to pulse with the driving wave (at a low voltage)
	 * with an (almost) pass all filter.  
	 * This lets us see the phasing of the driving wave.
	 * (strictly need to worry about the varying frequency response of driving transducer)
	 *
	 */
	//change filter
	eq.pulser.set_high_pass_filter_A(filter::HPFA::ten_kHz);
	eq.pulser.notify();

	//create a new directory for this prerecord
	subdir = "/phase_profile";
	dir.mkdir(dir.get_directory()+subdir); //create the subdirectory
	dir.pushd(dir.get_directory()+subdir);  //goto subdirectory

	readme README_PHASE(dir.get_directory());
	README_PHASE
	  (desc.intro)
	  (desc.setup)
	  (desc.exp_proceedures)
	  ("-----------------------")
	  ("IMAGING WAVE SOLO")
	  ("-----------------------") 
	  ("BUS pulse details")
	  ("-----------------------") 
	  ("gate (approx) (us) ", 20)
	  ("time between driving gates (us)", 150.35)
	  ("-----------------------")
	  ("Driving pulse details")
	  ("-----------------------")
	  ("driving voltage",  driving_voltage)
	  ("driving min voltage",  driving_min_voltage)
	  ("driving max voltage",  driving_max_voltage)
	  ("driving voltage steps",  eq.driving_voltage_steps)
	  ("driving phase", eq.driving_phase)
	  ("driving frequency", eq.driving_frequency)
	  ("driving offset", eq.driving_offset)
	  ("driving cycles", eq.driving_cycles)
	  ("-----------------------")
	  ("Imaging pulse details")
	  ("-----------------------")
	  ("Imaging Receiver", "RPL2")
	  ("Imaging damping", "damping_RPL2::fourtyfour_Ohm")
	  ("Imaging energy_policy", "HIGH")
	  ("Imaging receive_mode","PULSE_ECHO")
	  ("Imaging voltage (need to check no bug here)",eq.imaging_voltage)
	  ("imaging gain", eq.imaging_gain)
	  ("Imaging high pass filter","ten_kHz")
	  ("Imaging low pass filter", "fifty_MHz")   
	  ("-----------------------")   
	  ("Lecroy setup")
	  ("-----------------------")
	  ("Lecroy sequence", eq.lecroy_alines)
	  ("Lecroy samples ", eq.lecroy_samples)
	  ("Lecroy timebase",eq.lecroy_timebase )
	  ("Lecroy trigger_delay", eq.lecroy_trigger_delay)
	  ("Lecroy volts per div", eq.lecroy_volts_per_div)
	  ("-----------------------")   
	  ("MISC")
	  ("-----------------------")
	  ("Repeats", eq.experimental_repeats)
	  ;
  
	boost::progress_display pd_phase(eq.experimental_repeats);
	for(size_t i=0;i<eq.experimental_repeats;++i){
	  eq.bus_sync.trigger_now();
	  lecroy_file file =  eq.scope.get_waveform(location::C4);

	  aline a = file.get_data1();

	  std::string index = stringify_with_zeros(i,2); //pad the index with zeros
      
	  ICR::file Filename(dir,"phase_profile_off_"+index+".dat");
	  ICR::file Filename_GP(dir,"phase_profile_off_"+index+"_GP.dat");
      
	  //save alines;
	  ICR::lecroy::save(a, Filename.full()    );
	  //save gnuplot
	  ICR::lecroy::save_gnuplot(a,   Filename_GP.full());
	  ++pd_phase;
	}
	//turn on driving pulse
	eq.driving_pulse.turn_on();

	boost::progress_display pd_phase2(eq.experimental_repeats);
	for(size_t i=0;i<eq.experimental_repeats;++i){
	  eq.bus_sync.trigger_now();
	  lecroy_file file =  eq.scope.get_waveform(location::C4);

	  aline a = file.get_data1();

	  std::string index = stringify_with_zeros(i,2); //pad the index with zeros
      
	  ICR::file Filename(dir,"phase_profile_on_"+index+".dat");
	  ICR::file Filename_GP(dir,"phase_profile_on_"+index+"_GP.dat");
      
	  //save alines;
	  ICR::lecroy::save(a, Filename.full()    );
	  //save gnuplot
	  ICR::lecroy::save_gnuplot(a,   Filename_GP.full());
	  ++pd_phase2;
	}
	dir.popd(); //leave the subdir.
	//-------------------------------------------------------------
      }

	eq.driving_pulse.turn_on();
      //change the filter back
      eq.pulser.set_high_pass_filter_A(filter::HPFA::sevenpointfive_MHz);
      eq.pulser.notify();
  
      //change lecroy setup
      eq.lecroy_alines=2;
      eq.lecroy_trigger_delay=-68e-6;
      eq.lecroy_timebase=(5e-6);
      eq.scope.trace_off(location::C3);
      eq.scope.trigger_delay(eq.lecroy_trigger_delay);
      eq.scope.set_timebase(eq.lecroy_timebase);
      eq.scope.trigger_select(trigger_type::EDGE,location::C1,trigger_hold_type::NO_HOLD);
      eq.scope.sequence(eq.lecroy_alines,eq.lecroy_samples);
  
      eq.lecroy_volts_per_div = 30e-3;
      eq.scope.volts_per_div(location::C4,eq.lecroy_volts_per_div);
      //EXPERIMENT
      double voltage_intv = (driving_max_voltage-driving_min_voltage)/(eq.driving_voltage_steps);
      //for(int i=0;i<eq.driving_voltage_steps;++i){
      while (driving_voltage< driving_max_voltage) {
	driving_voltage +=voltage_intv;
	if (driving_voltage>driving_max_voltage)  break;
    
	//set the voltage
	eq.driving_pulse.voltage( driving_voltage );

	boost::progress_display pd_exp(eq.experimental_repeats);
	//create a new directory for each set of repeats
	std::string subdir = "/voltage_"+stringify(driving_voltage);
	dir.mkdir(dir.get_directory()+subdir); //create the subdirectory
	dir.pushd(dir.get_directory()+subdir); //goto subdirectory
    
	//create a readme file in the directory.
	readme README_EXP(dir.get_directory());
	README_EXP
	  (desc.intro)
	  (desc.setup)
	  (desc.exp_proceedures)
	  ("-----------------------")
	  ("IMAGING WAVE SOLO")
	  ("-----------------------") 
	  ("BUS pulse details")
	  ("-----------------------") 
	  ("gate (approx) (us) ", 20)
	  ("time between driving gates (us)", 150.35)
	  ("-----------------------")
	  ("Driving pulse details")
	  ("-----------------------")
	  ("driving voltage",  driving_voltage)
	  ("driving min voltage",  driving_min_voltage)
	  ("driving max voltage",  driving_max_voltage)
	  ("driving voltage steps",  eq.driving_voltage_steps)
	  ("driving phase", eq.driving_phase)
	  ("driving frequency", eq.driving_frequency)
	  ("driving offset", eq.driving_offset)
	  ("driving cycles", eq.driving_cycles)
	  ("-----------------------")
	  ("Imaging pulse details")
	  ("-----------------------")
	  ("Imaging Receiver", "RPL2")
	  ("Imaging damping", "damping_RPL2::fourtyfour_Ohm")
	  ("Imaging energy_policy", "HIGH")
	  ("Imaging receive_mode","PULSE_ECHO")
	  ("Imaging voltage (need to check no bug here)",eq.imaging_voltage)
	  ("imaging gain", eq.imaging_gain)
	  ("Imaging high pass filter","sevenpointfive_MHz")
	  ("Imaging low pass filter", "fifty_MHz")   
	  ("-----------------------")   
	  ("Lecroy setup")
	  ("-----------------------")
	  ("Lecroy sequence", eq.lecroy_alines)
	  ("Lecroy samples ", eq.lecroy_samples)
	  ("Lecroy timebase",eq.lecroy_timebase )
	  ("Lecroy trigger_delay", eq.lecroy_trigger_delay)
	  ("Lecroy volts per div", eq.lecroy_volts_per_div)
	  ("-----------------------")   
	  ("MISC")
	  ("-----------------------")
	  ("Repeats", eq.experimental_repeats)
	  ;
    
	// eq.bus_sync.trigger_now(); 
	//repeat the experiment a few times.
	for(int i=0;i<eq.experimental_repeats;++i){
      
	  // eq.scope.trigger_mode(trigger_mode::SINGLE);
	  //Trigger	
	  eq.bus_sync.trigger_now(); 
	  lecroy_file file;
	  bool collected=false;
	  while(!collected){
	    try{
	      file =  eq.scope.get_waveform(location::C4);
	      collected=true;
	    } catch (ICR::exception::could_not_get_fresh_aquisition& e) {
	      std::cout<<"failed to trigger retry"<<std::endl;
	      eq.bus_sync.trigger_now();
	    } catch(exception::timeout_exceeded& e){
	      e.debug_print();
	      std::cout<<"retrigger retry"<<std::endl;
	  
	      eq.bus_sync.trigger_now();
	      eq.bus_sync.trigger_now();
	      eq.bus_sync.trigger_now();
	    }
	  }
	
      
	  aline without_imaging_pulse = file.get_data1(0); // the first aline
	  aline with_imaging_pulse    = file.get_data1(1); // the second aline
	  aline excess_pulse          = with_imaging_pulse-without_imaging_pulse;

	  std::string index = stringify_with_zeros(i,4); //pad the index with zeros
      
	  ICR::file WithFilename   (dir,"with_imaging_pulse_"+index+".dat");
	  ICR::file WithoutFilename(dir,"without_imaging_pulse_"+index+".dat");
	  ICR::file ExcessFilename (dir,"excess_pulse_"+index+".dat");
      
	  ICR::file WithGnuplotFilename   (dir,"with_imaging_pulse_"+index+"_GP.dat");
	  ICR::file WithoutGnuplotFilename(dir,"without_imaging_pulse_"+index+"_GP.dat");
	  ICR::file ExcessGnuplotFilename (dir,"excess_pulse_"+index+"_GP.dat");
      

	  //save alines;
	  ICR::lecroy::save(with_imaging_pulse,   WithFilename.full()    );
	  ICR::lecroy::save(without_imaging_pulse,WithoutFilename.full() );
	  ICR::lecroy::save(excess_pulse,ExcessFilename.full()  );
	  //save gnuplot
	  // ICR::lecroy::save_gnuplot(with_imaging_pulse,   WithGnuplotFilename.full()    );
	  // ICR::lecroy::save_gnuplot(without_imaging_pulse,WithoutGnuplotFilename.full() );
	  // ICR::lecroy::save_gnuplot(excess_pulse,ExcessGnuplotFilename.full()  );
	  ++pd_exp;
	}
	dir.popd(); //leave the subdir.
      }
  
      
    // } catch (...) {
    //   failed =true;
    //   //catch everything - the experiment MUST finnish!
    //   std::cout<<"something went wrong restarting equipment"<<std::endl;
    // }
    //
}

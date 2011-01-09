#include "lecroy.hpp"
#include "agilent.hpp"
#include "DPR500.hpp"
#include "filename.hpp"
#include "readme.hpp"
#include "stringify.hpp"
#include "boost/progress.hpp"


#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>
using namespace ICR;
using namespace ICR::lecroy;
using namespace ICR::pulser;
using namespace ICR::agilent;




// double pulse_offset(double imaging_echo_time_to_rear,
// 		    double driving_echo_time_to_front,
// 		    double duration_of_driving_wave,
// 		    double time_between_pulse_pair)
// {
//   //want imaging wave to coincide with rear of driving wave
  
//   //imaging 1 way time
//   double imag_time = imaging_echo_time_to_rear/2.0;
  
//   //driving 1 way time to rear
//   double driving_time = (driving_echo_time_to_front/2.0)
//     + duration_of_driving_wave;
  
//   std::cout<<"driving time = "<<driving_time<<std::endl;


  
//   //difference in time 
//   //(driving wave first so should be negative)
//   double delta_time = imag_time-driving_time;
//   std::cout<<"delta time = "<<delta_time<<std::endl;

//   double offset = time_between_pulse_pair-delta_time;
  
//   std::cout<<"offset = "<<offset<<std::endl;

//   // std::cout<<"scope offset = "<<imag_time+driving_time<std::endl;


//   return offset;
// }



int
main  (int ac, char **av)
{
  


  //Equipment
  lecroy_64Xi     scope("10.0.0.10");
  WG33120A       bus_sync("/dev/ttyUSB0");
  WG33220A<agilent::IP>  driving_pulse("10.0.0.20");
  DPR500 pulser("/dev/ttyUSB1");

  PulserReceiverFactory factory;
   RPL2* imaging_pulse = factory.make_RPL2();

  //SETUP
  
  //BUS
  bus_sync.shape(shape::SQUARE);
  // bus_sync.trigger(trigger::BUS);
  // bus_sync.burst_on();

  //DRIVING PULSE
  int driving_cycles = 10;
  double driving_frequency = 0.5e6;
  double driving_offset = 0.0 ;
  double driving_voltage = 0.01;
  double driving_phase = 0.0;

  driving_pulse.shape(shape::SIN);
  driving_pulse.frequency(driving_frequency);
  driving_pulse.burst_ext(edge::POSITIVE);
  driving_pulse.cycles(driving_cycles);
  driving_pulse.phase(driving_phase);
  driving_pulse.voltage(driving_voltage);  //range from 0.2 to 0.35
  driving_pulse.offset(driving_offset);
  driving_pulse.burst_on();
  
  //PULSER
  double imaging_voltage = 275;
  int    imaging_gain = 50;

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

  int lecroy_samples=100000;
  double lecroy_timebase=10e-6;
  double trigger_delay = -40.0e-6 ;
  scope.set_date();
  scope.sequence(2,lecroy_samples);
  scope.trigger_mode(trigger_mode::NORM);

  

  //EXPERIMENT RANGES
  double voltage_min = 0.01;
  double voltage_max = 0.40;
  int steps = 50;
  
  int repeats= 12;
    
  ICR::directory dir("/data/2011/01/07/");  //the current directory
  double voltage_intv = (voltage_max-voltage_min)/(steps);

  //PRE EXPERIMENT
  //scope.calibrate() ; //calibrate now
  scope.auto_calibrate(false); //turn calibration off
  

  /* We want to take an image as seen by the imaging transducer so that we can compare 
   * the recoded time (as recorded from the driving transducer)
   * with the depth from the imaging transducer.
   * 
   * The imaging pulse is on channel 3
   * and so we first set the trigger to be from that channel 
   *
   *
   */
  lecroy_timebase=10e-6;
  trigger_delay = -40.0e-6 ;
  scope.trace_on(location::C3);
  scope.trigger_coupling(location::C3, coupling::DC);
  scope.trigger_positive_edge(location::C3);
  scope.trigger_select(trigger_type::EDGE,location::C3,trigger_hold_type::NO_HOLD);
  scope.trigger_delay(trigger_delay);
  scope.set_timebase(lecroy_timebase);
  scope.sequence(1,lecroy_samples);

  //turn off driving pulse
  driving_pulse.turn_off();

  //create a new directory for this prerecord
  std::string subdir = "./triggering_on_imaging_pulse"+stringify(driving_voltage);
  dir.mkdir(subdir); //create the subdirectory
  dir.pushd(subdir);  //goto subdirectory

  readme README(dir.get_directory());
  README
    ("Experiment varying the driving voltage to cavitating water.")
    ("Triggering on imaging receive")
    //-----------------------
    ("bus pulse details")
    //-----------------------
    ("Bus sync pulser details")
    ("gate (approx) (us) ", 20)
    ("time between driving gates (us)", 119.98)
    //-----------------------
    ("Driving pulse details")
    //-----------------------
    ("driving pulse","OFF")
    //-----------------------
    ("Imaging pulse details")
    //-----------------------
    ("Imaging Receiver", "RPL2")
    ("Imaging damping", "damping_RPL2::fourtyfour_Ohm")
    ("Imaging energy_policy", "HIGH")
    ("Imaging receive_mode","PULSE_ECHO")
    ("Imaging voltage (need to check no bug here)",imaging_voltage)
    ("imaging gain", imaging_gain)
    ("Imaging high pass filter","sevenpointfive")
    ("Imaging low pass filter", "fifty_MHz")   
    //-----------------------   
    ("Lecroy setup")
    //-----------------------
    ("Lecroy sequence", 2)
    ("Lecroy samples ", lecroy_samples)
    ("Lecroy timebase",lecroy_timebase )
    ("Lecroy trigger_delay", trigger_delay)
    ;
  
  for(size_t i=0;i<repeats;++i){
    bus_sync.trigger_now();
    lecroy_file file =  scope.get_waveform(location::C4);

    aline a = file.get_data1();

    std::string index = stringify_with_zeros(i,2); //pad the index with zeros
      
    ICR::file Filename(dir,"imaging_solo"+index+".dat");
    ICR::file Filename_GP(dir,"imaging_solo"+index+"_GP.dat");
      
    //save alines;
    ICR::lecroy::save(a, Filename.full()    );
    //save gnuplot
    ICR::lecroy::save_gnuplot(a,   Filename_GP.full());
  }

    dir.popd(); //leave the subdir.

  //turn off driving pulse
  driving_pulse.turn_on();


  // //EXPERIMENT
  // for(int i=0;i<steps;++i){
  //   double driving_voltage = voltage_min + i * voltage_intv;
  //   boost::progress_display pd(repeats);

  //   driving_pulse.voltage( driving_voltage );

  //   //create a new directory for each set of repeats
  //   std::string subdir = "./voltage_"+stringify(driving_voltage);
  //   dir.mkdir(subdir); //create the subdirectory
  //   dir.pushd(subdir);  //goto subdirectory
    
  //   //create a readme file in the directory.
  //   readme README(dir.get_directory());
  //   README
  //     ("Experiment varying the driving voltage to cavitating water.")
  //     //bus_sync details
  //     ("Bus sync pulser details")
  //     ("gate (approx) (us) ", 20)
  //     ("time between driving gates (us)", 119.98)
  //     //-----------------------
  //     ("Driving pulse details")
  //     //-----------------------
  //     ("driving generator", "AGILENT 33220A")
  //     ("Attached to", "A")
  //     ("driving Shape", "SIN")
  //     ("driving frequency", driving_frequency)
  //     ("driving burst edge", "POSITIVE")
  //     ("driving cycles", driving_cycles)
  //     ("driving phase", driving_phase)
  //     ("driving voltage", driving_voltage)
  //     //-----------------------
  //     ("Imaging pulse details")
  //     //-----------------------
  //     ("Imaging Receiver", "RPL2")
  //     ("Imaging damping", "damping_RPL2::fourtyfour_Ohm")
  //     ("Imaging energy_policy", "HIGH")
  //     ("Imaging receive_mode","PULSE_ECHO")
  //     ("Imaging voltage (need to check no bug here)",imaging_voltage)
  //     ("imaging gain", imaging_gain)
  //     ("Imaging high pass filter","sevenpointfive")
  //     ("Imaging low pass filter", "fifty_MHz")   
  //     //-----------------------   
  //     ("Lecroy setup")
  //     //-----------------------
  //     ("Lecroy sequence", 2)
  //     ("Lecroy samples ", lecroy_samples)
  //     ("Lecroy timebase", -76.0)
  //     ;
    
  //   //repeat the experiment a few times.
  //   for(int i=0;i<repeats;++i){
      
  ////Trigger	
  // //bus_sync.trigger_now();
  //     lecroy_file file =  scope.get_waveform(location::C4);

  //     aline without_imaging_pulse = file.get_data1(0); // the first aline
  //     aline with_imaging_pulse    = file.get_data1(1); // the first aline
  //     aline excess_pulse          = with_imaging_pulse-without_imaging_pulse;

  //     std::string index = stringify_with_zeros(i,4); //pad the index with zeros
      
  //     ICR::file WithFilename   (dir,"with_imaging_pulse_"+index+".dat");
  //     ICR::file WithoutFilename(dir,"without_imaging_pulse_"+index+".dat");
  //     ICR::file ExcessFilename (dir,"excess_pulse_"+index+".dat");
      
  //     ICR::file WithGnuplotFilename   (dir,"with_imaging_pulse_"+index+"_GP.dat");
  //     ICR::file WithoutGnuplotFilename(dir,"without_imaging_pulse_"+index+"_GP.dat");
  //     ICR::file ExcessGnuplotFilename (dir,"excess_pulse_"+index+"_GP.dat");
      

  //     //save alines;
  //     ICR::lecroy::save(with_imaging_pulse,   WithFilename.full()    );
  //     ICR::lecroy::save(without_imaging_pulse,WithoutFilename.full() );
  //     ICR::lecroy::save(without_imaging_pulse,ExcessFilename.full()  );
  //     //save gnuplot
  //     ICR::lecroy::save_gnuplot(with_imaging_pulse,   WithGnuplotFilename.full()    );
  //     ICR::lecroy::save_gnuplot(without_imaging_pulse,WithoutGnuplotFilename.full() );
  //     ICR::lecroy::save_gnuplot(without_imaging_pulse,ExcessGnuplotFilename.full()  );
  //     ++pd;
  //   }
  //   dir.popd(); //leave the subdir.
  // }
  
  ICR::coms::sleep (5000);
  pulser.turn_off_A();
  std::cout<<"OFF"<<std::endl;

  scope.auto_calibrate(true); //turn calibration on as people expect this to be set.
}

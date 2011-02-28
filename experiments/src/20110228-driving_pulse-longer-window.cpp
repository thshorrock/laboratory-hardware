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
    intro="Water experiment";
    std::stringstream setup_stream;
    setup_stream
      <<"The following equipment is used. \n"
      << "  - A computer,\n"
      << "  - An agilent 33220A,\n"
      << "  - The analogic 2045"
      << "  - A Lecroy Waverunner 64Xi\n"
      << "  - A DPR500 pulser with an PRL2 pulser-receiver,\n"	    
      <<"The Agilent 33220A is controlled by LAN connection.  Its role is trigged by the computer to produce a sine burst of 10 cycles.  The gate to the tomco and the analogic is conneted to the sync out.  The output is connected to the RF in on the Tomco\n\n"
      <<"The analogic is to provide a delayed pulse for the DPR500.  The reason this pulser is used rather than the Thandars or the agilent is because it has a much faster clock.  The other generators are not usable because the gitter is of order 50ns  ruining the averages.  The gitter from the analogic is nearer 5-10ns.  Its output goes to trigger the scope and to the sunc of the DPR500.  The scope is triggered from this input so that the time recorded is genuinly a pulse-echo-time.\n\n"
      <<"The DPR500 is controlled via serial connection.  It controls the imaging spike.\n\n"
      <<"The scope is controlled by LAN. Waveforms are grabbed directly from the scope.\n\n"
      <<"There is a no acoustic window with this experiment.  It is entirely unneccesary. \n\n"
      <<"The water is ultrapure water from December (i.e. left to stand in its container for a long time.  \n\n";
    setup=setup_stream.str();

    std::stringstream exp_stream;
    exp_stream
      <<"---------------------------------------------------------\n"
      <<"Since the extra signals seem dependant upon the gas in the water - this is a rerun after the whole cycle (upto 180mVpp) had gone before.  It should therefore be more stable"
      <<"---------------------------------------------------------";
    exp_proceedures = exp_stream.str();

  }
      
      
};


aline 
get_aline(
	  WG33220A<agilent::IP>& pulser,
	  lecroy_64Xi&  scope
	  ) 
{
  try{
    pulser.trigger_now();
    return scope.get_waveform(location::C3).get_data1();
  }
  catch(exception::could_not_get_fresh_aquisition& e){
    std::cout<<"missed trigger - refiring"<<std::endl;
    pulser.trigger_now();
    pulser.trigger_now();
    get_aline(pulser,scope);
  }

}



int
main  (int ac, char **av)
{
  
  //loadup descriptions
  description desc;

  
  ICR::directory dir("/data/2011/02/28/water/shorter_window-rerun");  //the current directory


  lecroy_64Xi            scope("10.0.0.10");
  WG33220A<agilent::IP>  driving_pulse("10.0.0.20");
  DPR500                 pulser("/dev/ttyUSB0");
  PulserReceiverFactory  factory;   
  RPL2* imaging_pulse = factory.make_RPL2();
  //variables
  double lecroy_timebase = 5e-6;
  double lecroy_trigger_delay = -50e-6;
  //double lecroy_timebase = 10e-6;
  //double lecroy_trigger_delay = -60e-6;
  unsigned long lecroy_samples = 250000;
  double lecroy_alines = 1;
  
  //double driving_voltage;
  const size_t different_driving_voltages = 30;
  double driving_min_voltage = 10e-3; //peak-to-peak
  double driving_max_voltage = 160e-3; //peak-to-peak
  double driving_interval    = (driving_max_voltage-driving_min_voltage)/double(different_driving_voltages);
  double driving_voltage = driving_min_voltage; //peak-to-peak
  double driving_phase = 0;
  double driving_frequency = 0.5e6;
  double driving_offset = 0;
  unsigned int driving_cycles = 10;
  

  double imaging_voltage = 300;
  int    imaging_gain = 50;
  

  //setup
  

  //IMAGING PULSE
  pulser.blink(200);  
  imaging_pulse->set_external_trigger();
  imaging_pulse->set_pulser_mode(damping_RPL2::fourtyfour_Ohm,
				 energy_policy::HIGH,
				 receive_mode::RECEIVE_ONLY);
  imaging_pulse->set_voltage(imaging_voltage);
  pulser.attach_A(imaging_pulse);
  pulser.notify();
  //)  filter::HPFA::   ten_kHz    sevenpointfive_MHz
  pulser.set_high_pass_filter_A(filter::HPFA::sevenpointfive_MHz);
  pulser.set_low_pass_filter_A(filter::LPFA::fifty_MHz);
  pulser.set_gain_A(imaging_gain);
  
  pulser.notify();

  driving_pulse.turn_off();
  driving_pulse.shape(shape::SIN);
  driving_pulse.frequency(driving_frequency);
  driving_pulse.burst_ext(edge::POSITIVE);
  driving_pulse.cycles(driving_cycles);
  driving_pulse.phase(driving_phase);
  driving_pulse.voltage(driving_voltage); 
  driving_pulse.offset(driving_offset) ;
  driving_pulse.trigger(trigger::BUS);
  driving_pulse.burst_on();

  driving_pulse.turn_on();
 
  //LECROY SETUP
  //scope.auto_calibrate(false); //turn calibration off
   scope.set_date();
   scope.auto_calibrate(true); //turn calibration on as people expect this to be set.
   scope.trigger_mode(trigger_mode::NORM);
   scope.trace_on (location::C1);
   scope.trace_off(location::C2);
   scope.trace_on(location::C3);
   scope.trace_off(location::C4);
   //scope.set_coupling(location::C1, coupling::D50);
   //scope.set_coupling(location::C2, coupling::D50);
   //scope.set_coupling(location::C3, coupling::D50);
   //scope.set_coupling(location::C4, coupling::D50);


    scope.trigger_level(location::C1, 1.0);
    scope.trigger_level(location::C3, 1.0);
    scope.trigger_coupling(location::C1, coupling::DC);
    scope.trigger_coupling(location::C3, coupling::DC);
    scope.trigger_positive_edge(location::C1);
    scope.trigger_positive_edge(location::C3);
    scope.trigger_select(trigger_type::EDGE,location::C1,trigger_hold_type::NO_HOLD);

    scope.trigger_delay(lecroy_trigger_delay);
    scope.set_timebase(lecroy_timebase);
    scope.volts_per_div(location::C1,1.0);
    scope.volts_per_div(location::C3,0.1);
    scope.volts_per_div(location::C4,1.0);
    scope.sequence(lecroy_alines,lecroy_samples);
   

    //create a new directory for this prerecord
    std::string subdir = "/receive_only";
    std::cout<<"subdir = "<< dir.get_directory()+subdir <<std::endl;

    dir.mkdir(dir.get_directory()+subdir); //create the subdirectory
    dir.pushd(dir.get_directory()+subdir);  //goto subdirectory
  

    pulser.turn_on_A();
    std::cout<<"ON"<<std::endl;

    aline a_test = get_aline(driving_pulse, scope);
    ICR::file Filename_test(dir,"test_GP.dat");
    ICR::lecroy::save_gnuplot(a_test,   Filename_test.full());

  std::cout<<" Enable power amp, press any key to continue"<<std::endl;
  char c;
  std::cin>> c; 
    
    for(size_t i = 18;i<different_driving_voltages;++i){
      std::cout<<"i = " <<i<<std::endl;

      driving_voltage = driving_min_voltage + i * driving_interval;
      
      driving_pulse.voltage(driving_voltage); 
      double lecroy_volts_per_div;

      if (driving_voltage<0.03) 
	lecroy_volts_per_div = 50e-3;
      else if (driving_voltage<0.05) 
	lecroy_volts_per_div = 70e-3;
      else if (driving_voltage<0.07) 
	lecroy_volts_per_div = 100e-3;
      else if (driving_voltage<0.08) 
	lecroy_volts_per_div = 150e-3;
      else if (driving_voltage<0.1) 
	lecroy_volts_per_div = 200e-3;
      else if (driving_voltage<0.11)
	{ 
	  lecroy_volts_per_div = 300e-3;
	  imaging_gain = 40;
	  pulser.set_gain_A(imaging_gain);
	  pulser.notify();
	}
      else if (driving_voltage<0.13)
	{ 
	  lecroy_volts_per_div = 400e-3;
	  imaging_gain = 40;
	  pulser.set_gain_A(imaging_gain);
	  pulser.notify();
	}
      else if (driving_voltage<0.14)
	{
	  lecroy_volts_per_div = 400e-3;
	  imaging_gain = 35;
	  pulser.set_gain_A(imaging_gain);
	  pulser.notify();
	}
      else
	{
	  lecroy_volts_per_div = 400e-3;
	  imaging_gain = 35;
	  pulser.set_gain_A(imaging_gain);
	  pulser.notify();
	}
      
      
      subdir = "/voltage_"+stringify(driving_voltage);
      dir.mkdir(dir.get_directory()+subdir); //create the subdirectory
      dir.pushd(dir.get_directory()+subdir); //goto subdirectory
	
      subdir = "/first_few_at_new_pressure_longer_window";
      dir.mkdir(dir.get_directory()+subdir); //create the subdirectory
      dir.pushd(dir.get_directory()+subdir); //goto subdirectory
//create a readme file in the directory.

      boost::progress_display pd_imag(20+50);
      //sometimes there are bubbles that burst in the first few alines - to capture these we set a large hight.
      scope.volts_per_div(location::C3, 2.0*lecroy_volts_per_div);
      //image the first 20
      for(size_t j=0;j<20;++j){
	readme README_EXP_2(dir.get_directory());
	README_EXP_2  
	  ("Lecroy volts per div", lecroy_volts_per_div)
	  ("imaging gain", imaging_gain);
	aline a = get_aline(driving_pulse, scope);
	std::string index = stringify_with_zeros(j,2); //pad the index with zeros
	ICR::file Filename_GP(dir,"driving_wave_only_new_pressure_rerun_short_var_gain"+index+"_GP.dat");
      
	//save gnuplot
	ICR::lecroy::save_gnuplot(a,   Filename_GP.full());
	++pd_imag;
      }
      dir.popd(); //return
      
	
      scope.volts_per_div(location::C3,lecroy_volts_per_div );
      readme README_EXP(dir.get_directory());
      README_EXP
	(desc.intro)
	(desc.setup)
	(desc.exp_proceedures)
	("-----------------------")
	("IMAGING WAVE RECEIVE ONLY")
	("-----------------------")
	("The first three alines are separated as these sometimes contain bubbles, and would like to save examples of these - The next 5 are abandoned so as to give time to settle down - the next 5 are saved so that reproducability is tested - the next 40 are averaged and only the average is returned, the next 5 are returned separately so that reproducability tested")
	("-----------------------")
	("Driving pulse details")
	("-----------------------")
	("driving voltage",  driving_voltage)
	("driving min voltage",  driving_min_voltage)
	("driving max voltage",  driving_max_voltage)
	("driving voltage steps", different_driving_voltages)
	("driving phase", driving_phase)
	("driving frequency",driving_frequency)
	("driving offset", driving_offset)
	("driving cycles", driving_cycles)
	("-----------------------")
	("Imaging pulse details")
	("-----------------------")
	("Delay from 33220A sync introduced with analogic", 28e-6)
	("Imaging Receiver", "RPL2")
	("Imaging damping", "damping_RPL2::fourtyfour_Ohm")
	("Imaging energy_policy", "HIGH")
	("Imaging receive_mode","RECEIVE_ONLY")
	("Imaging voltage ",imaging_voltage)
	("imaging gain", imaging_gain)
	("Imaging high pass filter","sevenpointfive_MHz")
	("Imaging low pass filter", "fifty_MHz")   
	("-----------------------")   
	("Lecroy setup")
	("-----------------------")
	("Lecroy sequence", lecroy_alines)
	("Lecroy samples ", lecroy_samples)
	("Lecroy timebase",lecroy_timebase )
	("Lecroy trigger_delay", lecroy_trigger_delay)
	("Lecroy volts per div", lecroy_volts_per_div)
	("-----------------------")   
	;
      //save the next 50 to give indication of reproducability
      for(size_t j=0;j<50;++j){
	aline a = get_aline(driving_pulse, scope); 
	std::string index = stringify_with_zeros(j,2); //pad the index with zeros
      
	ICR::file Filename_GP(dir,"driving_wave_only_rerun_short_"+index+"_GP.dat");
      
	//save gnuplot
	ICR::lecroy::save_gnuplot(a,   Filename_GP.full());
	++pd_imag;
      }
      
      dir.popd(); //leave the subdir.
      //DONE!
    }


    // //create a new directory for this prerecord
    // subdir = "/phase_profile";
    // dir.mkdir(dir.get_directory()+subdir); //create the subdirectory
    // dir.pushd(dir.get_directory()+subdir);  //goto subdirectory

    // readme README_PHASE(dir.get_directory());
    // README_PHASE
    //   (desc.intro)
    //   (desc.setup)
    //   (desc.exp_proceedures)
    //   ("-----------------------")
    //   ("Imaging pulse details")
    //   ("-----------------------")
    //   ("Imaging Receiver", "RPL2")
    //   ("Imaging damping", "damping_RPL2::fourtyfour_Ohm")
    //   ("Imaging energy_policy", "HIGH")
    //   ("Imaging receive_mode","RECEIVE_ONLY")
    //   ("Imaging voltage",imaging_voltage)
    //   ("imaging gain", imaging_gain)
    //   ("Imaging high pass filter","ten_kHz")
    //   ("Imaging low pass filter", "fifty_MHz")   
    //   ;
  
    // driving_pulse.turn_off();
    // boost::progress_display pd_phase(12);
    // for(size_t i=0;i<12;++i){
    //   driving_pulse.trigger_now();
    //   lecroy_file file =  scope.get_waveform(location::C4);

    //   aline a = file.get_data1();

    //   std::string index = stringify_with_zeros(i,2); //pad the index with zeros
      
    //   ICR::file Filename(dir,"phase_profile_off_"+index+".dat");
    //   ICR::file Filename_GP(dir,"phase_profile_off_"+index+"_GP.dat");
      
    //   //save alines;
    //   ICR::lecroy::save(a, Filename.full()    );
    //   //save gnuplot
    //   ICR::lecroy::save_gnuplot(a,   Filename_GP.full());
    //   ++pd_phase;
    // }
    // //turn on driving pulse
    // driving_pulse.turn_on();

    // boost::progress_display pd_phase2(12);
    // for(size_t i=0;i<12;++i){
    //   driving_pulse.trigger_now();
    //   lecroy_file file =  scope.get_waveform(location::C4);

    //   aline a = file.get_data1();

    //   std::string index = stringify_with_zeros(i,2); //pad the index with zeros
      
    //   ICR::file Filename(dir,"phase_profile_on_"+index+".dat");
    //   ICR::file Filename_GP(dir,"phase_profile_on_"+index+"_GP.dat");
      
    //   //save alines;
    //   ICR::lecroy::save(a, Filename.full()    );
    //   //save gnuplot
    //   ICR::lecroy::save_gnuplot(a,   Filename_GP.full());
    //   ++pd_phase2;
    // }
    // dir.popd(); //leave the subdir.
    // //-------------------------------------------------------------
    


    
    // pulser.turn_off_A();  //make sure the pulser perminantely running
    // std::cout<<"OFF"<<std::endl;
  


    // driving_pulse.turn_off();
    // scope.auto_calibrate(true); //turn calibration on as people expect this to be set.

}

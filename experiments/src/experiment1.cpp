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
using namespace ICR::lecroy::location;


double pulse_offset(double imaging_echo_time_to_rear,
		    double driving_echo_time_to_front,
		    double duration_of_driving_wave,
		    double time_between_pulse_pair)
{
  //want imaging wave to coincide with rear of driving wave
  
  //imaging 1 way time
  double imag_time = imaging_echo_time_to_rear/2.0;
  
  //driving 1 way time to rear
  double driving_time = (driving_echo_time_to_front/2.0)
    + duration_of_driving_wave;
  
  std::cout<<"driving time = "<<driving_time<<std::endl;


  
  //difference in time 
  //(driving wave first so should be negative)
  double delta_time = imag_time-driving_time;
  std::cout<<"delta time = "<<delta_time<<std::endl;

  double offset = time_between_pulse_pair-delta_time;
  
  std::cout<<"offset = "<<offset<<std::endl;

  // std::cout<<"scope offset = "<<imag_time+driving_time<std::endl;


  return offset;
}

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
  bus_sync.trigger(trigger::BUS);
  bus_sync.burst_on();

  int driving_cycles = 10;
  double driving_frequency = 0.5e6;
  double driving_offset = 0.0 ;
  double driving_voltage = 0.01;
  double driving_phase = 0.0;

  //DRIVING PULSE
  driving_pulse.shape(shape::SIN);
  driving_pulse.frequency(driving_frequency);
  driving_pulse.burst_ext(edge::POSITIVE);
  driving_pulse.cycles(driving_cycles);
  driving_pulse.phase(driving_phase);
  driving_pulse.voltage(driving_voltage);  //range from 0.2 to 0.35
  driving_pulse.offset(driving_offset);
  driving_pulse.burst_on();
  
  double imaging_voltage = 275;
  int    imaging_gain = 60;

  //PULSER
  pulser.blink(200);  
  imaging_pulse->set_external_trigger();
  imaging_pulse->set_pulser_mode(damping_RPL2::fourtyfour_Ohm,
				 energy_policy::HIGH,
				 receive_mode::PULSE_ECHO);
  imaging_pulse->set_voltage(imaging_voltage);
  pulser.attach_A(imaging_pulse);
  //)  filter::HPFA::   ten_kHz    sevenpointfive_MHz
  pulser.set_high_pass_filter_A(filter::HPFA::sevenpointfive_MHz);
  pulser.set_low_pass_filter_A(filter::LPFA::fifty_MHz);
  pulser.set_gain_A(imaging_gain);
  
  pulser.notify();
  pulser.turn_on_A();
  std::cout<<"ON"<<std::endl;
  
  int lecroy_samples=100000;
  //LECROY SETUP
  scope.set_date();
  scope.sequence(2,lecroy_samples);
  scope.trigger_mode(trigger_mode::NORM);
 

  //EXPERIMENT RANGES
  double voltage_min = 0.01;
  double voltage_max = 0.40;
  int steps = 50;
  
  int repeats= 12;
    
  //EXPERIMENT
  ICR::directory dir(".");  //the current directory
  double voltage_intv = (voltage_max-voltage_min)/(steps);

  try{
    scope.demand_fresh_aquisition();
  }
  catch(ICR::lecroy::exception::could_not_get_fresh_aquisition& e){
    /* missed the trigger for some reason, 
     * retrigger.
     */
    std::cout<<"could not get aquisition, retrying"<<std::endl;
    bus_sync.trigger_now();
  }
  
  scope.auto_calibrate(false);
  for(int i=0;i<steps;++i){
    double driving_voltage = voltage_min + i * voltage_intv;
    std::cout<<"voltage = "<<driving_voltage<<std::endl;
    
    boost::progress_display pd(repeats);

    driving_pulse.voltage( driving_voltage );

    //create a new directory for each set of repeats
    std::string subdir = "./voltage_"+stringify(driving_voltage);
    dir.mkdir(subdir); //create the subdirectory
    dir.pushd(subdir);  //goto subdirectory
    
    //create a readme file in the directory.
    readme README(dir.get_directory());
    
    // get the pulse offset
    // std::cout<<"pulse_offset = "<<
    //   pulse_offset(49.65,
    // 		   65.30,
    // 		   20,
    // 		   119.98)<<std::endl;
    
    //write the relevent info.
    README
      //bus_sync details
      ("gate (approx) (us) ", 20)
      ("time between driving gates (us)", 119.98)
      //
      //delay details
      ("time from driving trigger to driving wave", 0.2016)
      ("time from bottom of imaging trigger to top of returned echo - rear window (us)", 49.65 )
      ("time from bottom of driving trigger to first of returned echo - rear window (us)", 65.1386 )
      ("Time between 2nd driving trigger and imaging trigger should be (27.825)",28.2 )
      ("Scope timebase", -76.0)
      //driving pulse
      ("driving generator", "AGILENT 33220A")
      ("Attached to", "A")
      ("driving Shape", "SIN")
      ("driving frequency", driving_frequency)
      ("driving burst edge", "POSITIVE")
      ("driving cycles", driving_cycles)
      ("driving phase", driving_phase)
      ("driving voltage", driving_voltage)
      //Imaging pulse
      ("Imaging Receiver", "RPL2")
      ("Imaging damping", "damping_RPL2::fourtyfour_Ohm")
      ("Imaging energy_policy", "HIGH")
      ("Imaging receive_mode","PULSE_ECHO")
      ("Imaging voltage (need to check no bug here)",imaging_voltage)
      ("imaging gain", imaging_gain)
      ("Imaging high pass filter","sevenpointfive")
      ("Imaging low pass filter", "fifty_MHz")      
      //lecroy setup
      ("Lecroy sequence", 2)
      ("Lecroy samples ", lecroy_samples)
      
      ;
    
    //repeat the experiment a few times.
    for(int i=0;i<repeats;++i){
      
      //Trigger	
      bus_sync.trigger_now();
      lecroy_file file;
      
      // scope.wait();
      try{
	//std::cout<<"getting waveform"<<std::endl;

	file =  scope.get_waveform(C4);
      }
      catch(ICR::lecroy::exception::could_not_get_fresh_aquisition& e){
	/* missed the trigger for some reason, 
	 * retrigger.
	 */
	std::cout<<"could not get aquisition, retrying"<<std::endl;

	bus_sync.trigger_now();
	
	file =  scope.get_waveform(C4);
      }
      //	std::cout<<"got waveform"<<std::endl;
      //std::cout<<"got wave"<<std::endl;

      aline without_imaging_pulse = file.get_data1(0); // the first aline
      aline with_imaging_pulse    = file.get_data1(1); // the first aline
      aline excess_pulse          = with_imaging_pulse-without_imaging_pulse;

      // if (excess_pulse.max()>0.2) {
      // 	std::cout<<"excess max = "<<excess_pulse.max()<<std::endl;
      // }
	

      ICR::file WithFilename   (dir,"with_imaging_pulse_"+stringify(i)+".dat");
      ICR::file WithoutFilename(dir,"without_imaging_pulse_"+stringify(i)+".dat");
      ICR::file ExcessFilename (dir,"excess_pulse_"+stringify(i)+".dat");
      
      ICR::file WithGnuplotFilename   (dir,"with_imaging_pulse_"+stringify(i)+".txt");
      ICR::file WithoutGnuplotFilename(dir,"without_imaging_pulse_"+stringify(i)+".txt");
      ICR::file ExcessGnuplotFilename (dir,"excess_pulse_"+stringify(i)+".txt");
      


      // with_imaging_pulse.save_ascii(WithGnuplotFilename.full() );
      // without_imaging_pulse.save_ascii(WithoutGnuplotFilename.full() );
      // excess_pulse.save_ascii(ExcessGnuplotFilename.full() );
      
      // //save alines;
      // ICR::lecroy::save(with_imaging_pulse,   WithFilename.full()    );
      // ICR::lecroy::save(without_imaging_pulse,WithoutFilename.full() );
      // ICR::lecroy::save(without_imaging_pulse,ExcessFilename.full()  );
      ++pd;
    }
    dir.popd(); //leave the subdir.
  }
  
  // boost::this_thread::sleep(boost::posix_time::milliseconds(2000)); //
  
   pulser.turn_off_A();
   std::cout<<"OFF"<<std::endl;

}

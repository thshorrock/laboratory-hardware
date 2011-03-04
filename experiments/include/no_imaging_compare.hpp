#pragma once
#include "analogic.hpp"
#include "agilent.hpp"
#include "lecroy.hpp"
#include "DPR500.hpp"

#include "filename.hpp"
#include "readme.hpp"
#include "get_alines.hpp"
#include "vec.hpp"
#include "fourier/xcorrelation.hpp"
#include "boost/units/systems/si.hpp"

using namespace ICR::agilent;
using namespace ICR::analogic;
using namespace ICR::pulser;
using namespace ICR::lecroy;
using namespace ICR;

class no_imaging_compare
{
  ICR::directory m_dir;
  double m_deltaT, m_volts_per_div,m_imaging_gain, m_imaging_voltage;
  double m_analogic_delay, m_agilent_delay, m_clock;
  std::string m_subdirectory;
public:
  no_imaging_compare(const std::string& dir,
		     const double& driving_voltage,
		     const std::string subdir
		     );
  ~no_imaging_compare();
  void setup_analogic(analogic_remote_control& analogic,
		      const double& analogic_delay,
		      const double& agilent_delay);
  void setup_pulser(
		    DPR500& pulser,
		    RPL2* imaging_pulse,
		    const double& voltage
		    );
  double
  run(
      WG33220A<agilent::IP>& driving_pulse,
      lecroy_64Xi&  scope, 
      const size_t& repeats
      );
};

template<int i>
class imaging_on
{
  ICR::directory m_dir;
  double m_deltaT, m_volts_per_div,m_imaging_gain, m_imaging_voltage;
  double m_analogic_delay, m_agilent_delay, m_clock;
  std::string m_subdirectory;
public:
  imaging_on(const std::string& dir,
	     const double& driving_voltage,
	     const std::string subdir
	     );
  ~imaging_on();
  void setup_analogic(analogic_remote_control& analogic,
		      const double& analogic_delay,
		      const double& agilent_delay);
  
  void setup_pulser(
		    DPR500& pulser,
		    RPL2* imaging_pulse,
		    const double& voltage
		    );
  void
  run(
      WG33220A<agilent::IP>& driving_pulse,
      lecroy_64Xi&  scope, 
      const size_t& repeats
      );
};


class phase_plot
{
  ICR::directory m_dir;
  double m_deltaT, m_volts_per_div,m_imaging_gain, m_imaging_voltage;
  double m_analogic_delay, m_agilent_delay, m_clock;
  std::string m_subdirectory;
public:
  phase_plot(const std::string& dir,
	     const double& driving_voltage,
	     const std::string subdir = "phases"
	     );
  ~phase_plot();
  void setup_analogic(analogic_remote_control& analogic,
		      const double& analogic_delay);
  
  void setup_pulser(
		    DPR500& pulser,
		    RPL2* imaging_pulse,
		    const double& voltage
		    );
  void
  run(
      WG33220A<agilent::IP>& driving_pulse,
      lecroy_64Xi&  scope, 
      const size_t& repeats
      );
};




no_imaging_compare::no_imaging_compare(const std::string& dir,
				       const double& driving_voltage,
				       const std::string subdir
				       )
  : m_dir(dir),
    m_subdirectory(subdir)
{
  m_imaging_gain = 50;
  
  if (driving_voltage<0.03) 
    m_volts_per_div = 50e-3;
  else if (driving_voltage<0.05) 
    m_volts_per_div = 70e-3;
  else if (driving_voltage<0.07) 
    m_volts_per_div = 100e-3;
  else if (driving_voltage<0.08) 
    m_volts_per_div = 150e-3;
  else if (driving_voltage<0.1) 
    m_volts_per_div = 200e-3;
  else if (driving_voltage<0.11)
    { 
      m_volts_per_div = 300e-3;
      m_imaging_gain = 40;
    }
  else if (driving_voltage<0.13)
    { 
      m_volts_per_div = 400e-3;
      m_imaging_gain = 40;
    }
  else if (driving_voltage<0.14)
    {
      m_volts_per_div = 400e-3;
      m_imaging_gain = 35;
    }
  else
    {
      m_volts_per_div = 400e-3;
      m_imaging_gain = 35;
    }
      
}

no_imaging_compare::~no_imaging_compare()
{
  m_dir.popd();
}




void 
no_imaging_compare::setup_analogic(analogic_remote_control& analogic,
				   const double& analogic_delay,
				   const double& agilent_delay)
{
  
 double m_analogic_low = 1.5;


  //create a subdir
  std::string subdir = "/gap_between_pulses_"+stringify(agilent_delay);
  std::cout<<"subdir = "<< m_dir.get_directory()+subdir <<std::endl;
  
  m_dir.mkdir(m_dir.get_directory()+subdir); //create the subdirectory
  m_dir.pushd(m_dir.get_directory()+subdir);  //goto subdirectory

  m_analogic_delay = analogic_delay;
  m_agilent_delay = agilent_delay;
   
  if (agilent_delay<640)
    m_clock = 1.25;
  else if (agilent_delay<1280) 
    m_clock = 2.5;
  else if (agilent_delay<2560) 
    m_clock = 5;
  else 
    m_clock = 10;
  std::string no_imaging = "AT TRIG RPT 1 ( FOR "+stringify(m_analogic_delay)+"u 0.0 FOR 2u "+stringify(m_analogic_low)+" FOR "+stringify(m_agilent_delay+18)+"u 0.0 FOR 2u "+stringify(m_analogic_low)+" FOR 6u 0) CLK = "+stringify(m_clock)+"n";

    
  analogic.expression(no_imaging);  //create a pulse
  analogic.turn_on(); 
}


double
no_imaging_compare::run(
			WG33220A<agilent::IP>& driving_pulse,
			lecroy_64Xi&  scope, 
			const size_t& repeats)
{
  std::string subdir = m_subdirectory;
  std::cout<<"subdir = "<< m_dir.get_directory() + "/"+subdir <<std::endl;
  
  m_dir.mkdir(m_dir.get_directory()+subdir); //create the subdirectory
  m_dir.pushd(m_dir.get_directory()+subdir);  //goto subdirectory
  
  readme README(m_dir.get_directory());
  README
    ("repeats", repeats)
    ("-----------------------")
    ("Analogic setup")
    ("-----------------------")   
    ("analogic delay",m_analogic_delay)
    ("agilent delay",m_agilent_delay)
    ("clock",m_clock)
    ("-----------------------")
    ("Imaging pulse details")
    ("-----------------------")
    ("Delay from 33220A sync introduced with analogic (us)", m_analogic_delay)
    ("Imaging Receiver", "RPL2")
    ("Imaging damping", "damping_RPL2::fourtyfour_Ohm")
    ("Imaging energy_policy", "HIGH")
    ("Imaging receive_mode","PULSE_ECHO")
    ("Imaging voltage ", m_imaging_voltage)
    ("imaging gain", m_imaging_gain)
    ("Imaging high pass filter","sevenpointfive_MHz")
    ("Imaging low pass filter", "fifty_MHz")   
    ("-----------------------")
    ("Lecroy setup")
    ("-----------------------")
    ("Lecroy samples ",250000 )
    ("Lecroy timebase",5e-6)
    ("Lecroy trigger_delay", -50e-6)
    ("Lecroy volts per div", m_volts_per_div)
    ("-----------------------")   
    ;
  

  aline aline1;
  aline aline2;
  aline av1;
  aline av2;
    
  scope.sequence(2,250000);
  scope.set_timebase(5e-6);
  scope.trigger_delay(-50e-6);
  scope.volts_per_div(location::C2,m_volts_per_div);
    
  driving_pulse.turn_on();
  driving_pulse.trigger_now();
  driving_pulse.trigger_now();
  
  for(size_t i=0;i<repeats;++i){
    get_alines(driving_pulse, scope, aline1, aline2);
    std::string index = stringify_with_zeros(i,3); //pad the index with zeros
    ICR::file Filename_GP1(m_dir,"no_imaging_1st_"+index+"_GP.dat");
    ICR::file Filename_GP2(m_dir,"no_imaging_2nd_"+index+"_GP.dat");
    ICR::lecroy::save_gnuplot(aline1, Filename_GP1.full()  );
    ICR::lecroy::save_gnuplot(aline2, Filename_GP2.full()  );
    if (i==0) 
      {
	av1 = aline1;
	av2 = aline2;
      }
    else 
      {
	av1+=aline1;
	av2+=aline2;
      }
  }
  std::string av_subdir = "/averages";
  m_dir.mkdir(m_dir.get_directory()+av_subdir); //create the subdirectory
  m_dir.pushd(m_dir.get_directory()+av_subdir);  //goto subdirectory
  
  av1/=repeats;
  av2/=repeats;
  
  ICR::file av_Filename_GP1(m_dir,"av_no_imaging_1st_GP.dat");
  ICR::file av_Filename_GP2(m_dir,"av_no_imaging_2nd_GP.dat");
  ICR::lecroy::save_gnuplot(av1, av_Filename_GP1.full()  );
  ICR::lecroy::save_gnuplot(av2, av_Filename_GP2.full()  );
  

  ICR::maths::vector<boost::units::si::electric_potential> v1(av1.size(), av1._data());
  ICR::maths::vector<boost::units::si::electric_potential> v2(av2.size(), av2._data());
  
  ICR::maths::xcorr<boost::units::si::electric_potential,ICR::maths::xcorr_scaling::coeff> xc(v1,v2);
  double result = xc.result()[0]; //.max_index()];
  std::cout<<"xc = "<<result <<std::endl; //<<" at xc offset = "<<xc.offset_index<<std::endl;
  

  driving_pulse.turn_off();
  m_dir.popd();

  m_dir.popd(); //no_imag_pulse
  
  return result;
  
}



void
no_imaging_compare::setup_pulser(
				 DPR500& pulser,
				 RPL2* imaging_pulse,
				 const double& voltage
				 )

{
  m_imaging_voltage = voltage;
      
  pulser.blink(150);  
  imaging_pulse->set_external_trigger();
  imaging_pulse->set_pulser_mode(damping_RPL2::fourtyfour_Ohm,
				 energy_policy::HIGH,
				 receive_mode::PULSE_ECHO);
  imaging_pulse->set_voltage(m_imaging_voltage);
  pulser.notify();

  pulser.set_high_pass_filter_A(filter::HPFA::sevenpointfive_MHz);
  pulser.set_low_pass_filter_A(filter::LPFA::fifty_MHz);
  pulser.set_gain_A(m_imaging_gain);
  imaging_pulse->set_voltage(m_imaging_voltage);
  pulser.notify();
      
}

















template<int i>
imaging_on<i>::imaging_on(const std::string& dir,
			  const double& driving_voltage,
			  const std::string subdir
			  )
  : m_dir(dir),
    m_subdirectory(subdir)
{
  m_imaging_gain = 50;

  if (driving_voltage<0.03) 
    m_volts_per_div = 50e-3;
  else if (driving_voltage<0.05) 
    m_volts_per_div = 70e-3;
  else if (driving_voltage<0.07) 
    m_volts_per_div = 100e-3;
  else if (driving_voltage<0.08) 
    m_volts_per_div = 150e-3;
  else if (driving_voltage<0.1) 
    m_volts_per_div = 200e-3;
  else if (driving_voltage<0.11)
    { 
      m_volts_per_div = 300e-3;
      m_imaging_gain = 40;
    }
  else if (driving_voltage<0.13)
    { 
      m_volts_per_div = 400e-3;
      m_imaging_gain = 40;
    }
  else if (driving_voltage<0.14)
    {
      m_volts_per_div = 400e-3;
      m_imaging_gain = 35;
    }
  else
    {
      m_volts_per_div = 400e-3;
      m_imaging_gain = 35;
    }
      
      

}

template<int i>
imaging_on<i>::~imaging_on()
{
  m_dir.popd();
}




template<int ipulse>
void 
imaging_on<ipulse>::setup_analogic(analogic_remote_control& analogic,
				   const double& analogic_delay,
				   const double& agilent_delay)
{
  double m_analogic_high = 4.5; 
  double m_analogic_low = 1.5;

  //create a subdir
  std::string subdir = "/gap_between_pulses_"+stringify(agilent_delay);
  std::cout<<"subdir = "<< m_dir.get_directory()+subdir <<std::endl;
  
  m_dir.mkdir(m_dir.get_directory()+subdir); //create the subdirectory
  m_dir.pushd(m_dir.get_directory()+subdir);  //goto subdirectory

  m_analogic_delay = analogic_delay;
  m_agilent_delay = agilent_delay;
   
  if (agilent_delay<640)
    m_clock = 1.25;
  else if (agilent_delay<1280) 
    m_clock = 2.5;
  else if (agilent_delay<2560) 
    m_clock = 5;
  else 
    m_clock = 10;

  std::string  imaging;
  if (ipulse == 0) 
    imaging = "AT TRIG RPT 1 ( FOR "+stringify(m_analogic_delay)+"u 0.0 FOR 2u "+stringify(m_analogic_high)+" FOR "+stringify(m_agilent_delay+18)+"u 0.0 FOR 2u "+stringify(m_analogic_low)+" FOR 6u 0) CLK = "+stringify(m_clock)+"n";
  else
    imaging = "AT TRIG RPT 1 ( FOR "+stringify(m_analogic_delay)+"u 0.0 FOR 2u "+stringify(m_analogic_low)+" FOR "+stringify(m_agilent_delay+18)+"u 0.0 FOR 2u "+stringify(m_analogic_high)+" FOR 6u 0) CLK = "+stringify(m_clock)+"n";
  
  
  analogic.expression(imaging);  //create a pulse
  analogic.turn_on(); 
}



template<int ipulse>
void
imaging_on<ipulse>::run(
			WG33220A<agilent::IP>& driving_pulse,
			lecroy_64Xi&  scope, 
			const size_t& repeats)
{

  //create a subdir
  std::string subdir = m_subdirectory;
  std::cout<<"subdir = "<< m_dir.get_directory()+subdir <<std::endl;
  
  m_dir.mkdir(m_dir.get_directory()+"/"+subdir); //create the subdirectory
  m_dir.pushd(m_dir.get_directory()+"/"+subdir);  //goto subdirectory


  readme README(m_dir.get_directory());
  README
    ("repeats", repeats)
    ("-----------------------")
    ("Analogic setup")
    ("-----------------------")   
    ("analogic delay",m_analogic_delay)
    ("agilent delay",m_agilent_delay)
    ("clock",m_clock)
    ("-----------------------")
    ("Imaging pulse details")
    ("-----------------------")
    ("Delay from 33220A sync introduced with analogic", m_analogic_delay)
    ("Imaging Receiver", "RPL2")
    ("Imaging damping", "damping_RPL2::fourtyfour_Ohm")
    ("Imaging energy_policy", "HIGH")
    ("Imaging receive_mode","PULSE_ECHO")
    ("Imaging voltage ", m_imaging_voltage)
    ("imaging gain", m_imaging_gain)
    ("Imaging high pass filter","sevenpointfive_MHz")
    ("Imaging low pass filter", "fifty_MHz")   
    ("-----------------------")   
    ("Lecroy setup")
    ("-----------------------")
    ("Lecroy samples ",250000 )
    ("Lecroy timebase",5e-6)
    ("Lecroy trigger_delay", -50e-6)
    ("Lecroy volts per div", m_volts_per_div)
    ("-----------------------")   
    ;
  

  aline aline1;
  aline aline2;
    
  scope.sequence(2,250000);
  scope.set_timebase(5e-6);
  scope.trigger_delay(-50e-6);
  scope.volts_per_div(location::C2,m_volts_per_div);
    
  driving_pulse.turn_on();
  driving_pulse.trigger_now();
  driving_pulse.trigger_now();
  
  for(size_t i=0;i<repeats;++i){
    get_alines(driving_pulse, scope, aline1, aline2);
    std::string index = stringify_with_zeros(i,3); //pad the index with zeros
    ICR::file Filename_GP1(m_dir,"no_imaging_1st_"+index+"_GP.dat");
    ICR::file Filename_GP2(m_dir,"no_imaging_2nd_"+index+"_GP.dat");
    ICR::lecroy::save_gnuplot(aline1, Filename_GP1.full()  );
    ICR::lecroy::save_gnuplot(aline2, Filename_GP2.full()  );
   
  }

  driving_pulse.turn_off();
  m_dir.popd(); //out of imaging pulse
  
}

template<int i>
void 
imaging_on<i>::setup_pulser(
			    DPR500& pulser,
			    RPL2* imaging_pulse,
			    const double& voltage
			    )

{
  m_imaging_voltage = voltage;
      
  pulser.blink(150);  
  imaging_pulse->set_external_trigger();
  imaging_pulse->set_pulser_mode(damping_RPL2::fourtyfour_Ohm,
				 energy_policy::HIGH,
				 receive_mode::PULSE_ECHO);
  imaging_pulse->set_voltage(m_imaging_voltage);
  pulser.notify();

  pulser.set_high_pass_filter_A(filter::HPFA::sevenpointfive_MHz);
  pulser.set_low_pass_filter_A(filter::LPFA::fifty_MHz);
  pulser.set_gain_A(m_imaging_gain);
  imaging_pulse->set_voltage(m_imaging_voltage);
  pulser.notify();
      
}


























phase_plot::phase_plot(const std::string& dir,
		       const double& driving_voltage,
		       const std::string subdir 
		       )
  : m_dir(dir),
    m_subdirectory(subdir)
{
  

  m_dir.mkdir(m_dir.get_directory()+ "/"+subdir); //create the subdirectory
  m_dir.pushd(m_dir.get_directory()+ "/"+subdir);  //goto subdirectory
  
  m_dir.mkdir(m_dir.get_directory()+ "/voltage_"+stringify(driving_voltage)); //create the subdirectory
  m_dir.pushd(m_dir.get_directory()+ "/voltage_"+stringify(driving_voltage));  //goto subdirectory
  
  m_imaging_gain = 30;
  if (driving_voltage<0.03) 
    m_volts_per_div = 50e-3;
  else if (driving_voltage<0.05) 
    m_volts_per_div = 70e-3;
  else if (driving_voltage<0.07) 
    m_volts_per_div = 100e-3;
  else if (driving_voltage<0.08) 
    m_volts_per_div = 150e-3;
  else if (driving_voltage<0.1) 
    m_volts_per_div = 200e-3;
  else if (driving_voltage<0.11)
    { 
      m_volts_per_div = 300e-3;
      m_imaging_gain = 23;
    }
  else if (driving_voltage<0.13)
    { 
      m_volts_per_div = 300e-3;
      m_imaging_gain = 23;
    }
  else if (driving_voltage<0.14)
    {
      m_volts_per_div = 400e-3;
      m_imaging_gain = 20;
    }
  else
    {
      m_volts_per_div = 400e-3;
      m_imaging_gain = 20;
    }
      
}

phase_plot::~phase_plot()
{
  m_dir.popd();
}




void 
phase_plot::setup_analogic(analogic_remote_control& analogic,
			   const double& analogic_delay)
{
  
 double m_analogic_low = 1.5;


  m_analogic_delay = analogic_delay;
   
    m_clock = 1.25;
  std::string no_imaging = "AT TRIG RPT 1 ( FOR "+stringify(m_analogic_delay)+"u 0.0 FOR 2u "+stringify(m_analogic_low)+" FOR 18u 0.0) CLK = "+stringify(m_clock)+"n";

    
  analogic.expression(no_imaging);  //create a pulse
  analogic.turn_on(); 
}


void
phase_plot::run(
			WG33220A<agilent::IP>& driving_pulse,
			lecroy_64Xi&  scope, 
			const size_t& repeats)
{
  readme README(m_dir.get_directory());
  README
    ("repeats", repeats)
    ("-----------------------")
    ("Analogic setup")
    ("-----------------------")   
    ("analogic delay",m_analogic_delay)
    ("agilent delay",m_agilent_delay)
    ("clock",m_clock)
    ("-----------------------")
    ("Imaging pulse details")
    ("-----------------------")
    ("Delay from 33220A sync introduced with analogic", m_analogic_delay)
    ("Imaging Receiver", "RPL2")
    ("Imaging damping", "damping_RPL2::fourtyfour_Ohm")
    ("Imaging energy_policy", "HIGH")
    ("Imaging receive_mode","PULSE_ECHO")
    ("Imaging voltage ", m_imaging_voltage)
    ("imaging gain", m_imaging_gain)
    ("Imaging high pass filter","ten_kHz")
    ("Imaging low pass filter", "fifty_MHz")   
    ("-----------------------")
    ("Lecroy setup")
    ("-----------------------")
    ("Lecroy samples ",250000 )
    ("Lecroy timebase",5e-6)
    ("Lecroy trigger_delay", -50e-6)
    ("Lecroy volts per div", m_volts_per_div)
    ("-----------------------")   
    ;
  
    
  scope.sequence(1,250000);
  scope.set_timebase(5e-6);
  scope.trigger_delay(-50e-6);
  scope.volts_per_div(location::C2,m_volts_per_div);

  aline a;

  m_dir.mkdir(m_dir.get_directory()+"/driving_wave_off"); //create the subdirectory
  m_dir.mkdir(m_dir.get_directory()+"/driving_wave_on"); //create the subdirectory


  m_dir.pushd(m_dir.get_directory()+"/driving_wave_off");  //goto

  driving_pulse.trigger_now();
  driving_pulse.trigger_now();

  driving_pulse.turn_off();
  for(size_t i=0;i<repeats;++i){

    get_alines(driving_pulse, scope, a);
    std::string index = stringify_with_zeros(i,3); //pad the index with zeros
    ICR::file Filename_GP(m_dir,"phase_profile_off_"+index+"_GP.dat");
    ICR::lecroy::save_gnuplot(a,   Filename_GP.full());
  }
  m_dir.popd();
  m_dir.pushd(m_dir.get_directory()+"/driving_wave_on");  //goto

  //turn on driving pulse
  driving_pulse.turn_on();

  for(size_t i=0;i<repeats;++i){

    get_alines(driving_pulse, scope, a);
    std::string index = stringify_with_zeros(i,3); //pad the index with zeros
    ICR::file Filename_GP(m_dir,"phase_profile_on_"+index+"_GP.dat");
    ICR::lecroy::save_gnuplot(a,   Filename_GP.full());
  }



  m_dir.popd(); //leave the imaging_wave on.


  m_dir.popd(); //driving voltage
  
  
}



void
phase_plot::setup_pulser(
				 DPR500& pulser,
				 RPL2* imaging_pulse,
				 const double& voltage
				 )

{
  m_imaging_voltage = voltage;
      
  pulser.blink(150);  
  imaging_pulse->set_external_trigger();
  imaging_pulse->set_pulser_mode(damping_RPL2::fourtyfour_Ohm,
				 energy_policy::HIGH,
				 receive_mode::PULSE_ECHO);
  imaging_pulse->set_voltage(m_imaging_voltage);
  pulser.notify();

  pulser.set_high_pass_filter_A(filter::HPFA::ten_kHz);
  pulser.set_low_pass_filter_A(filter::LPFA::fifty_MHz);
  pulser.set_gain_A(m_imaging_gain);
  imaging_pulse->set_voltage(m_imaging_voltage);
  pulser.notify();
      
}














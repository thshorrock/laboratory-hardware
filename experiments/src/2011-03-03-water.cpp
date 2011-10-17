#include "analogic.hpp"
#include "stringify.hpp"
#include "agilent.hpp"
#include "lecroy.hpp"
#include "DPR500.hpp"

#include "get_alines.hpp"
#include "no_imaging_compare.hpp"

using namespace ICR::agilent;
using namespace ICR::analogic;
using namespace ICR::lecroy;
using namespace ICR::pulser;
using namespace ICR;




int
main  (int ac, char **av)
{


  // const double analogic_delay = 28; //microseconds
  // double driving_voltage=50e-3;
  // double agilent_delay=12000;
  
  //  WG33220A<agilent::IP>  driving_pulse("10.0.0.20");
  //  two_sin_with_n_microsecond_gap(&driving_pulse, 0.5, agilent_delay, 0.5* driving_voltage);
  //  driving_pulse.cycles(1);

   // two_pulse_with_n_microsecond_gap(&driving_pulse,22, agilent_delay-4, 2.5);
  //  driving_pulse.cycles(1);

  // WG33120A  gate("/dev/ttyUSB2");
  // two_pulse_with_n_microsecond_gap(&gate,22, agilent_delay-4, 2.5);
  // gate.cycles(1);


  ICR::directory dir("/mnt/ext_hd/data/2011/03/05/water");  //the current directory

  analogic_remote_control analogic("/dev/ttyUSB0");
  std::cout<<"here"<<std::endl;

  WG33220A<agilent::IP>  driving_pulse("10.0.0.20");
  //std::cout<<"here0"<<std::endl;
  lecroy_64Xi  scope("10.0.0.10");
  std::cout<<"here0.1"<<std::endl;
  
  DPR500                 pulser("/dev/ttyUSB1");
  std::cout<<"here0.2"<<std::endl;

  PulserReceiverFactory  factory;   
  RPL2* imaging_pulse = factory.make_RPL2();
  pulser.attach_A(imaging_pulse);
  pulser.turn_on_A();
  double imaging_voltage = 400;


  driving_pulse.turn_off();
  driving_pulse.frequency(.5e6);
  
  driving_pulse.burst_ext(edge::POSITIVE);
  driving_pulse.trigger(trigger::BUS);
  driving_pulse.burst_on();

  const double analogic_delay = 28; //microseconds
  
  const size_t repeats = 50;
  
  std::vector<double> driving_voltages;
  //   driving_voltages.push_back(80e-3);
  //driving_voltages.push_back(84e-3);
  //driving_voltages.push_back(88e-3);
  //driving_voltages.push_back(92e-3);
  //driving_voltages.push_back(96e-3);
  // driving_voltages.push_back(100e-3);
  //driving_voltages.push_back(104e-3);
  //driving_voltages.push_back(108e-3);
  //driving_voltages.push_back(112e-3);
  // driving_voltages.push_back(116e-3);
  //driving_voltages.push_back(120e-3);
  //driving_voltages.push_back(124e-3);
  //driving_voltages.push_back(128e-3);
  //driving_voltages.push_back(132e-3);
  //driving_voltages.push_back(136e-3);
  //driving_voltages.push_back(140e-3);
  driving_voltages.push_back(150e-3);

  std::vector<double> agilent_delays;
  agilent_delays.push_back(12000);
  agilent_delays.push_back(10000);
  agilent_delays.push_back(7500);
  agilent_delays.push_back(5000);
  agilent_delays.push_back(2000);
  agilent_delays.push_back(1000);
  agilent_delays.push_back(500);
  agilent_delays.push_back(250);
  agilent_delays.push_back(150);
  agilent_delays.push_back(100);
    
  //the different pressures
  //for(double driving_voltage=80e-3;driving_voltage<160e-3;driving_voltage+=4e-3)
  for(size_t dv=0;dv<driving_voltages.size();++dv)
    {
      std::cout<<"her= "<<std::endl;
      std::cout<<"her dv = "<<dv<<std::endl;
      std::cout<<"size = "<<driving_voltages.size()<<std::endl;

      //const double const_driving_voltage = driving_voltage;
      const double const_driving_voltage = driving_voltages[dv];
      std::cout<<"driving voltage = "<< const_driving_voltage<<std::endl;

      dir.mkdir(dir.get_directory()+ "/voltage_"+stringify(const_driving_voltage)); //create the subdirectory
      dir.pushd(dir.get_directory()+ "/voltage_"+stringify(const_driving_voltage));  //goto subdirectory
  
      


      driving_pulse.turn_off();
      driving_pulse.shape(shape::SIN);
      driving_pulse.cycles(10);
      driving_pulse.cycles(10);
      driving_pulse.frequency(.5e6);
      driving_pulse.voltage(const_driving_voltage); 
      driving_pulse.voltage(const_driving_voltage); 
      
      driving_pulse.phase(0);
      driving_pulse.burst_ext(edge::POSITIVE);
      driving_pulse.trigger(trigger::BUS);
      driving_pulse.burst_on();
      driving_pulse.turn_on();

      // std::cout<<"driving wave is set up - please enable the poweramp and press a key followed by enter"<<std::endl;
      // char confirm;
      // std::cin>>confirm;

      std::cout<<"phases"<<std::endl;
      phase_plot phases(dir.get_directory(), const_driving_voltage, "phases");
      phases.setup_analogic(analogic, analogic_delay);
      phases.setup_pulser(pulser, imaging_pulse, imaging_voltage);
      phases.run(driving_pulse,scope,12);

      driving_pulse.turn_off();
      //the different times
      for(size_t ad=0;ad<agilent_delays.size();++ad){  //12100
	const double agilent_delay = agilent_delays[ad];
	std::cout<<"agilent_delay = "<<agilent_delay<<std::endl;
	//0th exp (just one driving wave and imaging the tail afterwards.)


	driving_pulse.turn_off();
	driving_pulse.shape(shape::SIN);
	driving_pulse.cycles(10);
	driving_pulse.cycles(10);
	driving_pulse.frequency(.5e6);
	driving_pulse.voltage(const_driving_voltage); 
      
	driving_pulse.phase(0);
	driving_pulse.burst_ext(edge::POSITIVE);
	driving_pulse.trigger(trigger::BUS);
	driving_pulse.burst_on();
	driving_pulse.turn_on();
       
	std::cout<<"tail"<<std::endl;
	imaging_on<1> tail(dir.get_directory(), const_driving_voltage, "imaging_tail");
	tail.setup_analogic(analogic, analogic_delay, agilent_delay);
	tail.setup_pulser(pulser, imaging_pulse, imaging_voltage);
	tail.run(driving_pulse,scope,repeats);


	driving_pulse.turn_off();
	 two_sin_with_n_microsecond_gap(&driving_pulse, 0.5, agilent_delay,0.5*const_driving_voltage);
	driving_pulse.cycles(1);
	driving_pulse.cycles(2);
	driving_pulse.cycles(1);
	driving_pulse.turn_on();

	//first exp
	std::cout<<"no imag wave"<<std::endl;
	no_imaging_compare no_imag(dir.get_directory(), const_driving_voltage, "no_imaging");
	no_imag.setup_analogic(analogic, analogic_delay, agilent_delay);
	no_imag.setup_pulser(pulser, imaging_pulse, imaging_voltage);
	double xcorr = no_imag.run(driving_pulse,scope,repeats);


	//2nd exp
	std::cout<<"im 1st"<<std::endl;
	imaging_on<0> imag0(dir.get_directory(), const_driving_voltage, "imaging_1st");
	imag0.setup_analogic(analogic, analogic_delay, agilent_delay);
	imag0.setup_pulser(pulser, imaging_pulse, imaging_voltage);
	imag0.run(driving_pulse,scope,repeats);

	//3d exp
	std::cout<<"im 2nd"<<std::endl;
	imaging_on<1> imag1(dir.get_directory(), const_driving_voltage, "imaging_2nd");
	imag1.setup_analogic(analogic, analogic_delay, agilent_delay);
	imag1.setup_pulser(pulser, imaging_pulse, imaging_voltage);
	imag1.run(driving_pulse,scope,repeats);
    
    
      }
  
      std::cout<<"about to pop from "<<dir.get_directory()<<std::endl;

      dir.popd();
      std::cout<<"popped into "<<dir.get_directory()<<std::endl;

    }
    
  analogic.turn_off();
    

  pulser.turn_off_A();
  
  driving_pulse.turn_off();
}

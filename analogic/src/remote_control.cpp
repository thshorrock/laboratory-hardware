#include <map>
#include <boost/assign.hpp>
#include "analogic/remote_control.hpp"

ICR::analogic::analogic_remote_control::analogic_remote_control(const std::string& address,
								const unsigned int& baud_rate  ,
								const enum flow_control::type&   flow_control,
								const enum parity::type&         parity,
								const enum stop_bits::type&      stop_bits)
  : m_serial(address, baud_rate, flow_control, parity, stop_bits)
{
  ICR::coms::sleep(500);
}

ICR::analogic::analogic_remote_control::~analogic_remote_control()
{}

// //initialise the static
// std::map<const std::string, float>
// ICR::analogic::clock_rate::m_value = boost::assign::map_list_of
//   ("1.25ns", 1.25e-9)
//   ("2.5ns" , )
//   ( "5ns", )
//   ( "10ns", )
//   ( "20ns", )
//   ( "50ns", )
//   ( "100ns", )
//   ( "200ns", )
//   ( "500ns", )
//   ( "1us", )
//   ( "2us", )
//   ( "5us", )
//   ( "10us", )
//   ( "100us", )
//   ( "1ms", )
//   ( "2ms", )
//   ( "5ms", )
//   ( "20ms", )
//   ( "50ms", )
//   ( "100ms", )
//   ( "1s", )

//   ;

// ICR::analogic::clock_rate::clock_rate()
//       {
// 	// m_value["1.25ns"] = 1.25e-9;
// 	// m_value["2.5ns"] = 2.5e-9;
// 	// m_value["5ns"] = 5e-9;
// 	// m_value["10ns"] = 10-9;
// 	// m_value["20ns"] = 20e-9;
// 	// m_value["50ns"] = 50e-9;
// 	// m_value["100ns"] = 100e-9;
// 	// m_value["200ns"] = 200e-9;
// 	// m_value["500ns"] = 500e-9;
// 	// m_value["1us"] = 1e-6;
// 	// m_value["2us"] = 2e-6;
// 	// m_value["5us"] = 5e-6;
// 	// m_value["10us"] = 10e-6;
// 	// m_value["100us"] = 100e-6;
// 	// m_value["1ms"] = 1e-3;
// 	// m_value["2ms"] = 2e-3;
// 	// m_value["5ms"] = 5e-3;
// 	// m_value["10ms"] = 10e-3;
// 	// m_value["20ms"] = 20e-3;
// 	// m_value["50ms"] = 50e-3;
// 	// m_value["100ms"] = 100e-3;
// 	// m_value["1s"] = 1.0;
//       }


#include "stdafx.h"
#include "FPGA.hpp"
//#include <iostream>
//#include <boost/timer.hpp>
//System::IO::Ports
//using namespace System::IO::Ports;
using namespace benchtop;
namespace bt = benchtop;

bt::agilent_class::agilent_class()
  :

 
    io(),
    SerialPort(io),
#ifdef _MSC_VER
    serial_port_name("COM1"),
#else 
    serial_port_name("/dev/ttyS0"),
#endif


    //gui
    tblHIFU1(2,1,false),
    tblHIFU2(1,2,false),
    
	     
    /* Motor Stepping */
    frmSequence("Sequence"),
    algSequence(0,0,0,0),
    tblSequence(3,2,false),
    lblNoReferenceFrames(  "<small> # reference frames: </small>"), 
    lblNoFramesNoBurst(    "<small> # frames without burst: </small>"), 
    lblHIFUBurst(          "<small> # HIFU bursts: </small>"),
    txtNoReferenceFrames(), txtNoFramesNoBurst(), txtHIFUBurst(),
    
    /* Positioning */
    frmPulse("Pulse"),
    algPulse(0,0,0,0),
    tblPulse(4,3,false),
    lblDelayTime(        "<small> delay time </small>"), 
    lblFirst(            "<small> first </small>"), 
    lblIncrement(        "<small> increment </small>"), 
    lblLast(             "<small> last </small>"), 
    lblPulseLengthHIFU(  "<small> Pulse Length HIFU </small>"),
    txtFirst(), txtIncrement(), txtLast(), txtPulseLengthHIFU(),

    bbHIFU(),
    //bbHIFU(5,1,false),
    cmdConnect("connect"),
    cmdSet("set"), 
    cmdInitialise("initialise"), 
    cmdStart("start"), 
    cmdStop("stop") 

    
{
  connect();
};




bt::agilent_class::~agilent_class()
{

  // stop HIFU and acquisition sequence
  close();
};

void 
bt::agilent_class::write_command(const string cmd)
{
  if (!m_is_connected) {connect();};
  write_with_timeout(SerialPort, boost::asio::buffer(cmd.c_str(), cmd.size() ) );
}

void
bt::agilent_class::connect()
{

  SerialPort.open(serial_port_name);
  SerialPort.set_option(boost::asio::serial_port::baud_rate(9600) ); 
  SerialPort.set_option(
			boost::asio::serial_port::flow_control(
							       boost::asio::serial_port::flow_control::hardware 
							       ) 
			);
  boost::timer time;
  time.restart();//start the clock
  double timeout = 5.0;
  while(!SerialPort.is_open()){
    if (time.elapsed()>timeout) throw benchtop::error::TimeoutException();
  };
};




void
bt::agilent_class::open(){
  close(); //if running
};


void
bt::agilent_class::close(){
  // start HIFU and acquisition sequence

 write_with_timeout(SerialPort, boost::asio::buffer(strCommand.c_str(), strCommand.size() ) );
};



/*  gui   */
void
bt::agilent_class::gui(Gtk::Notebook& notebook){
  
  Gtk::VBox      vboxHIFU;
  Gtk::VBox      hboxHIFU;
  
  /* sequence */
  /* set frame details */
  frmSequence.set_label_align();
  frmSequence.set_shadow_type(Gtk::SHADOW_OUT);
  /* set label details */
  lblNoReferenceFrames.set_use_markup(true);
  lblNoFramesNoBurst.set_use_markup(true);
  lblHIFUBurst.set_use_markup(true);
  txtNoReferenceFrames.set_text("1");
  txtNoFramesNoBurst.set_text("1");
  txtHIFUBurst.set_text("1");
  /* table packing */
  tblSequence.attach( lblNoReferenceFrames, 0, 1, 0, 1);
  tblSequence.attach( txtNoReferenceFrames, 1, 2, 0, 1);
  tblSequence.attach( lblNoFramesNoBurst  , 0, 1, 1, 2);
  tblSequence.attach( txtNoFramesNoBurst  , 1, 2, 1, 2);
  tblSequence.attach( lblHIFUBurst        , 0, 1, 2, 3);
  tblSequence.attach( txtHIFUBurst        , 1, 2, 2, 3);
  /* add to frame */
  frmSequence.add(algSequence);
  algSequence.add(tblSequence);

  //frmSequence.show_all_children();

  /* pulse */
  /* set frame details */
  frmPulse.set_label_align();
  frmPulse.set_shadow_type(Gtk::SHADOW_OUT);
  /* set label details */
  lblDelayTime.set_use_markup(true);
  lblFirst.set_use_markup(true);
  lblIncrement.set_use_markup(true);
  lblLast.set_use_markup(true);
  lblPulseLengthHIFU.set_use_markup(true);
  txtFirst.set_text("4");
  txtIncrement.set_text("1");
  txtLast.set_text("4");
  txtPulseLengthHIFU.set_text("8");


  /* table packing */
  tblPulse.attach(lblDelayTime       , 0, 1, 0, 1);
  tblPulse.attach(lblFirst           , 1, 2, 0, 1);
  tblPulse.attach(txtFirst           , 2, 3, 0, 1);
  tblPulse.attach(lblIncrement       , 1, 2, 1, 2);
  tblPulse.attach(txtIncrement       , 2, 3, 1, 2);
  tblPulse.attach(lblLast            , 1, 2, 2, 3);
  tblPulse.attach(txtLast            , 2, 3, 2, 3);
  tblPulse.attach(lblPulseLengthHIFU , 1, 2, 3, 4);
  tblPulse.attach(txtPulseLengthHIFU , 2, 3, 3, 4);

  /* add to frame */
  frmPulse.add(algPulse);
  algPulse.add(tblPulse);

  //frmPulse.show_all_children();

  /* add the command boxes */
 //  bbHIFU.attach( cmdConnect     , 0, 1, 0, 1  );
//   bbHIFU.attach( cmdSet         , 0, 1, 1, 2  );
//   bbHIFU.attach( cmdInitialise  , 0, 1, 2, 3  );
//   bbHIFU.attach( cmdStart       , 0, 1, 3, 4  );
//   bbHIFU.attach( cmdStop        , 0, 1, 4, 5  );

  bbHIFU.pack_start(cmdConnect );
  bbHIFU.pack_start(cmdSet );
  bbHIFU.pack_start(cmdInitialise );
  bbHIFU.pack_start(cmdStart );
  bbHIFU.pack_start(cmdStop );
  
  //vboxHIFU.pack_start(frmSequence);
  //vboxHIFU.pack_start(frmPulse);
  //vboxHIFU.show_all_children();
  //hboxHIFU.pack_start(vboxHIFU);
  //boxHIFU.pack_start(bbHIFU);
  
  tblHIFU1.attach(frmSequence, 0,1,0,1);
  tblHIFU1.attach(frmPulse   , 0,1,1,2);

  tblHIFU2.attach(tblHIFU1, 0,1,0,1);
  tblHIFU2.attach(bbHIFU  , 1,2,0,1);
  
  notebook.append_page(tblHIFU2, "HIFU control"); //hboxHIFUfrmPulse

  
};



/*! \file agilent_33120A.hpp 
 *
 * \brief Where the agilent_33120A_class is defined.
 *
 * Contains the interface to the FPGA card.
 */
#pragma once
#include "stdafx.h"
#include "error.hpp"
#include "handy.hpp"
using namespace handy;

namespace benchtop{


  namespace agilent{
    
    namespace params {
      const int READ_BUFFER_SIZE = 128;
      const int MAX_DATA_LENGTH = 16;
      const int MAX_RESPONSE_TIME = 250;
      const int LINE_LENGTH = 16;
      const int MAX_PACKET_LENGTH = MAX_DATA_LENGTH + 4;
      const unsigned char NORMAL_RESPONSE = 0x40;
      const unsigned char NORMAL_REPORT = 0x80;
      const unsigned char ERROR_RESPONSE = 0xC0;
      const unsigned char KEY_ACTIVITY_REPORT = 0x80;
      const unsigned char FAN_SPEED_REPORT = 0x81;
      const unsigned char TEMPATURE_SENSOR_REPORT = 0x82;
      const unsigned short CRC_SEED = 0xFFFF;
      //defination related to FPGA card
      const double BCLK = 8.00526;
    };
    namespace operation_mode {
      enum {  OP_MODE0 = 0,
	      OP_MODE1 = 1,
	      OP_MODE2 = 2,
	      OP_MODE3 = 3,
	      OP_MODE4 = 4,
	      OP_MODE5 = 5,
	      OP_MODE6 = 6,
	      OP_MODE7 = 7,
      };
    }
    namespace control_register{
      const unsigned int BIT_SYS_EN0 = 0;
      const unsigned int BIT_T5_GATE = 1;
      const unsigned int BIT_T6_GATE = 2;
      const unsigned int BIT_WITH_COOLING = 3;
      const unsigned int BIT_OP_MD_SL0 = 4;
      const unsigned int BIT_OP_MD_SL1 = 5;
      const unsigned int BIT_OP_MD_SL2 = 6;
      const unsigned int BIT_NO_REF_FRM = 7;
      const unsigned int BIT_CAPTURE_ALL = 8;
      const unsigned int BIT_FSS_TYPE_0 = 9; // 00 for Zonare input, 01 for Acuson (negative pulse) input,
      const unsigned int BIT_FSS_TYPE_1 = 10 ;//  10 for positive pulse input, 11 for nothing
      const unsigned int BIT_RESET_IRQ = 11; //  0 keep it, 1 reset
      const unsigned int BIT_SG_START = 12; //  0 keep it, 1 reset
      const unsigned int BIT_MC_START = 13; //  0 keep it, 1 reset
      const unsigned int BIT_MEA_START = 14; //  0 keep it, 1 reset
      const unsigned int BIT_EN_START = 15; //  0 keep it, 1 reset

      //STATUS REG1
      const unsigned int BIT_SIG_T0_GATE = 0;
      const unsigned int BIT_SIG_T1_GATE = 1;
      const unsigned int BIT_SIG_T2_GATE = 2;
      const unsigned int BIT_SIG_T3_GATE = 3;
      const unsigned int BIT_SIG_T4_GATE = 4;
      const unsigned int BIT_SIG_T5_GATE = 5;
      const unsigned int BIT_SIG_T6_GATE = 6;

      //STATUS REG2
      const unsigned int BIT_SIG_EN0 = 0;
      const unsigned int BIT_SIG_EN_HIFU_CNT = 1;
      const unsigned int BIT_SIG_IRQ = 2;;
      const unsigned int BIT_SIG_T0_Ok = 3;
      const unsigned int BIT_SIG_T1_Ok = 4;;
      const unsigned int BIT_SIG_T2_Ok = 5;
      const unsigned int BIT_SIG_T_Ok  = 6;
      const unsigned int BIT_SIG_REF_OK1  = 7;
      const unsigned int BIT_SIG_REF_OK2  = 8;
      const unsigned int BIT_SIG_PULSES_OK  = 9;
      const unsigned int BIT_SG_OK  = 10;
      const unsigned int BIT_MC_OK  = 11;
      const unsigned int BIT_MEA_OK  = 12;
      const unsigned int BIT_SIG_AMP_ON  = 13;
      const unsigned int BIT_SIG_AMP_ON_PRECONDI  = 14;
      const unsigned int BIT_SIG_IMAGE_CAPTURE  = 15;
      
    };
    namespace commands{
      //DEFINE COMMANDS
      /* This is ugly but cannot think of a better way while still
       * using the System calls to access serial connection
       * would like to move onto using Boost::asio at somepoint */

      const tstring  CMD_BASE_ADDR = "BADD"; //command 17, base address
      const tstring  CMD_CTL_REG = "CREG"; //command 0, control register value
      const tstring  CMD_FRM_NUM = "FNUM"; //command 1, frame numbers
      const tstring  CMD_IMA_PUL = "IPUL"; //command 2, image pulses
      const tstring  CMD_T0_SET = "TASE"; //command 3, T0 Set
      const tstring  CMD_T1_SET = "TBSE"; //command 4, T1 Set
      const tstring  CMD_T2_SET = "TCSE"; //command 5, T2 Set
      const tstring  CMD_T3_SET = "TDSE"; //command 6, T3 Set
      const tstring  CMD_T4_SET = "TESE"; //command 7, T4 Set
      const tstring  CMD_T5_SET = "TFSE"; //command 8, T5 Set
      const tstring  CMD_T6_SET = "TGSE"; //command 9, T6 Set
      const tstring  CMD_T7_SET = "THSE"; //command 22, T7 Set
      const tstring  CMD_AMP_CH_ON = "ACEN"; //command 10, Amp Ch On Set
      const tstring  CMD_AMP_CH_SEL = "ACSL"; //command 11, Amp Ch Freq Set
      const tstring  CMD_INIT_FPGA = "INIT"; //command 12, initialize FPGA board
      const tstring  CMD_START_OP = "STAT";//command 13, start operation
      const tstring  CMD_STOP_OP = "STOP"; //command 14, stop operation
      const tstring  CMD_FPGA_READY = "REDY";//command 15, FPGA board is ready (feed back command to host PC)
      const tstring  CMD_EXIT_APP = "EXIT"; //command 16, exit aim104-386 application programme
      const tstring  CMD_REBOOT = "REBT"; //command 17, exit aim104-386 application programme
      const tstring  CMD_OP_INFO = "INFO"; //command 18, send operation info to master PC
      const tstring  CMD_RES_IRQ = "RIRQ"; //command 19, Reset IRQ and open IRQ line afterwards
      const tstring  CMD_STATUS_A =  "STAA"; //command 20, status register A
      const tstring  CMD_STATUS_B = "STAB"; //command 21, status register B
      const unsigned int  NUM_INFO = 16;//total 16 pices information read from AIM386ex


    };
    
  };
};


namespace benchtop{

  using namespace agilent;


  class LIB_CLASS agilent_33120A_class{
  private:

    


//    gcroot<System::IO::Ports::SerialPort ^> com;
#ifdef _MSC_VER
    
    CSerial com;
#else
    boost::asio::io_service io;
    boost::asio::serial_port SerialPort;
#endif
    string serial_port_name;
    //! Reconnect to the agilent
    void connect();
    
    void set_variables();
  public:
    //! Constructor
     FPGA_class();
    //! Destructor
     ~FPGA_class();
     
    
    //! Start the FPGA card
    void open();
    //! Stop the FPGA card
    void close();
    //start the fpga run
    void run();
      
    //! Set the resolution
    void set_resolution(int);
    //! Set the delay
    void set_delay(double);
    //! Set the burst length 
    void set_burst_length(double);
    //! Set the number of reference frames
    void set_number_reference_frames(int);
    //! Set the number or frames without any burst
    void set_number_frames_without_burst(int);
    //! Set the number of bursts
    void set_number_bursts(int);
    

    //    void configure() {};
    //! Add the FPGA tab to the notebook of benchtop_system.
    void gui(Gtk::Notebook&);

  private:
      /* gui */
    
    Gtk::Table     tblHIFU1, tblHIFU2;

    /* sequence */
    Gtk::Frame     frmSequence;
    Gtk::Alignment algSequence;
    Gtk::Table     tblSequence;
    Gtk::Label     lblNoReferenceFrames, lblNoFramesNoBurst, lblHIFUBurst;
    Gtk::Entry     txtNoReferenceFrames, txtNoFramesNoBurst, txtHIFUBurst;
    
    /* pulse */
    Gtk::Frame     frmPulse;
    Gtk::Alignment algPulse;
    Gtk::Table     tblPulse;
    Gtk::Label     lblDelayTime, lblFirst, lblIncrement, lblLast, lblPulseLengthHIFU;
    Gtk::Entry     txtFirst, txtIncrement, txtLast, txtPulseLengthHIFU;

    /* command boxes  */
    Gtk::VButtonBox     bbHIFU;
    Gtk::Button    cmdConnect, cmdSet, cmdInitialise, cmdStart, cmdStop;

    void ConnectClicked();
    void SetClicked();
    void InitialiseClicked();
    void StartClicked();
    void StopClicked();
    

    
  }; 









 //FPGA_class
  /*IMPLEMENTATION*/
  inline  void FPGA_class::set_resolution(int resolution_){resolution = resolution_;set_variables();};
  inline  void FPGA_class::set_delay(double delay_ ){ delay = delay_ ;set_variables();};
  inline  void FPGA_class::set_burst_length(double burst_length_){ burst_length =  burst_length_;  set_variables(); };
  inline  void FPGA_class::set_number_reference_frames(int numReferenceFrames_){ numReferenceFrames =  numReferenceFrames_; set_variables();};
  inline  void FPGA_class::set_number_frames_without_burst(int numFramesNoBurst_){ numFramesNoBurst =  numFramesNoBurst_; set_variables();};
  inline  void FPGA_class::set_number_bursts(int numBursts_){ numBursts =  numBursts_;set_variables();};

  

}//namespace benchtop


#include "agilent_com.hpp"

namespace ICR {
  namespace agilent{
    template<class com_method>
    class  WG33220A : public agilent_com<com_method>
    {
    public:
      WG33220A(const std::string& device) : agilent_com<com_method>(device) {};
      using agilent_com<com_method>::send;
      using agilent_com<com_method>::recv;
      using agilent_com<com_method>::apply;
      using agilent_com<com_method>::load;
      using agilent_com<com_method>::duty_cycle;
      using agilent_com<com_method>::frequency;
      using agilent_com<com_method>::voltage;
      using agilent_com<com_method>::offset;
      using agilent_com<com_method>::cycles;
      using agilent_com<com_method>::burst_on;
      using agilent_com<com_method>::burst_off;
      using agilent_com<com_method>::phase;
      using agilent_com<com_method>::burst_int;
      using agilent_com<com_method>::burst_ext;
      using agilent_com<com_method>::trigger;
      using agilent_com<com_method>::trigger_now;
      using agilent_com<com_method>::shape;
      using agilent_com<com_method>::arb_wave;
      using agilent_com<com_method>::reset;
      using agilent_com<com_method>::error;
    };
    // class WG33220A : public coms::IPmanager {
    // public:
    //   WG33220A(const std::string& IPaddress);
    //   virtual ~WG33220A() ;

    //   using coms::IPmanager::send;
    //   using coms::IPmanager::recv;
      
    //   std::string error();
    //   void apply(const int shape, const double freq,const  double volts,const  double offset);
      
    //   void load(const int);
    //   void duty_cycle(const double); // square wave only

    //   void frequency(const double frequency);
    //   void voltage(const double V);
    //   void offset(const double offset);

    //   //burst commands
    //   void cycles(const unsigned int cycles);
    //   void burst_on(void);
    //   void burst_off(void);
    //   void phase(const double degrees);
    //   void burst_int(const  double  rate);
    //   void burst_ext();

    //   void trigger(const int source);
    //   void trigger_now() {send("*TRG");};

    //   void shape (const int);
    //   void arb_wave(const std::string name, const float*, const unsigned long size);
    //   //void arb_wave(vector<double> data);

    //   void reset();

    // };

  }
}

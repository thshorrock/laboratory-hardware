#include <string>

namespace ICR {
  namespace analogic {
    
    namespace details {
      struct clock_period
      {
	
      };
      struct offset
      {

      };
      struct signal_amp_pkpk
      {

      };
      struct noise_amp 
      {

      };
      struct noise_bandwidth
      {

      };
      struct filter_cutoff
      {

      };
      struct trigger_mode{

      };
    }

    class main_header{

      
    public:
      main_header();
      std::string get() const;
    };
    
  }
}

std::string
ICR::analogic::main_header::get() const
{
  

}

#pragma once

namespace ICR {
  namespace analogic {
    
    namespace detail {
      struct pod_main_header
      {
	float clock_period;
	float offset;
	float signal_amp;
	float noise_amp;
	float noise_bandwidth;
	float filter_cutoff;
	short mode;
      };

      union main_head_pack
      {
	char str[32];
	pod_main_header header;
      };
    }
  }
}

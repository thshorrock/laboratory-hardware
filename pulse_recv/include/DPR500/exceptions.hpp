#pragma once
namespace ICR{

  namespace pulser{
    /** The exceptions namespace */
    namespace exceptions{
      struct DPR500_exception{};
      /** Invalid Query Exception. */
      struct invalid_query :  public DPR500_exception {};
      /** Failed to contact the DPR500 device. */
      struct failed_to_contact_DPR500_device : public DPR500_exception {};
      /** pulser receiver not attached to the  DPR500 device. */
      struct pulser_receiver_not_attached_to_DPR500 : public DPR500_exception {};
      /** Incorrect input to gain command */
      struct incorrect_input_to_gain_command : public DPR500_exception {};
      /** Requested gain out of bounds */
      struct requested_gain_out_of_bounds : public DPR500_exception {};

       /** Requested voltage out of bounds */
      struct  voltage_out_of_bounds : public DPR500_exception {};
       /** No device attached to channel A */
      struct  nothing_attached_to_A : public DPR500_exception {};
       /** No device attached to channel B */
      struct  nothing_attached_to_B: public DPR500_exception {};
       /** Wrong serial number for pulser.
	* Maybe you attached the wrong pulser to the channel
	*/
      struct incorrect_serial_number_for_pulser : public DPR500_exception {};

    }
  }
}

#include "DPR500/DPR500.hpp"

/**
 * @mainpage DPR500 Documentation
 * @author Tom Shorrock (t.h.shorrock@gmail.com)
 * @date   January, 2011
 * @section purpose_sec Purpose
 * 
 * Provide a cross platform c++ interface to the DPR500 pulser receiver.
 *
 * This is provided curtosy of the ICR::pulser::DPR500 class, (the amplifier)
 * and the pulser-receivers provided by the ICR::pulser::PulserReceiverFactory.
 * 
 *
 *
 * @section example_sec Example:
@code
#include "DPR500.hpp"
int
main  (int ac, char **av)
{
  //use the pulser namespace
  using namespace ICR::pulser;
  
  //Connect to the DPR500 via the serial port "/dev/ttyUSB1"
  DPR500 pulser("/dev/ttyUSB1");
  
  //create a Pulser Receiver factory for constructing the pulser-recievers
  PulserReceiverFactory factory;
  
  //Construct a pointer to the RPL2
  RPL2* imaging_pulse = factory.make_RPL2();

  //set various properties
  imaging_pulse->set_external_trigger();
  imaging_pulse->set_pulser_mode(damping_RPL2::fourtyfour_Ohm,
				 energy_policy::HIGH,
				 receive_mode::PULSE_ECHO);
  imaging_pulse->set_voltage(300);

  //attach to the DPR500 via channel A
  pulser.attach_A(imaging_pulse);
  
  //set various properties of the amplifier
  pulser.set_high_pass_filter_A(filter::HPFA::sevenpointfive_MHz);
  pulser.set_low_pass_filter_A(filter::LPFA::fifty_MHz);
  pulser.set_gain_A(imaging_gain);

  //notify the pulser-reciever of these changes 
  // NOTE: This function must always be called when you want 
  //      to commit changes 
  pulser.notify();

  //turn on
  pulser.turn_on_A();
  
  //... waits for triggering here and fires when triggered
  
  //turn of
  pulser.turn_off_A();
}
@endcode

\section feature_sec Feature
A swig interface file is provided for this library.
This means that a wrapper to the c++ library is easily created for any of the following languages:
 - AllegroCL
 - C# - Mono
 - C# - MS .NET
 - CFFI
 - CHICKEN
 - CLISP
 - Go language
 - Guile
 - Java
 - Lua
 - MzScheme
 - Ocaml
 - Octave (and Matlab via Octave)
 - Perl
 - PHP
 - Python
 - R
 - Ruby
 - Tcl/Tk


\section dependancies_sec Dependancies.
 This library uses the Boost c++ libraries.  These are already included in the distribution.




\section git_repo_sec Location of the Repository

The library source code is stored in a git repository.
For more information on how to get up and running with git please see the wiki page
http://us-physics/wiki/Git

The Git repository is located at 
@verbatim
/mirror/progs/hardware/.git
@endverbatim
To clone the repository type in a terminal ( or in the git bash terminal if you are using windows):
@code
git clone cu@us-physics:/mirror/progs/hardware/.git
@endcode
This will install a folder called "hardware" at the location in which you typed the command.



\section installation_sec Installation

This library is most conveniently installed with bjam.
Bjam is a cross platform installation tool.

For more help with bjam please see the wiki
http://us-physics/wiki/Bjam

\subsection windows_install_sec Windows

A precomiled Windows executable is included in the repository.
To use it open up a DOS command prompt (not the git bash terminal) 
and go to windows directory that is within the cloned  "hardware" directory. For example
@code
cd C:\progs\hardware\windows
@endcode
Then simply type
@code
bjam ../
@endcode
This should install a debug version of the libraries in the folder
@verbatim
C:\progs\hardware\lib
@endverbatim

You may pass additional options to bjam. For example
@code
bjam release toolset=msvc -j2 ../
@endcode
specifies that 
  - the release rather than the debug version of the library (profile is also a valid option)
  - the microsoft compiler should be used.  You can change this if you have other compilers on your system
  - the compilation should be carried out in 2 threads.

\subsection linux_install_sec linux
On Debian systems obtaining bjam is trivial, and so it is not included in the repository.
Simply type
@code
sudo apt-get install bjam
@endcode
Then go to the "hardware" folder in which the repository is located and type, for example, 
@code
bjam release toolset=gcc
@endcode

@section bugs_sec Bugs

If you find a bug or would like to request a feature, then please use the bug tracking tool located at
http://us-physics:3000/projects/show/hardware




@section licence_sec Licence
Since the library uses the Boost c++ libraries, the Boost lisence applies.

\verbatim
Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
\endverbatim
*/

  /** @file /include/DPR500.hpp
 *  @brief The DPR500 header files are specified
 *
 */

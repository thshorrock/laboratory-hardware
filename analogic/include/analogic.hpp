#pragma once

#include "analogic/remote_control.hpp"
#include "analogic/construct_data/segment.hpp"


/**
 * @mainpage Communication Manager Documentation.
 * @author Tom Shorrock (t.h.shorrock@gmail.com)
 * @date   January, 2011
 * @section purpose_sec Purpose
 * 
 * Provide a cross platform c++ interface to the analogic polynomial waveform generator (2045)
 *
 * This is provided curtosy of the ICR::analogic::analogic_remote_control class
 *
 * @section example_sec Example:
@code
#include "coms_manager.hpp"
int
main  (int ac, char **av)
{
  //use the analogic namespace
  using namespace ICR::analogic;
  
  //Open a serial connection to a device attached to "COM1" port
  analogic_remote_control analogic("COM1");
  
  analogic.expression("AT TRIG FOR 28u 0.0 FOR 2u 0.5 FOR 1u 0 CLK = 1.25n MARK = 20u");  //create a pulse
  analogic.turn_on();  //run

  
  //The connections automatically close when out of scope.
}
@endcode

\section not_working_sec In case it is not working.
Sometimes to get the link to the serial port working at all you need to reset the RS232 connection options set in the analogic (even if it seems to be set correctly).

To do this 
  - unplug the RS232 cable from the back of the analogic.
  - Click the OP 4 command (with the buttons on the front of the 2045)
  - Scroll to CMD DEVICE=SER0: option.
  - move it away from CMD DEVICE=SER0: and then back again.
  - Make sure the output is OFF.
  - Plug in the RS232 cable into the RS 232 DCE slot (no null terminating cable)
  - run ./bin/test_anologic_1 (this should turn the output on).



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

@section usage_sec Using the Library.

Your operating system know where the library is located.
For help with this, please see the wiki at
http://us-physics/wiki/Library_path




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

  /** @file /include/analogic.hpp
 *  @brief The analogic header files are specified and the documentation is writen.
 *
 */

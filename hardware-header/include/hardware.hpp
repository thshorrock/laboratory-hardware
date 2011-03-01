#include "lecroy.hpp"
#include "DPR500.hpp"
#include "agilent.hpp"

/**
 * @mainpage Hardware Documentation
 * @author Tom Shorrock (t.h.shorrock@gmail.com)
 * @date   December, 2010
 * @section purpose_sec Purpose
 * 
 * Provide a cross platform c++ interface to the Lecroy Waverunner oscilloscope, the Agilent waveform generators
 * and the DPR500 pulser-receiver.
 *
 * Please see the individual project pages for more information.
 * @htmlonly
 * <UL>
 * <LI> <a href="./com_manager/index.html"> General Communication Manager</a>
 * <LI> <a href="./lecroy/index.html"> Lecroy</a>
 * <LI> <a href="./DPR500/index.html"> DPR500</a>
 * <LI> <a href="./agilent/index.html"> Agilent waveform generators</a>
 * <LI> <a href="./analogic/index.html"> Analogic 2045</a>
 * </UL>
 *
 * The following auxillary libraries can also be helpful
 * <UL>
 * <LI> <a href="./stringify/index.html"> Stringify </a>
 * <LI> <a href="./filename/index.html"> File management </a>
 * <LI> <a href="./readme/index.html"> Create a README file </a>
 * </UL>
 * @endhtmlonly
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

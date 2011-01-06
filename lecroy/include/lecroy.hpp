#include "lecroy/lecroy_com.hpp"

/**
 * @mainpage Lecroy Documentation
 * @author Tom Shorrock (t.h.shorrock@gmail.com)
 * @date   January, 2011
 * @section purpose_sec Purpose
 * 
 * Provide a cross platform c++ interface to the Lecroy Waverunner oscilloscope.
 *
 * This is provided curtosy of the ICR::lecroy::lecroy_com class.
 *
 * @section example_sec Example:
@code
#include "lecroy.hpp"
int
main  (int ac, char **av)
{
  //use the Lecroy namespace
  using namespace ICR::lecroy;
  
  // Open a connection to the lecroy scope by creating a lecroy_com object.
  // The string  containes is the IP address of the scope, which is in this example 193.62.12.322.
  lecroy_com lc("193.62.12.322"); 
  
  //set channel 1 to 50 ohm DC coupling      
  lc.set_coupling(location::C1, coupling::D50); 

  //The connection automatically closes when lc runs out of scope.
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

The interface to the python library is automatically compiled.
The following python script repeates the previous example.
@code
  #!/usr/bin/python                #Identify where to find python. 
  from hardware_lecroy import *    #import the module named hardware_lecroy
  lc = lecroy_com("193.62.12.322") #open the connection
  lc.set_coupling(C1,D50)          #set the coupling
@endcode


\section dependancies_sec Dependancies.
 This library uses the Boost c++ libraries.  These are already included in the distribution.

\section git_repo_sec Location of the Repository

The Git repository is located at 
@verbatim
/mirror/progs/hardware/.git
@endverbatim
To clone the repository type:
@code
git clone cu@us-physics /mirror/progs/hardware/.git
@endcode

For more information on how to get up and running with git please see the wiki page
http://us-physics/wiki/Git

\section installation_sec Installation

This library is most conveniently installed with bjam (on both Windows and Linux based operating systems).
 - First setup bjam (which can be downloaded from http://www.boost.org/)
 - go to the root directory and type:
  \code bjam --v2 \endcode

For more help with bjam please see the wiki
http://us-physics/wiki/Bjam

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

  /** @file lecroy.hpp
 *  @brief The lecroy header files are specified
 *
 */

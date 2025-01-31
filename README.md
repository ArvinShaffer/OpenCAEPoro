# OpenCAEPoro

OpenCAEPoro is part of the OpenCAEPlus project written in C++. It focuses on 
multicomponent multiphase flow simulation in porous media. For more info, 
please refer to: https://faspdevteam.github.io/OpenCAEPoro/.

## Install
There is a top level cmake for configuration and building of the OpenCAEPoro 
library and the test programs suite. You can use a cmake-style approach to 
compile the package; see https://cmake.org on how to use cmake for your own 
operating system. The typical command for compiling OpenCAEPoro is:

Config the environment for building with cmake:
```bash
  >>> mkdir Build; cd Build; cmake ..
```

After succesfully configing, just make the library as well as examples:
```bash
  >>> make
```

To make the library and install it, run:
```bash
  >>> make install
```

## Structure
The directory structure of OpenCAEPoro is designed as follows:
  - data: Input files for test examples
  - docs: Html files automatically generated by Doxygen
  - include: Header files
  - src: Source files
  - main: Main source code 
  - manual: User manual and design considerations
  - CMakelist.txt: Main cmake script
  - LICENSE: LGPL license agreement
  - README.md: This document

## License
This software is free software distributed under the Lesser General Public
License or LGPL, version 3.0 or any later versions. This software distributed
in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with OpenCAEPoro. If not, see <http://www.gnu.org/licenses/>.

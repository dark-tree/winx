## Window Management Library
WINX is a simple to use, minimal, cross-platform and single-header window management C library designed to easily create an OpenGL 3.0+ compatible window on both Windows and Linux (as well as on MacOS through X11).

#### Usage
To use WINX compile the single source file `winx.c` with the `-lGL -lX11` options on Linux and `-lopengl32 -lgdi32` on Windows, after that include the `winx.h`
header anywhere you need access to WINX functions, and link the object files. For small single-file programs you can also directly include the `winx.c` source file -
but note that this is not recommended as it sometimes causes issues with other libraries such as GLAD if done incorrectly. If you plan on using both GLAD and WINX
together consider the combined library [WXGL](wxgl/).

#### Example
To see WINX in action run the `build.sh` (or `./build.bat` on Windows) script to compile and execute a simple OpenGL program using a WINX created context and window.

#### Docs
Documentation describing the basics of using WINX can be found at the beginning of the `winx.h` header file, just under the license.

#### License
Boost Software License - Version 1.0 - August 17th, 2003

## WXGL
WXGL is a single-header library made from combining WINX and GLAD, to use WXGL follow those steps:

- Go to https://glad.dav1d.de and generate GLAD zip file. **Important**: Select the "Local Files" checkbox at the bottom of the page!

- Run `python3 generate.py --glad "/path/to/your/glad.zip"` from the `./wxgl` directory.  

The `wxgl.c` and `wxgl.h` files should have been generated in the `wxgl` directory, now you can take them and use anywhere! **Note**: The `wxgl.c` has to be compiled with the `-ldl -lGL -lX11` options on Linux and `-lopengl32 -lgdi32` on Windows.

Advantage of doing this is an even further simplification of the process of starting a new OpenGL project. **Note**: When using WXGL you don't need to call `gladLoadGL()` this is now done from within `winxOpen()`.

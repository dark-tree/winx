gcc -g -Wall -Wextra -Wno-unused-parameter -Wno-cast-function-type -O0 winx.c example/main.c example/glad/glad.c -I. -lopengl32 -lgdi32 -o main.exe && main.exe

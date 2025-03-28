# Simple CLAY (C Layout) Initial Project

- Careful if you use this you must get original clay.h and your renderer
- See them at https://github.com/nicbarker/clay

# Rendering and compiling

- If you know your renderer, you probably already know how to compile your project
- If you don't you have to get the Source Code / Libraries / DLL etc and its header
- You can download your renderer at https://github.com/nicbarker/clay/tree/main/renderers
- Actual projet supports [SDL2, SDL3, cairo, raylib, web, win32_gdi]
- Do not forget to set its path in `main.c`
- Put your libs in lib folder and their headers in include folder

# Using Raylib

- Actual project is to be compiled with raylib and can quickly be compiled with `nob.c`
- Nob comes from https://github.com/tsoding/nob.h
- Compile it ONCE ONLY with your favourite C compiler, I Personnaly use gcc
```shell
gcc -o nob nob.c
```

# Other renderers

// TODO
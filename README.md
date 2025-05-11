# CLI File Explorer

A simple terminal-based file explorer that allows you to navigate directories and locate files using a keyboard interface. Built using the `ncursesw` library for a text-based UI.

## Features

- Navigate through directories
- View files and subfolders
- Lightweight and minimal
- Works in the terminal

## Requirements

- C compiler (e.g., GCC or Clang)
- CMake ≥ 4.0.1
- `ncursesw` library (wide-character version of ncurses)

## Build Instructions

### Windows (MSYS2/MinGW64)

1. Make sure you have MSYS2 installed and open the MinGW64 shell.
2. Install the required packages (if not already installed):

```sh
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ncurses
```

3. Clone the repository:

```sh
git clone https://github.com/findof1/file-explorer.git
cd file-explorer
```

4. Create a build directory and compile the project:

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

5. Run the program:

```sh
./file-explorer
```

## Project Structure

```sh
file-explorer/
├── CMakeLists.txt
├── CMakePresets.json
├── include/
│   └── fileUtils.h
├── src/
│   ├── main.c
│   └── fileUtils.c
└── build/ (generated during compilation)
```

## Notes

- The include paths in `CMakeLists.txt` are currently hardcoded for MSYS2. Modify them if you're building on a different platform.
- This tool is meant for simple navigation and file discovery, not file manipulation (copy/move/delete).
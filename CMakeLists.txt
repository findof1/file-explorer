cmake_minimum_required(VERSION 4.0.1)
project(fileExplorer VERSION 0.1.0 LANGUAGES C)

add_executable(fileExplorer main.c fileUtils.c)

target_link_libraries(fileExplorer PRIVATE ncursesw)

target_include_directories(fileExplorer PRIVATE C:/msys64/mingw64/include C:/msys64/mingw64/include/ncurses)

if(MSVC)
  target_compile_options(fileExplorer PRIVATE /W4 /WX)
else()
  target_compile_options(fileExplorer PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
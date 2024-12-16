cmake_minimum_required(VERSION 3.5)
project(Game C CXX)

# Set the directory where the bin will be built.
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)


if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 --shell-file ${CMAKE_SOURCE_DIR}/src/shell.html ")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

    set(PLATFORM Web)
    set(OPENGL_VERSION "ES 2.0")
else()
    set(PLATFORM Desktop)
    set(OPENGL_VERSION 3.3)

endif ()

add_subdirectory(src game)

add_subdirectory(raylib-master raylib)



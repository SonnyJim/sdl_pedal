cmake_minimum_required(VERSION 3.10)
project(sdl_pedal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL2 and SDL2_ttf
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
find_package(ALSA REQUIRED)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${ALSA_INCLUDE_DIRS}
    src
)

add_executable(sdl_pedal
    src/main.cpp
    src/midi.cpp
    src/button.cpp
    src/slider.cpp
    src/nav.cpp
)

target_link_libraries(sdl_pedal
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${ALSA_LIBRARIES}
)


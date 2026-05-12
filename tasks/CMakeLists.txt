cmake_minimum_required (VERSION 3.8)

add_executable(tasks "main.cpp" "modulator.h" "modulator.cpp" "awgn.h" "awgn.cpp" "demodulator.h" "demodulator.cpp" "utils.h" "utils.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET tasks PROPERTY CXX_STANDARD 20)
endif()


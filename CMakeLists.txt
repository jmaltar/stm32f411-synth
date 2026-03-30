cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME stm32f411-synth)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM, C and C++ languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

set(SYNTH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/synth)
set(TINYUSB_DIR /home/jurica/.pico-sdk/sdk/2.2.0/lib/tinyusb/src)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Src/main.cpp
    Core/Src/usb_descriptors.c
    # Synthesis engine (shared)
    ${SYNTH_DIR}/adsr.cpp
    ${SYNTH_DIR}/oscillator.cpp
    ${SYNTH_DIR}/voice.cpp
    ${SYNTH_DIR}/synth.cpp
    # TinyUSB core
    ${TINYUSB_DIR}/tusb.c
    ${TINYUSB_DIR}/common/tusb_fifo.c
    ${TINYUSB_DIR}/device/usbd.c
    ${TINYUSB_DIR}/device/usbd_control.c
    # MIDI class
    ${TINYUSB_DIR}/class/midi/midi_device.c
    # DWC2 backend for STM32F4 OTG FS
    ${TINYUSB_DIR}/portable/synopsys/dwc2/dcd_dwc2.c
    ${TINYUSB_DIR}/portable/synopsys/dwc2/dwc2_common.c
)

set(SYNTH_SOURCES
    ${SYNTH_DIR}/adsr.cpp
    ${SYNTH_DIR}/oscillator.cpp
    ${SYNTH_DIR}/voice.cpp
    ${SYNTH_DIR}/synth.cpp
)

# Keep HAL/USB conservative, but allow faster floating-point optimizations in
# the synth engine for better DSP throughput.
set_source_files_properties(${SYNTH_SOURCES} PROPERTIES
    COMPILE_OPTIONS "$<$<CONFIG:Release>:-ffast-math;-fno-math-errno;-ffp-contract=fast>"
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Inc
    ${SYNTH_DIR}
    ${TINYUSB_DIR}
    ${TINYUSB_DIR}/portable/synopsys/dwc2
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    CFG_TUSB_MCU=OPT_MCU_STM32F4
    SYNTH_N_VOICES=6
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

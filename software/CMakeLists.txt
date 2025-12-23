cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# == DO NOT EDIT (VS Code extension integration) ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ================================================

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico-sdk/pico_sdk_init.cmake)

project(flute C CXX ASM)

# =====================================================
# Build configuration options
# =====================================================

option(DEBUG_MODE "Enable debug mode" OFF)

set(PROJECT_TYPE "INT" CACHE STRING "Project numeric type: INT or FLOAT")
set_property(CACHE PROJECT_TYPE PROPERTY STRINGS INT FLOAT)

# Initialise SDK
pico_sdk_init()

# Compiler options
add_compile_options(-Wall -Wextra -O2)

# =====================================================
# Add include and source directories
# =====================================================

# Add include/ to header search paths
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Collect all .c files from source/
file(GLOB FLUTE_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/source/*.c
)

# (optional) also add main flute.c if it is not in /source
# file(GLOB FLUTE_SOURCES ${FLUTE_SOURCES} ${CMAKE_CURRENT_LIST_DIR}/flute.c)

# Create executable
add_executable(flute
    ${FLUTE_SOURCES}
)

# =====================================================
# Compile definitions
# =====================================================

# Common numeric identifiers
target_compile_definitions(flute PRIVATE
    PROJECT_INT=1
    PROJECT_FLOAT=2
)

# Debug mode (0 or 1)
target_compile_definitions(flute PRIVATE
    DEBUG_MODE=$<BOOL:${DEBUG_MODE}>
)

message("${DEBUG_MODE}")

# Project type selection
if(PROJECT_TYPE STREQUAL "INT")
    target_compile_definitions(flute PRIVATE PROJECT_TYPE=PROJECT_INT)
	message("int")
elseif(PROJECT_TYPE STREQUAL "FLOAT")
    target_compile_definitions(flute PRIVATE PROJECT_TYPE=PROJECT_FLOAT)
	message("float")
else()
    message(FATAL_ERROR "PROJECT_TYPE must be INT or FLOAT")
endif()

# Link libraries
target_link_libraries(flute
    pico_stdlib
    hardware_adc
    hardware_pwm
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(flute pico_cyw43_arch_none)
endif()

pico_enable_stdio_usb(flute 1)
pico_enable_stdio_uart(flute 0)

# Generate map/bin/hex files
pico_add_extra_outputs(flute)


# ========================================================
# CMake configuration for Programa_21 - Finite Volume Mesh
# Optimized for performance and parallel execution
# ========================================================

cmake_minimum_required(VERSION 3.15)

# Project configuration
project(Programa_21 
    VERSION 1.0 
    DESCRIPTION "Program for 1D mesh node coordinates calculation using geometric progression"
    LANGUAGES CXX
)

# ========================
# Compiler Configuration
# ========================

# C++ standard setup
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ========================
# Performance Flags
# ========================

# Base optimization flags for all compilers
add_compile_options(
    "$<$<CONFIG:RELEASE>:-O3>"
    "$<$<CONFIG:RELEASE>:-DNDEBUG>"
)

# Compiler-specific optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC/Clang specific optimizations
    add_compile_options(
        -march=native       # Optimize for current CPU architecture
        -flto               # Link-time optimization
        -ffast-math         # Faster math operations (safe for this application)
        -funroll-loops      # Loop unrolling
        -fopenmp           # OpenMP support for parallelization
    )
    add_link_options(-fopenmp)
    
elseif(MSVC)
    # MSVC specific optimizations
    add_compile_options(
        /O2        # Optimize for speed
        /GL        # Whole program optimization
        /arch:AVX2 # Enable AVX2 instructions
        /openmp    # OpenMP support
    )
endif()

# ========================
# Source Files Setup
# ========================

set(EXECUTABLE_NAME "Programa_21")

# Source directories
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Find all source files
file(GLOB_RECURSE SOURCE_FILES 
    "${SRC_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Programa_21.cpp"
)

# ========================
# Executable Configuration
# ========================

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# Include directories
target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link TBB for parallel execution
find_package(TBB REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE TBB::tbb)

# ========================
# Output Configuration
# ========================

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    OUTPUT_NAME "${EXECUTABLE_NAME}"
)

# ========================
# Build Messages
# ========================

message(STATUS "Building with maximum optimizations")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Source files: ${SOURCE_FILES}")

# ========================
# Post-Build Configuration
# ========================

# Add run target
add_custom_target(run_${EXECUTABLE_NAME}
    COMMAND ${EXECUTABLE_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
    DEPENDS ${EXECUTABLE_NAME}
    COMMENT "Running ${EXECUTABLE_NAME} with optimized settings"
)
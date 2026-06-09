include_guard()

option(BUILD_TESTS "Build test targets" OFF)
option(BUILD_EXAMPLES "Build example targets" OFF)
option(BUILD_BOOK "Build book/chapter targets" ON)

option(FVM_ENABLE_TBB "Enable Intel TBB support" ON)
option(FVM_ENABLE_OPENMP "Enable OpenMP support" OFF)
option(FVM_ENABLE_IPO "Enable IPO/LTO for release-like builds" ON)
option(FVM_ENABLE_NATIVE_OPTIMIZATION "Enable -march=native on GNU-like compilers" ON)
option(FVM_ENABLE_NDEBUG "Define NDEBUG explicitly for release-like builds" OFF)
option(FVM_BUILD_SHARED "Build FVMaker as a shared library" OFF)

set(FVM_ALLOWED_BUILD_TYPES
    Debug
    Release
    RelWithDebInfo
    MinSizeRel
)

if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    endif()

    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        ${FVM_ALLOWED_BUILD_TYPES}
    )

    if(NOT CMAKE_BUILD_TYPE IN_LIST FVM_ALLOWED_BUILD_TYPES)
        message(FATAL_ERROR
            "Invalid CMAKE_BUILD_TYPE='${CMAKE_BUILD_TYPE}'. "
            "Allowed values are: ${FVM_ALLOWED_BUILD_TYPES}"
        )
    endif()
endif()

if(BUILD_TESTS AND BUILD_EXAMPLES)
    message(FATAL_ERROR
        "BUILD_TESTS and BUILD_EXAMPLES cannot be enabled simultaneously."
    )
endif()

if(BUILD_TESTS AND BUILD_BOOK)
    message(FATAL_ERROR
        "BUILD_TESTS and BUILD_BOOK cannot be enabled simultaneously."
    )
endif()

if(FVM_BUILD_SHARED)
    set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
else()
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
endif()

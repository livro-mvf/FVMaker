cmake_minimum_required(VERSION 3.20)

project(FVMaker
    VERSION 1.0.0
    LANGUAGES CXX
)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR
        "In-source builds are not allowed. "
        "Create a separate build directory."
    )
endif()

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

include(ConfigOptions)
include(ConfigDependencies)
include(ConfigCompiler)
include(ConfigTargets)
include(ConfigExamples)
include(ConfigLivro)
include(ConfigTests)
include(ConfigPerformance)
include(ConfigDocs)

set(FVM_RUN_ALL_TARGETS)

if(TARGET run_all_examples)
    list(APPEND FVM_RUN_ALL_TARGETS run_all_examples)
endif()

if(TARGET run_all_capitulos)
    list(APPEND FVM_RUN_ALL_TARGETS run_all_capitulos)
endif()

if(TARGET run_all_tests)
    list(APPEND FVM_RUN_ALL_TARGETS run_all_tests)
endif()

if(TARGET run_all_performance)
    list(APPEND FVM_RUN_ALL_TARGETS run_all_performance)
endif()

if(FVM_RUN_ALL_TARGETS)
    add_custom_target(run_all
        DEPENDS ${FVM_RUN_ALL_TARGETS}
    )
endif()

message(STATUS "FVMaker configuration complete")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")

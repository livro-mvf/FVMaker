include_guard()

if(NOT BUILD_PERFORMANCE)
    return()
endif()

set(FVM_PERFORMANCE_DIR_CANDIDATES
    "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks"
    "${CMAKE_CURRENT_SOURCE_DIR}/profiling"
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/performance"
)

set(FVM_PERFORMANCE_SOURCES)

foreach(FVM_PERFORMANCE_DIR IN LISTS FVM_PERFORMANCE_DIR_CANDIDATES)
    if(EXISTS "${FVM_PERFORMANCE_DIR}")
        file(GLOB_RECURSE FVM_PERFORMANCE_DIR_SOURCES CONFIGURE_DEPENDS
            "${FVM_PERFORMANCE_DIR}/bm_*.cpp"
            "${FVM_PERFORMANCE_DIR}/bm_*.cc"
            "${FVM_PERFORMANCE_DIR}/prof_*.cpp"
            "${FVM_PERFORMANCE_DIR}/prof_*.cc"
        )

        list(APPEND FVM_PERFORMANCE_SOURCES
            ${FVM_PERFORMANCE_DIR_SOURCES}
        )
    endif()
endforeach()

if(NOT FVM_PERFORMANCE_SOURCES)
    message(WARNING
        "BUILD_PERFORMANCE is ON, but no performance sources were found. "
        "Expected files named bm_*.cpp, bm_*.cc, prof_*.cpp or prof_*.cc "
        "under benchmarks/, profiling/ or tools/performance/."
    )
    return()
endif()

find_program(FVM_PERF_EXECUTABLE perf)
find_program(FVM_VALGRIND_EXECUTABLE valgrind)
find_program(FVM_GPROF_EXECUTABLE gprof)

set(FVM_PERFORMANCE_RUN_TARGETS)

foreach(FVM_PERFORMANCE_SOURCE IN LISTS FVM_PERFORMANCE_SOURCES)
    file(RELATIVE_PATH
        FVM_PERFORMANCE_RELATIVE_PATH
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${FVM_PERFORMANCE_SOURCE}"
    )

    get_filename_component(FVM_PERFORMANCE_NAME
        "${FVM_PERFORMANCE_RELATIVE_PATH}"
        NAME_WE
    )

    get_filename_component(FVM_PERFORMANCE_RELATIVE_DIR
        "${FVM_PERFORMANCE_RELATIVE_PATH}"
        DIRECTORY
    )

    string(REGEX REPLACE "^(bm_|prof_)" ""
        FVM_PERFORMANCE_NAME_TOKEN
        "${FVM_PERFORMANCE_NAME}"
    )

    string(REPLACE "/" "_"
        FVM_PERFORMANCE_DIR_TOKEN
        "${FVM_PERFORMANCE_RELATIVE_DIR}"
    )

    string(REPLACE "\\" "_"
        FVM_PERFORMANCE_DIR_TOKEN
        "${FVM_PERFORMANCE_DIR_TOKEN}"
    )

    string(REGEX REPLACE "[^A-Za-z0-9_]" "_"
        FVM_PERFORMANCE_DIR_TOKEN
        "${FVM_PERFORMANCE_DIR_TOKEN}"
    )

    string(REGEX REPLACE "[^A-Za-z0-9_]" "_"
        FVM_PERFORMANCE_NAME_TOKEN
        "${FVM_PERFORMANCE_NAME_TOKEN}"
    )

    set(FVM_PERFORMANCE_TARGET
        "perf_${FVM_PERFORMANCE_DIR_TOKEN}_${FVM_PERFORMANCE_NAME_TOKEN}"
    )

    if(TARGET "${FVM_PERFORMANCE_TARGET}")
        message(FATAL_ERROR
            "Duplicate performance target detected: "
            "${FVM_PERFORMANCE_TARGET}"
        )
    endif()

    add_executable("${FVM_PERFORMANCE_TARGET}"
        "${FVM_PERFORMANCE_SOURCE}"
    )

    target_link_libraries("${FVM_PERFORMANCE_TARGET}"
        PRIVATE
            FVMaker::FVMaker
    )

    target_compile_features("${FVM_PERFORMANCE_TARGET}"
        PRIVATE
            cxx_std_20
    )

    set_target_properties("${FVM_PERFORMANCE_TARGET}"
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            RUNTIME_OUTPUT_DIRECTORY
                "${CMAKE_CURRENT_BINARY_DIR}/bin/performance"
    )

    set_target_optimizations("${FVM_PERFORMANCE_TARGET}")

    if(FVM_ENABLE_GPROF)
        fvm_is_gnu_like(FVM_PERFORMANCE_IS_GNU_LIKE)

        if(FVM_PERFORMANCE_IS_GNU_LIKE)
            target_compile_options("${FVM_PERFORMANCE_TARGET}"
                PRIVATE
                    -pg
            )

            target_link_options("${FVM_PERFORMANCE_TARGET}"
                PRIVATE
                    -pg
            )
        else()
            message(WARNING
                "FVM_ENABLE_GPROF is ON, but gprof instrumentation is "
                "currently configured only for GNU-like compilers."
            )
        endif()
    endif()

    add_custom_target("run_${FVM_PERFORMANCE_TARGET}"
        COMMAND "$<TARGET_FILE:${FVM_PERFORMANCE_TARGET}>"
        DEPENDS "${FVM_PERFORMANCE_TARGET}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )

    list(APPEND FVM_PERFORMANCE_RUN_TARGETS
        "run_${FVM_PERFORMANCE_TARGET}"
    )

    if(FVM_PERF_EXECUTABLE)
        add_custom_target("perf_stat_${FVM_PERFORMANCE_TARGET}"
            COMMAND "${FVM_PERF_EXECUTABLE}" stat
                    "$<TARGET_FILE:${FVM_PERFORMANCE_TARGET}>"
            DEPENDS "${FVM_PERFORMANCE_TARGET}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            VERBATIM
        )
    endif()

    if(FVM_VALGRIND_EXECUTABLE)
        add_custom_target("callgrind_${FVM_PERFORMANCE_TARGET}"
            COMMAND "${FVM_VALGRIND_EXECUTABLE}"
                    --tool=callgrind
                    "--callgrind-out-file=callgrind.${FVM_PERFORMANCE_TARGET}.out"
                    "$<TARGET_FILE:${FVM_PERFORMANCE_TARGET}>"
            DEPENDS "${FVM_PERFORMANCE_TARGET}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            VERBATIM
        )
    endif()

    if(FVM_ENABLE_GPROF AND FVM_GPROF_EXECUTABLE)
        add_custom_target("gprof_${FVM_PERFORMANCE_TARGET}"
            COMMAND "$<TARGET_FILE:${FVM_PERFORMANCE_TARGET}>"
            COMMAND "${FVM_GPROF_EXECUTABLE}"
                    "$<TARGET_FILE:${FVM_PERFORMANCE_TARGET}>"
                    gmon.out
            DEPENDS "${FVM_PERFORMANCE_TARGET}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            VERBATIM
        )
    endif()
endforeach()

if(FVM_PERFORMANCE_RUN_TARGETS)
    add_custom_target(run_all_performance
        DEPENDS ${FVM_PERFORMANCE_RUN_TARGETS}
    )
endif()

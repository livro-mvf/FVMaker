include_guard()

if(NOT BUILD_TESTS)
    return()
endif()

include(CTest)

if(NOT BUILD_TESTING)
    return()
endif()

enable_testing()

option(FVM_TESTS_FETCH_GOOGLETEST
    "Download GoogleTest with FetchContent if it is not found"
    ON
)

find_package(GTest CONFIG QUIET)

if(NOT GTest_FOUND)
    if(NOT FVM_TESTS_FETCH_GOOGLETEST)
        message(FATAL_ERROR
            "GoogleTest was not found and FVM_TESTS_FETCH_GOOGLETEST is OFF."
        )
    endif()

    include(FetchContent)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)

set(FVM_TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/tests")

if(NOT EXISTS "${FVM_TESTS_DIR}")
    set(FVM_TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Tests")
endif()

if(NOT EXISTS "${FVM_TESTS_DIR}")
    message(WARNING "Tests directory was not found.")
    return()
endif()

file(GLOB_RECURSE FVM_TEST_SOURCES CONFIGURE_DEPENDS
    "${FVM_TESTS_DIR}/tst_*.cpp"
    "${FVM_TESTS_DIR}/tst_*.cxx"
    "${FVM_TESTS_DIR}/tst_*.cc"
)

if(NOT FVM_TEST_SOURCES)
    message(WARNING
        "No test entry-point files matching tst_*.cpp, tst_*.cxx or "
        "tst_*.cc were found in '${FVM_TESTS_DIR}'."
    )
    return()
endif()

set(FVM_TEST_RUN_TARGETS)
set(FVM_MEMCHECK_RUN_TARGETS)

if(BUILD_MEMCHECK)
    find_program(FVM_VALGRIND_EXECUTABLE valgrind)

    if(NOT FVM_VALGRIND_EXECUTABLE)
        message(WARNING
            "BUILD_MEMCHECK is ON, but valgrind was not found. "
            "Memory-check targets will not be created."
        )
    endif()
endif()

foreach(FVM_TEST_SOURCE IN LISTS FVM_TEST_SOURCES)
    file(RELATIVE_PATH
        FVM_TEST_RELATIVE_PATH
        "${FVM_TESTS_DIR}"
        "${FVM_TEST_SOURCE}"
    )

    get_filename_component(FVM_TEST_NAME
        "${FVM_TEST_RELATIVE_PATH}"
        NAME_WE
    )

    get_filename_component(FVM_TEST_RELATIVE_DIR
        "${FVM_TEST_RELATIVE_PATH}"
        DIRECTORY
    )

    set(FVM_TEST_NAME_TOKEN "${FVM_TEST_NAME}")

    if(FVM_TEST_NAME_TOKEN MATCHES "^tst_(.+)$")
        set(FVM_TEST_NAME_TOKEN "${CMAKE_MATCH_1}")
    endif()

    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" FVM_TEST_NAME_TOKEN
        "${FVM_TEST_NAME_TOKEN}"
    )

    if(FVM_TEST_RELATIVE_DIR)
        string(REPLACE "/" "_" FVM_TEST_DIR_TOKEN
            "${FVM_TEST_RELATIVE_DIR}"
        )
        string(REPLACE "\\" "_" FVM_TEST_DIR_TOKEN
            "${FVM_TEST_DIR_TOKEN}"
        )
        string(REGEX REPLACE "[^A-Za-z0-9_]" "_" FVM_TEST_DIR_TOKEN
            "${FVM_TEST_DIR_TOKEN}"
        )

        set(FVM_TEST_TARGET
            "tst_${FVM_TEST_DIR_TOKEN}_${FVM_TEST_NAME_TOKEN}"
        )
    else()
        set(FVM_TEST_TARGET
            "tst_${FVM_TEST_NAME_TOKEN}"
        )
    endif()

    if(TARGET "${FVM_TEST_TARGET}")
        message(FATAL_ERROR
            "Duplicate test target detected: ${FVM_TEST_TARGET}. "
            "Test executable names must be unique relative to the tests tree."
        )
    endif()

    add_executable("${FVM_TEST_TARGET}"
        "${FVM_TEST_SOURCE}"
    )

    target_link_libraries("${FVM_TEST_TARGET}"
        PRIVATE
            FVMaker::FVMaker
            GTest::gtest_main
    )

    target_compile_features("${FVM_TEST_TARGET}"
        PRIVATE
            cxx_std_20
    )

    set_target_properties("${FVM_TEST_TARGET}"
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/tests"
    )

    set_target_optimizations("${FVM_TEST_TARGET}")

    gtest_discover_tests("${FVM_TEST_TARGET}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    )

    add_custom_target("run_${FVM_TEST_TARGET}"
        COMMAND "$<TARGET_FILE:${FVM_TEST_TARGET}>"
        DEPENDS "${FVM_TEST_TARGET}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )

    list(APPEND FVM_TEST_RUN_TARGETS
        "run_${FVM_TEST_TARGET}"
    )

    if(BUILD_MEMCHECK AND FVM_VALGRIND_EXECUTABLE)
        add_custom_target("memcheck_${FVM_TEST_TARGET}"
            COMMAND "${FVM_VALGRIND_EXECUTABLE}"
                    --tool=memcheck
                    --leak-check=full
                    --show-leak-kinds=definite,possible
                    --errors-for-leak-kinds=definite,possible
                    --track-origins=yes
                    --error-exitcode=99
                    "$<TARGET_FILE:${FVM_TEST_TARGET}>"
            DEPENDS "${FVM_TEST_TARGET}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            VERBATIM
        )

        list(APPEND FVM_MEMCHECK_RUN_TARGETS
            "memcheck_${FVM_TEST_TARGET}"
        )
    endif()
endforeach()

if(FVM_TEST_RUN_TARGETS)
    add_custom_target(run_all_tests
        DEPENDS ${FVM_TEST_RUN_TARGETS}
    )
endif()

if(FVM_MEMCHECK_RUN_TARGETS)
    add_custom_target(memcheck_all_tests
        DEPENDS ${FVM_MEMCHECK_RUN_TARGETS}
    )
endif()

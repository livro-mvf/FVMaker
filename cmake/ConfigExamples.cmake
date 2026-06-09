include_guard()

if(NOT BUILD_EXAMPLES)
    return()
endif()

set(FVM_EXAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples")

if(NOT EXISTS "${FVM_EXAMPLES_DIR}")
    set(FVM_EXAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Examples")
endif()

if(NOT EXISTS "${FVM_EXAMPLES_DIR}")
    message(WARNING "Examples directory was not found.")
    return()
endif()

file(GLOB_RECURSE FVM_EXAMPLE_SOURCES CONFIGURE_DEPENDS
    "${FVM_EXAMPLES_DIR}/ex_*.cpp"
    "${FVM_EXAMPLES_DIR}/ex_*.cc"
)

if(NOT FVM_EXAMPLE_SOURCES)
    message(WARNING
        "No example entry-point files matching ex_*.cpp or ex_*.cc "
        "were found in '${FVM_EXAMPLES_DIR}'."
    )
    return()
endif()

set(FVM_EXAMPLE_RUN_TARGETS)

foreach(FVM_EXAMPLE_SOURCE IN LISTS FVM_EXAMPLE_SOURCES)
    get_filename_component(FVM_EXAMPLE_NAME
        "${FVM_EXAMPLE_SOURCE}"
        NAME_WE
    )

    get_filename_component(FVM_EXAMPLE_PARENT_DIR
        "${FVM_EXAMPLE_SOURCE}"
        DIRECTORY
    )

    get_filename_component(FVM_EXAMPLE_PARENT_TOKEN
        "${FVM_EXAMPLE_PARENT_DIR}"
        NAME
    )

    string(REGEX REPLACE "^ex_" "" FVM_EXAMPLE_NAME_TOKEN
        "${FVM_EXAMPLE_NAME}"
    )

    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" FVM_EXAMPLE_PARENT_TOKEN
        "${FVM_EXAMPLE_PARENT_TOKEN}"
    )

    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" FVM_EXAMPLE_NAME_TOKEN
        "${FVM_EXAMPLE_NAME_TOKEN}"
    )

    if(FVM_EXAMPLE_PARENT_TOKEN STREQUAL "examples" OR
       FVM_EXAMPLE_PARENT_TOKEN STREQUAL "Examples")
        set(FVM_EXAMPLE_TARGET
            "ex_${FVM_EXAMPLE_NAME_TOKEN}"
        )
    else()
        set(FVM_EXAMPLE_TARGET
            "ex_${FVM_EXAMPLE_PARENT_TOKEN}_${FVM_EXAMPLE_NAME_TOKEN}"
        )
    endif()

    if(TARGET "${FVM_EXAMPLE_TARGET}")
        message(FATAL_ERROR
            "Duplicate example target detected: ${FVM_EXAMPLE_TARGET}. "
            "Rename one example file or move it to a distinct parent directory."
        )
    endif()

    add_executable("${FVM_EXAMPLE_TARGET}"
        "${FVM_EXAMPLE_SOURCE}"
    )

    target_link_libraries("${FVM_EXAMPLE_TARGET}"
        PRIVATE
            FVMaker::FVMaker
    )

    target_compile_features("${FVM_EXAMPLE_TARGET}"
        PRIVATE
            cxx_std_20
    )

    set_target_properties("${FVM_EXAMPLE_TARGET}"
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/examples"
    )

    add_custom_target("run_${FVM_EXAMPLE_TARGET}"
        COMMAND "$<TARGET_FILE:${FVM_EXAMPLE_TARGET}>"
        DEPENDS "${FVM_EXAMPLE_TARGET}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )

    list(APPEND FVM_EXAMPLE_RUN_TARGETS
        "run_${FVM_EXAMPLE_TARGET}"
    )
endforeach()

if(FVM_EXAMPLE_RUN_TARGETS)
    add_custom_target(run_all_examples
        DEPENDS ${FVM_EXAMPLE_RUN_TARGETS}
    )
endif()
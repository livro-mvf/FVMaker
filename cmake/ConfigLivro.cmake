include_guard()

if(NOT BUILD_BOOK)
    return()
endif()

set(FVM_BOOK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/capitulos")

if(NOT EXISTS "${FVM_BOOK_DIR}")
    set(FVM_BOOK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Capitulos")
endif()

if(NOT EXISTS "${FVM_BOOK_DIR}")
    message(WARNING "Book chapters directory was not found.")
    return()
endif()

file(GLOB_RECURSE FVM_BOOK_SOURCES CONFIGURE_DEPENDS
    "${FVM_BOOK_DIR}/*.cpp"
    "${FVM_BOOK_DIR}/*.cc"
)

if(NOT FVM_BOOK_SOURCES)
    message(WARNING
        "No book/chapter entry-point files matching ex_*.cpp or ex_*.cc "
        "were found in '${FVM_BOOK_DIR}'."
    )
    return()
endif()

set(FVM_BOOK_RUN_TARGETS)

foreach(FVM_BOOK_SOURCE IN LISTS FVM_BOOK_SOURCES)
    get_filename_component(FVM_BOOK_NAME
        "${FVM_BOOK_SOURCE}"
        NAME_WE
    )

    string(REGEX REPLACE "^ex_" "" FVM_BOOK_NAME_TOKEN
        "${FVM_BOOK_NAME}"
    )

    string(REGEX REPLACE "[^A-Za-z0-9_]" "_" FVM_BOOK_NAME_TOKEN
        "${FVM_BOOK_NAME_TOKEN}"
    )

    set(FVM_BOOK_TARGET
        "cap_${FVM_BOOK_NAME_TOKEN}"
    )

    if(TARGET "${FVM_BOOK_TARGET}")
        message(FATAL_ERROR
            "Duplicate book/chapter target detected: ${FVM_BOOK_TARGET}. "
            "Book/chapter executable names must be unique."
        )
    endif()

    add_executable("${FVM_BOOK_TARGET}"
        "${FVM_BOOK_SOURCE}"
    )

    target_link_libraries("${FVM_BOOK_TARGET}"
        PRIVATE
            FVMaker::FVMaker
    )

    target_compile_features("${FVM_BOOK_TARGET}"
        PRIVATE
            cxx_std_20
    )

    set_target_properties("${FVM_BOOK_TARGET}"
        PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/capitulos"
    )

    add_custom_target("run_${FVM_BOOK_TARGET}"
        COMMAND "$<TARGET_FILE:${FVM_BOOK_TARGET}>"
        DEPENDS "${FVM_BOOK_TARGET}"
        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
        VERBATIM
    )

    list(APPEND FVM_BOOK_RUN_TARGETS
        "run_${FVM_BOOK_TARGET}"
    )
endforeach()

if(FVM_BOOK_RUN_TARGETS)
    add_custom_target(run_all_capitulos
        DEPENDS ${FVM_BOOK_RUN_TARGETS}
    )
endif()

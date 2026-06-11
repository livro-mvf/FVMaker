include_guard()

if(NOT BUILD_DOCUMENTATION)
    return()
endif()

set(FVM_DOCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/docs")
set(FVM_DOCS_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/docs")

if(NOT EXISTS "${FVM_DOCS_DIR}")
    message(WARNING "Documentation directory was not found: ${FVM_DOCS_DIR}")
    return()
endif()

find_program(FVM_SPHINX_BUILD_EXECUTABLE
    NAMES sphinx-build sphinx-build.exe
)

if(NOT FVM_SPHINX_BUILD_EXECUTABLE)
    message(FATAL_ERROR
        "BUILD_DOCUMENTATION is ON, but sphinx-build was not found. "
        "Install Sphinx or disable BUILD_DOCUMENTATION."
    )
endif()

add_custom_target(docs
    COMMAND "${FVM_SPHINX_BUILD_EXECUTABLE}"
            -b html
            "${FVM_DOCS_DIR}"
            "${FVM_DOCS_BUILD_DIR}/html"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Building FVMaker Sphinx documentation"
    VERBATIM
)

add_custom_target(clean-docs
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${FVM_DOCS_BUILD_DIR}"
    COMMENT "Removing generated FVMaker documentation"
    VERBATIM
)

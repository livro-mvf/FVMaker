include_guard()

set(FVM_LIBRARY_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/FVMakerLib/src"
)

set(FVM_LIBRARY_INCLUDE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/FVMakerLib/include"
)

if(NOT EXISTS "${FVM_LIBRARY_INCLUDE_DIR}")
    message(FATAL_ERROR
        "FVMaker include directory was not found: "
        "${FVM_LIBRARY_INCLUDE_DIR}"
    )
endif()

set(FVM_LIBRARY_SOURCES)

if(EXISTS "${FVM_LIBRARY_SOURCE_DIR}")
    file(GLOB_RECURSE FVM_LIBRARY_SOURCES CONFIGURE_DEPENDS
        "${FVM_LIBRARY_SOURCE_DIR}/*.cpp"
        "${FVM_LIBRARY_SOURCE_DIR}/*.cxx"
        "${FVM_LIBRARY_SOURCE_DIR}/*.cc"
    )
endif()

if(FVM_LIBRARY_SOURCES)
    add_library(FVMaker ${FVM_LIBRARY_SOURCES})

    target_include_directories(FVMaker
        PUBLIC
            $<BUILD_INTERFACE:${FVM_LIBRARY_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:include>
    )

    target_compile_features(FVMaker
        PUBLIC
            cxx_std_20
    )

    target_compile_definitions(FVMaker
        PRIVATE
            $<$<AND:$<BOOL:${FVM_ENABLE_NDEBUG}>,$<CONFIG:Release>>:NDEBUG>
            $<$<AND:$<BOOL:${FVM_ENABLE_NDEBUG}>,$<CONFIG:RelWithDebInfo>>:NDEBUG>
            $<$<AND:$<BOOL:${FVM_ENABLE_NDEBUG}>,$<CONFIG:MinSizeRel>>:NDEBUG>
    )

    if(FVM_PRIVATE_DEPENDENCIES)
        target_link_libraries(FVMaker
            PRIVATE
                ${FVM_PRIVATE_DEPENDENCIES}
        )
    endif()

    set_target_properties(FVMaker
        PROPERTIES
            OUTPUT_NAME FVMaker
            ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            POSITION_INDEPENDENT_CODE ON
    )

    set_target_optimizations(FVMaker)
else()
    add_library(FVMaker INTERFACE)

    target_include_directories(FVMaker
        INTERFACE
            $<BUILD_INTERFACE:${FVM_LIBRARY_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:include>
    )

    target_compile_features(FVMaker
        INTERFACE
            cxx_std_20
    )

    message(WARNING
        "No FVMaker source files were found in "
        "'${FVM_LIBRARY_SOURCE_DIR}'. "
        "FVMaker will be configured as an INTERFACE header-only target."
    )
endif()

add_library(FVMaker::FVMaker ALIAS FVMaker)

add_custom_target(info
    COMMAND ${CMAKE_COMMAND} -E echo "Project: ${PROJECT_NAME}"
    COMMAND ${CMAKE_COMMAND} -E echo "Version: ${PROJECT_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E echo "Build type: ${CMAKE_BUILD_TYPE}"
    COMMAND ${CMAKE_COMMAND} -E echo "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
    COMMAND ${CMAKE_COMMAND} -E echo "C++ standard: C++20"
    COMMAND ${CMAKE_COMMAND} -E echo "Source directory: ${FVM_LIBRARY_SOURCE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Include directory: ${FVM_LIBRARY_INCLUDE_DIR}"
    VERBATIM
)

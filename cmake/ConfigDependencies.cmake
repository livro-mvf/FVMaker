include_guard()

set(FVM_PUBLIC_DEPENDENCIES)
set(FVM_PRIVATE_DEPENDENCIES)

option(FVM_ENABLE_FVGRIDMAKER
    "Enable integration with FVGridMaker structured meshes"
    ON
)

set(FVM_FVGRIDMAKER_ROOT
    "${CMAKE_CURRENT_SOURCE_DIR}/../FVGridMaker"
    CACHE PATH
    "Path to a local FVGridMaker source tree used when find_package fails"
)

if(FVM_ENABLE_FVGRIDMAKER)
    find_package(FVGridMaker CONFIG QUIET)

    if(NOT TARGET FVGridMaker::FVGridMaker AND
       EXISTS "${FVM_FVGRIDMAKER_ROOT}/CMakeLists.txt")
        set(FVM_SAVED_BUILD_TESTS "${BUILD_TESTS}")
        set(FVM_SAVED_BUILD_EXAMPLES "${BUILD_EXAMPLES}")
        set(FVM_SAVED_BUILD_BOOK "${BUILD_BOOK}")
        set(FVM_SAVED_BUILD_DOCUMENTATION "${BUILD_DOCUMENTATION}")
        set(FVM_SAVED_CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}")

        set(BUILD_TESTS OFF)
        set(BUILD_EXAMPLES OFF)
        set(BUILD_BOOK OFF)
        set(BUILD_DOCUMENTATION OFF)
        set(CMAKE_MODULE_PATH)

        add_subdirectory(
            "${FVM_FVGRIDMAKER_ROOT}"
            "${CMAKE_CURRENT_BINARY_DIR}/_deps/FVGridMaker"
            EXCLUDE_FROM_ALL
        )

        set(BUILD_TESTS "${FVM_SAVED_BUILD_TESTS}")
        set(BUILD_EXAMPLES "${FVM_SAVED_BUILD_EXAMPLES}")
        set(BUILD_BOOK "${FVM_SAVED_BUILD_BOOK}")
        set(BUILD_DOCUMENTATION "${FVM_SAVED_BUILD_DOCUMENTATION}")
        set(CMAKE_MODULE_PATH "${FVM_SAVED_CMAKE_MODULE_PATH}")
    endif()

    if(TARGET FVGridMaker::FVGridMaker)
        list(APPEND FVM_PUBLIC_DEPENDENCIES FVGridMaker::FVGridMaker)
    else()
        message(FATAL_ERROR
            "FVGridMaker was not found. Install FVGridMaker, set "
            "FVM_FVGRIDMAKER_ROOT, or disable FVM_ENABLE_FVGRIDMAKER."
        )
    endif()
endif()

if(FVM_ENABLE_TBB)
    find_package(TBB REQUIRED COMPONENTS tbb)
    list(APPEND FVM_PRIVATE_DEPENDENCIES TBB::tbb)
endif()

if(FVM_ENABLE_OPENMP)
    find_package(OpenMP REQUIRED COMPONENTS CXX)
    list(APPEND FVM_PRIVATE_DEPENDENCIES OpenMP::OpenMP_CXX)
endif()

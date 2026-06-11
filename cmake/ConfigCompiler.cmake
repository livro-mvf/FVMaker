include_guard()

include(CheckIPOSupported)

function(fvm_is_msvc_like out_var)
    if(MSVC)
        set("${out_var}" TRUE PARENT_SCOPE)
        return()
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND
       CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        set("${out_var}" TRUE PARENT_SCOPE)
        return()
    endif()

    if(DEFINED CMAKE_CXX_COMPILER_FRONTEND_VARIANT AND
       CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        set("${out_var}" TRUE PARENT_SCOPE)
        return()
    endif()

    set("${out_var}" FALSE PARENT_SCOPE)
endfunction()

function(fvm_is_gnu_like out_var)
    fvm_is_msvc_like(is_msvc_like)

    if(is_msvc_like)
        set("${out_var}" FALSE PARENT_SCOPE)
        return()
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
       CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR
       CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        set("${out_var}" TRUE PARENT_SCOPE)
        return()
    endif()

    set("${out_var}" FALSE PARENT_SCOPE)
endfunction()

function(fvm_enable_ipo target)
    if(NOT FVM_ENABLE_IPO)
        return()
    endif()

    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)

    if(ipo_supported)
        set_property(TARGET "${target}" PROPERTY
            INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE
        )
        set_property(TARGET "${target}" PROPERTY
            INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE
        )
        set_property(TARGET "${target}" PROPERTY
            INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL TRUE
        )
    else()
        message(WARNING
            "IPO/LTO requested for target ${target}, but it is not supported: "
            "${ipo_error}"
        )
    endif()
endfunction()

function(set_target_optimizations target)
    fvm_is_msvc_like(is_msvc_like)
    fvm_is_gnu_like(is_gnu_like)

    if(is_gnu_like)
        target_compile_options("${target}"
            PRIVATE
                $<$<CONFIG:Release>:-O3>
                $<$<CONFIG:RelWithDebInfo>:-O2>
                $<$<CONFIG:MinSizeRel>:-Os>
                $<$<CONFIG:Profile>:-O3>
                $<$<CONFIG:Profile>:-g>
                $<$<CONFIG:Profile>:-fno-omit-frame-pointer>
                $<$<CONFIG:Debug>:-O0>
                $<$<CONFIG:Debug>:-g3>
                $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        )

        if(FVM_ENABLE_NATIVE_OPTIMIZATION)
            target_compile_options("${target}"
                PRIVATE
                    $<$<CONFIG:Release>:-march=native>
                    $<$<CONFIG:RelWithDebInfo>:-march=native>
                    $<$<CONFIG:Profile>:-march=native>
            )
        endif()
    endif()

    if(is_msvc_like)
        target_compile_options("${target}"
            PRIVATE
                $<$<CONFIG:Release>:/O2>
                $<$<CONFIG:RelWithDebInfo>:/O2>
                $<$<CONFIG:MinSizeRel>:/O1>
                $<$<CONFIG:Profile>:/O2>
                $<$<CONFIG:Profile>:/Zi>
                $<$<CONFIG:Debug>:/Od>
                $<$<CONFIG:Debug>:/Zi>
        )
    endif()

    target_compile_definitions("${target}"
        PRIVATE
            $<$<CONFIG:Debug>:FVM_DEBUG=1>
            $<$<CONFIG:Debug>:FVM_ENABLE_ASSERTIONS=1>
    )

    fvm_enable_ipo("${target}")
endfunction()

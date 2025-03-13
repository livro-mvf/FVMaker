cmake_minimum_required(VERSION 3.15)
project(Programa_22 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(example_name "Programa_22")
set(SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${SRC_DIR}/**/*.cpp")
if(NOT SOURCE_FILES)
    message(WARNING "Nenhum arquivo fonte .cpp encontrado em ${SRC_DIR}")
else()
    message(STATUS "Arquivos encontrados em Programa_22: ${SOURCE_FILES}")
endif()

set(ADDITIONAL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/Programa_22.cpp)
if(EXISTS ${ADDITIONAL_SOURCE})
    list(APPEND SOURCE_FILES ${ADDITIONAL_SOURCE})
else()
    message(WARNING "Arquivo ${ADDITIONAL_SOURCE} não foi encontrado.")
endif()

add_executable(${example_name} ${SOURCE_FILES})
target_link_libraries(${example_name} PRIVATE FVMaker)

target_include_directories(${example_name} PRIVATE
    ${INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/includes
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${example_name} PRIVATE -O3 -march=native -flto)
elseif(MSVC)
    target_compile_options(${example_name} PRIVATE /O2 /GL /DNDEBUG)
endif()

set_target_properties(${example_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

add_custom_command(
    TARGET ${example_name}
    POST_BUILD
    COMMAND $<TARGET_FILE:${example_name}>
    COMMENT "Executando exemplo ${example_name} em Programa_22..."
)

# Registra a pasta "bin" para ser limpa com "make clean"
set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin)


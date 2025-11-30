#==============================================================================
# Configuracao CMake para Programa_21 - Malha de Diferenças Finitas 1D
# Versao: 1.0
# Data: 2024-06-20
# Otimizado para desempenho e execucao paralela
#==============================================================================

cmake_minimum_required(VERSION 3.15)

#==============================================================================
# Configuracao Basica do Projeto
#==============================================================================

project(Programa_21
    VERSION 1.0
    DESCRIPTION "Programa para calculo de coordenadas de nos em malha 1D usando progressao geometrica"
    LANGUAGES CXX
)

#==============================================================================
# Configuracao do Compilador
#==============================================================================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#==============================================================================
# Diretorios e Nomes
#==============================================================================

set(EXECUTABLE_NAME "Programa_21")
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(MAKE_DIRECTORY ${BIN_DIR})

#==============================================================================
# Configuracao de Fontes
#==============================================================================

file(GLOB_RECURSE SOURCE_FILES
    "${SRC_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Programa_21.cpp"
)

#==============================================================================
# Configuracao do Executavel
#==============================================================================

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#==============================================================================
# Otimizacoes de Compilacao
#==============================================================================

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE
        -O3
        -march=native
        -flto
        -fopenmp
    )
    target_link_options(${EXECUTABLE_NAME} PRIVATE -fopenmp)
elseif(MSVC)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE
        /O2
        /GL
        /arch:AVX2
        /openmp
    )
endif()

#==============================================================================
# Configuracao de Saida
#==============================================================================

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

#==============================================================================
# Alvos Personalizados
#==============================================================================

add_custom_target(run_${EXECUTABLE_NAME}
    COMMAND ${EXECUTABLE_NAME}
    WORKING_DIRECTORY ${BIN_DIR}
    DEPENDS ${EXECUTABLE_NAME}
    COMMENT "Executando ${EXECUTABLE_NAME}"
)

#==============================================================================
# Mensagens de Status
#==============================================================================

message(STATUS "Configuracao completa para ${EXECUTABLE_NAME}")
message(STATUS "Compilador: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Flags de otimizacao ativadas")
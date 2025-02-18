#==============================================================================#
#                          Configuração do CMake para FVMaker (RAIZ)           #
#==============================================================================#

cmake_minimum_required(VERSION 3.20)

project(FVMaker VERSION 1.0 LANGUAGES CXX)

# Impedir execução do CMake diretamente na raiz (opcional)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(WARNING "Erro: A configuração do CMake não pode ser feita na raiz do projeto. Apagando arquivos gerados...")

    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/CMakeCache.txt")
        file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/CMakeCache.txt")
    endif()

    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles")
        file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles")
    endif()

    if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/CMakeCache.txt" OR EXISTS "${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles")
        message(WARNING "Aviso: Nem todos os arquivos foram removidos corretamente. Verifique permissões.")
    else()
        message(STATUS "Arquivos de cache removidos com sucesso.")
    endif()

    message(FATAL_ERROR "Por favor, crie um diretório separado para compilação. Exemplo:\n  mkdir build\n  cd build\n  cmake ..")
endif()

# Definir o padrão C++ como C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Forçar o tipo de build para Release (opcional)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)

# Definições de otimização (ajuste conforme quiser)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -O3
        -march=native
        -flto
        -fno-exceptions
        -fno-rtti
        -fconstexpr-depth=8192
        -fconstexpr-ops-limit=4294967295
        -fopenmp
    )
elseif(MSVC)
    add_compile_options(
        /O2
        /GL
        /DNDEBUG
        /permissive-
        /std:c++20
    )
endif()

# ------------------------------------------------------------------------------
# Definição de caminhos para a biblioteca
set(FVMAKER_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/FVMakerLib/include)
set(FVMAKER_SRC_DIR     ${CMAKE_SOURCE_DIR}/FVMakerLib/src)
set(FVMAKER_LIB_DIR     ${CMAKE_SOURCE_DIR}/FVMakerLib/lib)

# Buscar todos os .cpp da biblioteca (recursivamente)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${FVMAKER_SRC_DIR}/**/*.cpp")

if(NOT SOURCES)
    message(FATAL_ERROR "Nenhum arquivo fonte (.cpp) encontrado na pasta ${FVMAKER_SRC_DIR}")
else()
    message(STATUS "Arquivos fonte da biblioteca encontrados: ${SOURCES}")
endif()

# Criar a biblioteca estática FVMaker
add_library(FVMaker STATIC ${SOURCES})
target_include_directories(FVMaker PUBLIC ${FVMAKER_INCLUDE_DIR})

# Definir diretórios de saída da biblioteca
set_target_properties(FVMaker PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${FVMAKER_LIB_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${FVMAKER_LIB_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${FVMAKER_LIB_DIR}
)

# ------------------------------------------------------------------------------
# Opções para testes, exemplos e documentação
option(BUILD_TESTS "Compilar e executar os testes" OFF)
option(BUILD_EXAMPLES "Compilar e executar os exemplos" ON)
option(BUILD_DOCUMENTATION "Gerar documentação com Doxygen e Sphinx" OFF)

if(BUILD_TESTS AND BUILD_EXAMPLES)
    message(FATAL_ERROR "Não é permitido habilitar BUILD_TESTS e BUILD_EXAMPLES simultaneamente.")
endif()

# Compilar testes se ativado
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()

# Compilar exemplos se ativado
if(BUILD_EXAMPLES)
    # Adiciona o diretório com os exemplos
    add_subdirectory(Capitulos)
endif()

# Geração de documentação (opcional)
if(BUILD_DOCUMENTATION)
    # Configuração da documentação (Doxygen/Sphinx) se necessário...
endif()

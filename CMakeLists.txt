#==============================================================================#
#                          Configuração do CMake para FVMaker                   #
#==============================================================================#

# Versão mínima requerida do CMake
cmake_minimum_required(VERSION 3.15)

# Configuração do projeto
project(FVMaker VERSION 1.0 LANGUAGES CXX)

# Definir o padrão C++ como C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Forçar o tipo de build para Release para desempenho
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Flags do compilador para desempenho
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -O3
        -march=native
        -mtune=native
        -flto
        -fno-exceptions
        -fno-rtti
        -fconstexpr-depth=8192
        -fconstexpr-ops-limit=4294967295
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

# Coletar todos os arquivos fonte da pasta FVMaker e subpastas
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/FVMaker/*.cpp")

# Criar a biblioteca
add_library(FVMaker STATIC ${SOURCES})

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/FVMaker/includes
)
# Diretórios de saída
set_target_properties(FVMaker PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Regras de instalação
install(TARGETS FVMaker
    ARCHIVE DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
    LIBRARY DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
    RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Obter e imprimir os diretórios de inclusão do alvo FVMaker
get_property(INCLUDE_DIRECTORIES TARGET FVMaker PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "Diretórios de inclusão para FVMaker: ${INCLUDE_DIRECTORIES}")

# Opções para compilar testes, exemplos ou documentação
option(BUILD_TESTS "Compilar e executar os testes" OFF)
option(BUILD_EXAMPLES "Compilar e executar os exemplos" ON)
option(BUILD_DOCUMENTATION "Gerar documentação com Doxygen e Sphinx" OFF)

# Garantir que apenas uma das opções de testes ou exemplos esteja habilitada
if(BUILD_TESTS AND BUILD_EXAMPLES)
    message(FATAL_ERROR "Você pode habilitar apenas BUILD_TESTS ou BUILD_EXAMPLES, não ambos.")
endif()

# Compilar e executar os testes
if(BUILD_TESTS)
    message(STATUS "Compilando os testes...")
    enable_testing()
    add_subdirectory(tests)
    
    # Adicionar um alvo personalizado para executar todos os testes após a compilação
    add_custom_target(run_tests
        COMMAND ${CMAKE_CTEST_COMMAND}
        COMMENT "Executando todos os testes..."
    )
    add_dependencies(run_tests FVMaker)
endif()

# Compilar e executar os exemplos
if(BUILD_EXAMPLES)
    message(STATUS "Compilando os exemplos...")
    add_subdirectory(Capitulos)
    
    # Adicionar um alvo personalizado para executar todos os exemplos após a compilação
    add_custom_target(run_examples
        COMMENT "Executando todos os exemplos..."
    )

    # Encontrar e executar automaticamente todos os executáveis de exemplo
    file(GLOB_RECURSE EXAMPLE_EXECUTABLES RELATIVE ${CMAKE_BINARY_DIR} "${CMAKE_BINARY_DIR}/examples/*")
    foreach(EXECUTABLE ${EXAMPLE_EXECUTABLES})
        if(IS_DIRECTORY "${CMAKE_BINARY_DIR}/${EXECUTABLE}")
            continue()
        endif()
        if(IS_EXECUTABLE "${CMAKE_BINARY_DIR}/${EXECUTABLE}")
            add_custom_command(
                TARGET run_examples POST_BUILD
                COMMAND "${CMAKE_BINARY_DIR}/${EXECUTABLE}"
                COMMENT "Executando exemplo: ${EXECUTABLE}"
            )
        endif()
    endforeach()
    add_dependencies(run_examples FVMaker)
endif()

# Geração da documentação com Doxygen e Sphinx
if(BUILD_DOCUMENTATION)
    find_package(Doxygen REQUIRED)
    if(DOXYGEN_FOUND)
        message(STATUS "Gerando documentação com Doxygen...")
        
        # Configuração do Doxygen
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        add_custom_target(doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx
            COMMENT "Gerando documentação da API com Doxygen"
            VERBATIM
        )

        # Procurar pelo executável do Sphinx
        find_program(SPHINX_EXECUTABLE sphinx-build)
        if(SPHINX_EXECUTABLE)
            message(STATUS "Gerando documentação HTML com Sphinx...")
            
            add_custom_target(doc_sphinx ALL
                COMMAND ${SPHINX_EXECUTABLE} -b html ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx ${CMAKE_CURRENT_BINARY_DIR}/sphinx
                COMMENT "Gerando documentação HTML com Sphinx"
                VERBATIM
            )
            add_dependencies(doc_sphinx doc_doxygen)
        else()
            message(WARNING "Sphinx não encontrado. Apenas a documentação do Doxygen será gerada.")
        endif()
    else()
        message(FATAL_ERROR "Doxygen não encontrado. Por favor, instale o Doxygen para gerar a documentação.")
    endif()
endif()

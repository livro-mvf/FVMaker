cmake_minimum_required(VERSION 3.15)

# Definir o nome do projeto e a versão
project(FVMaker VERSION 1.0 LANGUAGES CXX)

# Definir o padrão C++ para C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Definir o diretório de include da biblioteca
include_directories(${CMAKE_SOURCE_DIR}/FVMAKER/include)

# Coletar todos os arquivos fonte da pasta src
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Verificar se arquivos fonte foram encontrados
if(NOT SOURCES)
    message(FATAL_ERROR "Erro: Nenhum arquivo fonte (.cpp) encontrado na pasta src.")
else()
    message(STATUS "Arquivos fonte encontrados: ${SOURCES}")
endif()

# Criar a biblioteca estática (FVMaker)
add_library(FVMaker STATIC ${SOURCES})

# Definir diretórios de saída para a biblioteca compilada
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

# Adicionar a opção de testes, se necessário
option(BUILD_TESTS "Compilar e executar os testes" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Testes)
endif()

# Adicionar a opção de geração de documentação, se necessário
option(BUILD_DOCUMENTATION "Gerar documentação com Doxygen" OFF)

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
    else()
        message(FATAL_ERROR "Doxygen não encontrado. Por favor, instale o Doxygen para gerar a documentação.")
    endif()
endif()

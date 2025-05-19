# ========================================================
# Configuração CMake para Programa_22 - Malha de Volumes Finitos 1D
# Otimizado para desempenho e execução paralela
# ========================================================

cmake_minimum_required(VERSION 3.15)

# ========================
# Configuração do Projeto
# ========================

project(Programa_22
    VERSION 1.0
    DESCRIPTION "Cálculo de coordenadas dos nós em malha 1D via progressão geométrica"
    LANGUAGES CXX
)

# ========================
# Configuração do Compilador
# ========================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ========================
# Diretórios e Nomes
# ========================

set(EXECUTABLE_NAME "Programa_22")
set(SRC_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BIN_DIR       ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Cria diretório de binários
file(MAKE_DIRECTORY ${BIN_DIR})

# ========================
# Fontes do Projeto
# ========================

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${SRC_DIR}/**/*.cpp")
if(NOT SOURCE_FILES)
    message(WARNING "Nenhum arquivo fonte .cpp encontrado em ${SRC_DIR}")
endif()

# ========================
# Definição do Executável
# ========================

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_include_directories(${EXECUTABLE_NAME}
    PRIVATE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

# Vincula biblioteca FVMaker (assume configuração via find_package)
find_package(FVMaker CONFIG REQUIRED)
target_link_libraries(${EXECUTABLE_NAME}
    PRIVATE
        FVMaker::FVMaker
)

# ========================
# Otimizações de Compilação
# ========================

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${EXECUTABLE_NAME} PRIVATE -O3 -march=native -flto)
elseif(MSVC)
    target_compile_options(${EXECUTABLE_NAME} PRIVATE /O2 /GL /DNDEBUG)
endif()

# Define diretório de saída do executável
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

# ========================
# Informações de Build
# ========================

message(STATUS "Compilando ${EXECUTABLE_NAME} com otimizações máximas")
message(STATUS "Compilador: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Arquivos fonte: ${SOURCE_FILES}")

# ========================
# Alvo de Execução
# ========================

add_custom_target(executar_${EXECUTABLE_NAME}
    COMMAND ${EXECUTABLE_NAME}
    WORKING_DIRECTORY ${BIN_DIR}
    DEPENDS ${EXECUTABLE_NAME}
    COMMENT "Executando ${EXECUTABLE_NAME}"
)

# Garante que 'make clean' remova o diretório bin/
set_property(DIRECTORY PROPERTY
    ADDITIONAL_MAKE_CLEAN_FILES ${BIN_DIR}
)

# ========================================================
# Alvos de Documentação
# ========================================================

# Documentação com Doxygen
dfind_package(Doxygen REQUIRED dot)
if(DOXYGEN_FOUND)
    set(DOXYGEN_IN  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Gerando documentação Doxygen"
        VERBATIM
    )
endif()

# Documentação com Sphinx
find_package(Sphinx REQUIRED)
if(SPHINX_FOUND)
    set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/docs_sphinx)
    set(SPHINX_BUILD  ${CMAKE_CURRENT_BINARY_DIR}/docs_html)
    add_custom_target(doc_sphinx ALL
        COMMAND ${SPHINX_EXECUTABLE} -b html ${SPHINX_SOURCE} ${SPHINX_BUILD}
        WORKING_DIRECTORY ${SPHINX_SOURCE}
        COMMENT "Gerando documentação Sphinx"
        VERBATIM
    )
endif()

# Alvo agregador de documentação
add_custom_target(doc
    DEPENDS doc_doxygen doc_sphinx
    COMMENT "Gerando toda a documentação"
)

cmake_minimum_required(VERSION 3.15)

# Project configuration
project(MalhaFronteira VERSION 1.0 LANGUAGES CXX)

# Define o nome do executável
set(example_name "MalhaFronteira")

# Defina o padrão C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Inclua os diretórios para cabeçalhos e fontes
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Busque recursivamente todos os arquivos .cpp na pasta src
file(GLOB_RECURSE SOURCE_FILES "${SRC_DIR}/*.cpp")

# Adicione o arquivo MalhaFronteira.cpp explicitamente
set(ADDITIONAL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/MalhaFronteira.cpp)
if(NOT EXISTS ${ADDITIONAL_SOURCE})
    message(FATAL_ERROR "The source file ${ADDITIONAL_SOURCE} was not found.")
endif()
list(APPEND SOURCE_FILES ${ADDITIONAL_SOURCE})

# Crie o executável
add_executable(${example_name} ${SOURCE_FILES})

# Inclua os diretórios de cabeçalhos
target_include_directories(${example_name} PRIVATE
    ${INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/includes
)

# Configure opções de compilação para otimização
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${example_name} PRIVATE -O3 -march=native -flto)
elseif(MSVC)
    target_compile_options(${example_name} PRIVATE /O2 /GL /DNDEBUG)
endif()

# Configure o diretório de saída do executável
set_target_properties(${example_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

# Adicione o exemplo ao alvo run_examples
if(TARGET run_examples)
    add_dependencies(run_examples ${example_name})
    add_custom_command(
        TARGET ${example_name}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "Executable ${example_name} successfully built!"
        COMMAND ${CMAKE_COMMAND} -E echo "Running example ${example_name}..."
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Running ${example_name}"
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/${example_name}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running example ${example_name}..."
    )
endif()

# Mensagens de depuração (opcional)
message(STATUS "SOURCES: ${SOURCE_FILES}")
message(STATUS "INCLUDE_DIR: ${INCLUDE_DIR}")
message(STATUS "SRC_DIR: ${SRC_DIR}")

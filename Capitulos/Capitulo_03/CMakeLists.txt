cmake_minimum_required(VERSION 3.10)
project(Capitulo_02)

# Definir o padrão C++ para C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Busca recursiva de TODOS os arquivos .cpp em Capitulo_02 e suas subpastas
file(GLOB_RECURSE EXAMPLES_SRC CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
)

if(NOT EXAMPLES_SRC)
    message(WARNING "Nenhum arquivo fonte .cpp foi encontrado em ${CMAKE_CURRENT_SOURCE_DIR}")
else()
    message(STATUS "Arquivos encontrados em Capitulo_02: ${EXAMPLES_SRC}")
endif()

foreach(EXAMPLE ${EXAMPLES_SRC})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE})
    target_link_libraries(${EXAMPLE_NAME} PRIVATE FVMaker)

    # Obtém o diretório do arquivo fonte
    get_filename_component(SRC_DIR ${EXAMPLE} PATH)
    set_target_properties(${EXAMPLE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${SRC_DIR}/bin"
    )

    add_custom_command(
        TARGET ${EXAMPLE_NAME}
        POST_BUILD
        COMMAND $<TARGET_FILE:${EXAMPLE_NAME}>
        COMMENT "Executando exemplo ${EXAMPLE_NAME}..."
    )
endforeach()

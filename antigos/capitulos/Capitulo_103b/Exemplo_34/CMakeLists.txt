cmake_minimum_required(VERSION 3.15)
project(Exemplo34 VERSION 1.0.0 LANGUAGES CXX)

# Configurações padrão
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Opção para habilitar paralelismo
option(ENABLE_PARALLEL "Habilitar execução paralela (requer TBB)" ON)

# Verificar TBB se paralelismo habilitado
if(ENABLE_PARALLEL)
    find_package(TBB QUIET)
    if(TBB_FOUND)
        message(STATUS "TBB encontrada: paralelismo habilitado")
        set(USE_PARALLEL 1)
    else()
        message(WARNING "TBB não encontrada. Paralelismo desabilitado.")
        set(USE_PARALLEL 0)
        set(ENABLE_PARALLEL OFF)
    endif()
else()
    set(USE_PARALLEL 0)
    message(STATUS "Paralelismo desabilitado pelo usuário")
endif()

# Criar executável
add_executable(Exemplo_34 Exemplo_34.cpp)

# Configurações baseadas nas opções
if(ENABLE_PARALLEL AND TBB_FOUND)
    target_link_libraries(Exemplo_34 PUBLIC TBB::tbb)
    target_compile_definitions(Exemplo_34 PRIVATE 
        ENABLE_PARALLEL=1
        USE_PARALLEL=1
    )
else()
    target_compile_definitions(Exemplo_34 PRIVATE 
        ENABLE_PARALLEL=0
        USE_PARALLEL=0
    )
endif()

# Flags de compilação
target_compile_options(Exemplo_34 PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
)

# Instalação
install(TARGETS Exemplo_34 DESTINATION bin)

# Mensagem final
message(STATUS "----------------------------------------")
message(STATUS "Configuração do Exemplo 34:")
message(STATUS "  Versão: ${PROJECT_VERSION}")
message(STATUS "  Paralelismo: ${ENABLE_PARALLEL}")
if(ENABLE_PARALLEL)
    message(STATUS "  TBB: ${TBB_VERSION}")
endif()
message(STATUS "----------------------------------------")
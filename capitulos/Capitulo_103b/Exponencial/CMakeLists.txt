# CMakeLists.txt com verificação automática
cmake_minimum_required(VERSION 3.15)
project(Exponencial VERSION 1.0.0 LANGUAGES CXX)

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
add_executable(Exponencial Exponencial.cpp)

# Configurações baseadas nas opções
if(ENABLE_PARALLEL AND TBB_FOUND)
    target_link_libraries(Exponencial PUBLIC TBB::tbb)
    target_compile_definitions(Exponencial PRIVATE 
        ENABLE_PARALLEL=1
        USE_PARALLEL=1
    )
else()
    target_compile_definitions(Exponencial PRIVATE 
        ENABLE_PARALLEL=0
        USE_PARALLEL=0
    )
endif()

# Flags de compilação
target_compile_options(Exponencial PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
)

# Instalação
install(TARGETS Exponencial DESTINATION bin)

# Mensagem final
message(STATUS "----------------------------------------")
message(STATUS "Configuração do Malha Exponencial:")
message(STATUS "  Versão: ${PROJECT_VERSION}")
message(STATUS "  Paralelismo: ${ENABLE_PARALLEL}")
if(ENABLE_PARALLEL)
    message(STATUS "  TBB: ${TBB_VERSION}")
endif()
message(STATUS "----------------------------------------")
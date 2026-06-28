// ============================================================================
// Arquivo: FVMakerException.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa FVMaker Exception no contexto de Error.
// Autor: João Flávio Vieira de Vasconcellos
//
// SPDX-FileCopyrightText: 2026 João Flávio Vieira de Vasconcellos
// SPDX-License-Identifier: BSD-3-Clause
//
// Este arquivo faz parte do FVMaker.
//
// Licença: BSD 3-Clause.
// É permitido usar, copiar, modificar e redistribuir este arquivo, em código-fonte
// ou forma binária, com ou sem modificações, desde que sejam preservados os avisos
// de copyright, esta identificação de licença e as condições descritas no arquivo
// LICENSE.md.
//
// O nome do autor, de colaboradores ou de instituições associadas ao projeto não
// pode ser usado para endossar ou promover produtos derivados sem autorização
// prévia por escrito.
//
// Este software é fornecido sem garantias de qualquer natureza. Consulte o arquivo
// LICENSE.md, na raiz do repositório, para o texto completo da licença.
// ============================================================================

#include <format>
#include <mutex>
#include <vector>

//==============================================================================
// Includes da biblioteca FVMaker
//==============================================================================

#include <FVMaker/Error/FVMakerException.h>



FVMAKER_NAMESPACE_OPEN

// Definicao do catalogo de erros com unordered_map
const std::unordered_map<ErrorCode, std::string_view> errorCatalog = []() {
    std::unordered_map<ErrorCode, std::string_view> catalog;
    
    // Pre-reserva espaco para todos os erros (melhora performance de insercao)
    catalog.reserve(15); // Atualize este numero quando adicionar novos erros
    
    // Insere todos os pares codigo-mensagem
    catalog.insert({
        { ErrorCode::GridGenerationError,           "Erro nao definido na geracao da malha." },
        { ErrorCode::InvalidGridDimension,          "Dimensao da Grid nao definida." },            
        { ErrorCode::InvalidBeta,                   "Beta <= 1.0 invalido." },
        { ErrorCode::InvalidLength,                 "Comprimento do dominio invalido." },
        { ErrorCode::InvalidNumbersOfVolumes,       "Numero de volumes da malha invalido." },
        { ErrorCode::InvalidOption,                 "Opcao nao liberada." },
        { ErrorCode::MemoryAllocationError,         "Erro ao alocar memoria (std::bad_alloc)." },
        { ErrorCode::UndefiniedCoefficient,         "Coeficiente nao definido." },
        { ErrorCode::UndefiniedFunction,            "Funcao nao definida." },
        { ErrorCode::UndefiniedGrid,                "Malha nao definida." },
        { ErrorCode::UnhandledException,            "Excecao nao tratada." },            
        { ErrorCode::UnorderedCenterCoordinates,    "As coordenadas dos centros nao estao ordenadas." },
        { ErrorCode::UnorderedFaceCoordinates,      "As coordenadas das faces nao estao ordenadas." },
        { ErrorCode::VectorResizeError,             "Erro ao redimensionar vetor (std::length_error)." },
        { ErrorCode::ExampleNewError1,              "Exemplo de novo erro 1." },
        { ErrorCode::ExampleNewError2,              "Exemplo de novo erro 2." }
        // Adicione novos erros aqui
    });
    
    return catalog;
}();

std::string_view getErrorMessage(ErrorCode code) noexcept {
    // Busca o erro no catalogo
    const auto it = errorCatalog.find(code);
    
    // Retorna a mensagem se encontrado, ou mensagem padrao caso contrario
    return it != errorCatalog.end() ? it->second : "Erro desconhecido.";
}

// Implementacao do cache de mensagens (interno ao .cpp)
namespace {
    struct CachedMessage {
        ErrorCode code;
        std::source_location loc;
        std::string message;
        
        bool matches(ErrorCode c, const std::source_location& l) const {
            return code == c && 
                   loc.line() == l.line() &&
                   std::string_view(loc.file_name()) == loc.file_name() &&
                   std::string_view(loc.function_name()) == loc.function_name();
        }
    };
    
    std::vector<CachedMessage> messageCache;
    std::mutex cacheMutex;
}

FVMakerException::FVMakerException(ErrorCode code, const std::source_location& loc) noexcept
    : code_(code) {
    // Primeiro verifica no cache
    {
        std::lock_guard<std::mutex> lock(cacheMutex);
        for (const auto& cached : messageCache) {
            if (cached.matches(code, loc)) {
                message_ = cached.message;
                return;
            }
        }
    }
    
    // Se nao encontrou no cache, cria nova mensagem
    message_ = std::format("{}\nArquivo: {}\nLinha: {}\nFuncao: {}",
                          getErrorMessage(code),
                          loc.file_name(),
                          loc.line(),
                          loc.function_name());
    
    // Adiciona ao cache (se ainda houver espaco)
    {
        std::lock_guard<std::mutex> lock(cacheMutex);
        if (messageCache.size() < 100) { // Limite maximo do cache
            messageCache.push_back({code, loc, message_});
        }
    }
}

const char* FVMakerException::what() const noexcept {
    return message_.c_str();
}

ErrorCode FVMakerException::getCode() const noexcept {
    return code_;
}

FVMAKER_NAMESPACE_CLOSE
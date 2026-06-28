// ============================================================================
// Arquivo: ThrowError.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Throw Error no contexto de ErrorHandling.
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

#include <utility>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ThrowError.h>

namespace fvm {

[[noreturn]] void throw_error(
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location
) {
    throw_error(
        descriptor.code,
        std::string{descriptor.message},
        descriptor.category,
        source,
        location
    );
}

[[noreturn]] void throw_error(
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location
) {
    throw FVMException{
        ErrorRecord{
            .code = code,
            .message = std::move(message),
            .category = category,
            .source = source,
            .location = location,
        }
    };
}

void require(
    bool condition,
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location
) {
    if (!condition) {
        throw_error(descriptor, source, location);
    }
}

void require(
    bool condition,
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location
) {
    if (!condition) {
        throw_error(code, std::move(message), category, source, location);
    }
}

}  // namespace fvm

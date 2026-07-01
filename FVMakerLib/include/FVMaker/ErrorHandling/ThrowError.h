// ============================================================================
// Arquivo: ThrowError.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Throw Error no contexto de ErrorHandling.
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

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <source_location>
#include <string>
#include <string_view>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorDescriptor.h>
#include <FVMaker/ErrorHandling/FVMException.h>

namespace fvm {

// Realiza a operacao throw error definida por esta interface.
[[noreturn]] void throw_error(
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location = std::source_location::current()
);

// Realiza a operacao throw error definida por esta interface.
[[noreturn]] void throw_error(
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location = std::source_location::current()
);

// Realiza a operacao require definida por esta interface.
void require(
    bool condition,
    ErrorDescriptor descriptor,
    ID source,
    std::source_location location = std::source_location::current()
);

// Realiza a operacao require definida por esta interface.
void require(
    bool condition,
    std::string_view code,
    std::string message,
    std::string_view category,
    ID source,
    std::source_location location = std::source_location::current()
);

}  // namespace fvm

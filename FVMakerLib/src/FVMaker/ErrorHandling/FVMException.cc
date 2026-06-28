// ============================================================================
// Arquivo: FVMException.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa FVMException no contexto de ErrorHandling.
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

#include <sstream>
#include <utility>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/FVMException.h>

namespace fvm {

namespace {

[[nodiscard]] std::string format_error_message(const ErrorRecord& record) {
    std::ostringstream stream;

    stream << "[" << record.code << "] " << record.message;

    if (!record.category.empty()) {
        stream << " (category: " << record.category << ")";
    }

    if (!record.source.module().empty()) {
        stream << " (module: " << record.source.module() << ")";
    }

    if (!record.source.class_name().empty()) {
        stream << " (class: " << record.source.class_name() << ")";
    }

    if (!record.source.class_id().empty()) {
        stream << " (class id: " << record.source.class_id() << ")";
    }

    stream << " at " << record.location.file_name() << ":"
           << record.location.line() << " in "
           << record.location.function_name();

    return stream.str();
}

}  // namespace

FVMException::FVMException(ErrorRecord record)
    : record_(std::move(record)),
      formatted_message_(format_error_message(record_)) {}

const char* FVMException::what() const noexcept {
    return formatted_message_.c_str();
}

const ErrorRecord& FVMException::record() const noexcept {
    return record_;
}

}  // namespace fvm

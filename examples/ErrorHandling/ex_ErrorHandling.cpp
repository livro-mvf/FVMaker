// ============================================================================
// Arquivo: ex_ErrorHandling.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa ex Error Handling no contexto de projeto.
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

#include <iostream>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/ErrorHandling/ThrowError.h>

int main() {
    const fvm::ID example_id{
        "Examples",
        "Ex_ErrorHandling",
        "fvm.examples.Ex_ErrorHandling"
    };

    try {
        fvm::require(false, fvm::error_catalog::kMissingMesh, example_id);
    } catch (const fvm::FVMException& exception) {
        const fvm::ErrorRecord& record = exception.record();

        std::cout << "Caught FVMException\n";
        std::cout << "code       : " << record.code << '\n';
        std::cout << "category   : " << record.category << '\n';
        std::cout << "module     : " << record.source.module() << '\n';
        std::cout << "class name : " << record.source.class_name() << '\n';
        std::cout << "class id   : " << record.source.class_id() << '\n';
        std::cout << "message    : " << record.message << '\n';
        std::cout << "what       : " << exception.what() << '\n';

        return 0;
    }

    return 1;
}

// ============================================================================
// Arquivo: tst_ErrorRecord.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Error Record no contexto de ErrorHandling.
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

#include <source_location>
#include <string_view>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorRecord.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(ErrorRecord, StoresCodeMessageCategoryAndSource) {
    const ErrorRecord record{
        .code = "FVM.TEST.ERROR",
        .message = "test error message",
        .category = "Test",
        .source =
            ID{"ErrorHandling", "ErrorRecordTest", "fvm.test.ErrorRecordTest"},
    };

    EXPECT_EQ(record.code, std::string_view{"FVM.TEST.ERROR"});
    EXPECT_EQ(record.message, "test error message");
    EXPECT_EQ(record.category, std::string_view{"Test"});
    EXPECT_EQ(record.source.module(), std::string_view{"ErrorHandling"});
    EXPECT_EQ(record.source.class_name(), std::string_view{"ErrorRecordTest"});
    EXPECT_EQ(
        record.source.class_id(),
        std::string_view{"fvm.test.ErrorRecordTest"}
    );
}

TEST(ErrorRecord, StoresExplicitSourceLocation) {
    constexpr auto location = std::source_location::current();

    const ErrorRecord record{
        .code = "FVM.TEST.LOCATION",
        .message = "location test",
        .category = "Test",
        .source =
            ID{"ErrorHandling", "ErrorRecordTest", "fvm.test.ErrorRecordTest"},
        .location = location,
    };

    EXPECT_EQ(record.location.line(), location.line());
    EXPECT_STREQ(record.location.file_name(), location.file_name());
}

}  // namespace fvm

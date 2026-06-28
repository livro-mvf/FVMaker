// ============================================================================
// Arquivo: tst_FVMException.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst FVMException no contexto de ErrorHandling.
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
#include <string>
#include <string_view>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>
#include <FVMaker/ErrorHandling/FVMException.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(FVMException, StoresErrorRecord) {
    const ErrorRecord record{
        .code = "FVM.TEST.EXCEPTION",
        .message = "exception test message",
        .category = "Test",
        .source = ID{
            "ErrorHandling",
            "FVMExceptionTest",
            "fvm.test.FVMExceptionTest"
        },
    };

    const FVMException exception{record};

    EXPECT_EQ(exception.record().code, std::string_view{"FVM.TEST.EXCEPTION"});
    EXPECT_EQ(exception.record().message, "exception test message");
    EXPECT_EQ(exception.record().category, std::string_view{"Test"});
    EXPECT_EQ(exception.record().source.module(), std::string_view{"ErrorHandling"});
    EXPECT_EQ(
        exception.record().source.class_name(),
        std::string_view{"FVMExceptionTest"}
    );
    EXPECT_EQ(
        exception.record().source.class_id(),
        std::string_view{"fvm.test.FVMExceptionTest"}
    );
}

TEST(FVMException, WhatContainsCodeMessageCategoryAndSource) {
    const ErrorRecord record{
        .code = "FVM.TEST.WHAT",
        .message = "what test message",
        .category = "Test",
        .source = ID{
            "ErrorHandling",
            "FVMExceptionTest",
            "fvm.test.FVMExceptionTest"
        },
    };

    const FVMException exception{record};
    const std::string what{exception.what()};

    EXPECT_NE(what.find("FVM.TEST.WHAT"), std::string::npos);
    EXPECT_NE(what.find("what test message"), std::string::npos);
    EXPECT_NE(what.find("Test"), std::string::npos);
    EXPECT_NE(what.find("ErrorHandling"), std::string::npos);
    EXPECT_NE(what.find("FVMExceptionTest"), std::string::npos);
    EXPECT_NE(what.find("fvm.test.FVMExceptionTest"), std::string::npos);
}

TEST(FVMException, WhatContainsSourceLocation) {
    constexpr auto location = std::source_location::current();

    const ErrorRecord record{
        .code = "FVM.TEST.LOCATION",
        .message = "location test message",
        .category = "Test",
        .source = ID{
            "ErrorHandling",
            "FVMExceptionTest",
            "fvm.test.FVMExceptionTest"
        },
        .location = location,
    };

    const FVMException exception{record};
    const std::string what{exception.what()};

    EXPECT_NE(what.find(location.file_name()), std::string::npos);
}

}  // namespace fvm

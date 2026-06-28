// ============================================================================
// Arquivo: tst_Version.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Version no contexto de Core.
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

#include <string_view>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker.h>
#include <FVMaker/Core/Version.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(Version, VersionNumbersAreAvailable) {
    EXPECT_EQ(version_major(), 0);
    EXPECT_EQ(version_minor(), 1);
    EXPECT_EQ(version_patch(), 0);
}

TEST(Version, VersionStringIsAvailable) {
    EXPECT_EQ(version_string(), std::string_view{"0.1.0"});
}

TEST(Version, RootVersionFunctionMatchesVersionString) {
    EXPECT_STREQ(version(), version_string().data());
}

TEST(Version, VersionConstantsMatchAccessors) {
    EXPECT_EQ(kVersionMajor, version_major());
    EXPECT_EQ(kVersionMinor, version_minor());
    EXPECT_EQ(kVersionPatch, version_patch());
    EXPECT_EQ(kVersionString, version_string());
}

}  // namespace fvm

// ============================================================================
// Arquivo: tst_ID.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst ID no contexto de Core.
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
#include <utility>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(ID, DefaultIdentityIsUnknown) {
    constexpr ID id;

    EXPECT_EQ(id.module(), std::string_view{"Unknown"});
    EXPECT_EQ(id.class_name(), std::string_view{"Unknown"});
    EXPECT_EQ(id.class_id(), std::string_view{"fvm.unknown.Unknown"});
}

TEST(ID, StoresModuleClassNameAndClassId) {
    constexpr ID id{
        "OneDimensional",
        "Field1D",
        "fvm.1d.field.Field1D"
    };

    EXPECT_EQ(id.module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(id.class_name(), std::string_view{"Field1D"});
    EXPECT_EQ(id.class_id(), std::string_view{"fvm.1d.field.Field1D"});
}

TEST(ID, IsCopyConstructible) {
    constexpr ID original{
        "Core",
        "ID",
        "fvm.core.ID"
    };

    constexpr ID copy{original};

    EXPECT_EQ(copy.module(), original.module());
    EXPECT_EQ(copy.class_name(), original.class_name());
    EXPECT_EQ(copy.class_id(), original.class_id());
}

TEST(ID, IsMoveConstructible) {
    ID original{
        "Core",
        "ID",
        "fvm.core.ID"
    };

    const ID moved{std::move(original)};

    EXPECT_EQ(moved.module(), std::string_view{"Core"});
    EXPECT_EQ(moved.class_name(), std::string_view{"ID"});
    EXPECT_EQ(moved.class_id(), std::string_view{"fvm.core.ID"});
}

}  // namespace fvm

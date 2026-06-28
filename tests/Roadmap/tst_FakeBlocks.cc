// ============================================================================
// Arquivo: tst_FakeBlocks.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Fake Blocks no contexto de Roadmap.
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

#include <FVMaker/Roadmap/FakeBlocks.h>

#include <gtest/gtest.h>

namespace fvm::roadmap {

TEST(FakeBlocks, ExposesAllRemainingBlocks) {
    constexpr auto blocks = remaining_blocks();

    static_assert(blocks.size() == 15);
    EXPECT_EQ(blocks.front().block, 15);
    EXPECT_EQ(blocks.back().block, 29);
    EXPECT_EQ(blocks.front().name, std::string_view{"Transient1D/Ddt1D"});
    EXPECT_EQ(blocks.back().name, std::string_view{"PerformanceProfiling"});
}

TEST(FakeBlocks, AllRemainingBlocksAreCallableAndProvisional) {
    EXPECT_TRUE(all_remaining_blocks_callable());

    for (const FakeBlockResult block : remaining_blocks()) {
        EXPECT_TRUE(block.callable);
        EXPECT_TRUE(block.provisional);
    }
}

TEST(FakeBlocks, CallsRepresentativeRoutines) {
    EXPECT_EQ(time_schemes_1d().block, 16);
    EXPECT_EQ(grid_view_2d().block, 18);
    EXPECT_EQ(advection_2d().block, 26);
    EXPECT_EQ(yaml_examples().block, 28);
}

}  // namespace fvm::roadmap

// ============================================================================
// Arquivo: tst_GhostCells1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Ghost Cells 1 D no contexto de OneDimensional / Boundary.
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

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Boundary/GhostCells1D.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(GhostCells1D, StoresClassIdentity) {
    EXPECT_EQ(GhostCells1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(GhostCells1D::id().class_name(), std::string_view{"GhostCells1D"});
    EXPECT_EQ(
        GhostCells1D::id().class_id(),
        std::string_view{"fvm.1d.boundary.GhostCells1D"}
    );
}

TEST(GhostCells1D, DefaultHasNoGhostCells) {
    const GhostCells1D ghost_cells;

    EXPECT_EQ(ghost_cells.left(), static_cast<Size>(0));
    EXPECT_EQ(ghost_cells.right(), static_cast<Size>(0));
    EXPECT_EQ(ghost_cells.total(), static_cast<Size>(0));
    EXPECT_TRUE(ghost_cells.empty());
    EXPECT_EQ(ghost_cells.storage_size(5), static_cast<Size>(5));
    EXPECT_EQ(ghost_cells.storage_index(3), static_cast<Size>(3));
}

TEST(GhostCells1D, AcceptsUpToTwoGhostCellsPerSide) {
    const GhostCells1D ghost_cells{2, 1};

    EXPECT_EQ(GhostCells1D::max_per_side(), static_cast<Size>(2));
    EXPECT_EQ(ghost_cells.left(), static_cast<Size>(2));
    EXPECT_EQ(ghost_cells.right(), static_cast<Size>(1));
    EXPECT_EQ(ghost_cells.total(), static_cast<Size>(3));
    EXPECT_FALSE(ghost_cells.empty());
    EXPECT_EQ(ghost_cells.first_physical_index(), static_cast<Size>(2));
    EXPECT_EQ(ghost_cells.storage_size(10), static_cast<Size>(13));
    EXPECT_EQ(ghost_cells.storage_index(0), static_cast<Size>(2));
    EXPECT_EQ(ghost_cells.storage_index(9), static_cast<Size>(11));
}

TEST(GhostCells1D, RejectsMoreThanTwoGhostCellsPerSide) {
    try {
        const GhostCells1D ghost_cells{3, 0};
        (void)ghost_cells;
    } catch (const FVMException& exception) {
        EXPECT_EQ(
            exception.record().code,
            error_catalog::kInvalidBoundaryCondition.code
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Boundary"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"GhostCells1D"}
        );
        return;
    }

    FAIL() << "GhostCells1D did not reject more than two ghost cells.";
}

}  // namespace fvm

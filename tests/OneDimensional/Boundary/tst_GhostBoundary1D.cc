// ============================================================================
// Arquivo: tst_GhostBoundary1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Ghost Boundary 1 D no contexto de OneDimensional / Boundary.
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
#include <FVMaker/OneDimensional/Boundary/GhostBoundary1D.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(GhostBoundary1D, ComputesDirichletGhostValue) {
    const BoundaryCondition1D condition = dirichlet_1d(3.0);

    EXPECT_DOUBLE_EQ(
        first_ghost_value(
            condition,
            BoundarySide1D::left,
            0.0,
            2.0,
            0.25
        ),
        4.0
    );
}

TEST(GhostBoundary1D, LinearizesDirichletGhostValue) {
    const BoundaryCondition1D condition = dirichlet_1d(3.0);

    const GhostCellLinearization1D linearization = first_ghost_linearization(
        condition,
        BoundarySide1D::left,
        0.0,
        0.25
    );

    EXPECT_DOUBLE_EQ(linearization.constant, 6.0);
    EXPECT_DOUBLE_EQ(linearization.interior_coefficient, -1.0);
}

TEST(GhostBoundary1D, ComputesLeftNeumannGhostValueUsingDxDerivative) {
    const BoundaryCondition1D condition = neumann_1d(4.0);

    EXPECT_DOUBLE_EQ(
        first_ghost_value(
            condition,
            BoundarySide1D::left,
            0.0,
            10.0,
            0.5
        ),
        8.0
    );
}

TEST(GhostBoundary1D, ComputesRightNeumannGhostValueUsingDxDerivative) {
    const BoundaryCondition1D condition = neumann_1d(4.0);

    EXPECT_DOUBLE_EQ(
        first_ghost_value(
            condition,
            BoundarySide1D::right,
            1.0,
            10.0,
            0.5
        ),
        12.0
    );
}

TEST(GhostBoundary1D, LinearizesLeftNeumannGhostValue) {
    const BoundaryCondition1D condition = neumann_1d(4.0);

    const GhostCellLinearization1D linearization = first_ghost_linearization(
        condition,
        BoundarySide1D::left,
        0.0,
        0.5
    );

    EXPECT_DOUBLE_EQ(linearization.constant, -2.0);
    EXPECT_DOUBLE_EQ(linearization.interior_coefficient, 1.0);
}

TEST(GhostBoundary1D, ComputesLeftRobinGhostValue) {
    const BoundaryCondition1D condition = robin_1d(1.0, 2.0, 5.0);

    const Real ghost = first_ghost_value(
        condition,
        BoundarySide1D::left,
        0.0,
        3.0,
        0.5
    );

    EXPECT_DOUBLE_EQ(ghost, 2.4285714285714284);
}

TEST(GhostBoundary1D, ComputesRightRobinGhostValue) {
    const BoundaryCondition1D condition = robin_1d(1.0, 2.0, 5.0);

    const Real ghost = first_ghost_value(
        condition,
        BoundarySide1D::right,
        1.0,
        3.0,
        0.5
    );

    EXPECT_DOUBLE_EQ(ghost, 3.4444444444444446);
}

TEST(GhostBoundary1D, UsesBoundarySetSide) {
    const BoundarySet1D boundaries{dirichlet_1d(4.0), neumann_1d(1.0)};

    EXPECT_DOUBLE_EQ(
        first_ghost_value(
            boundaries,
            BoundarySide1D::left,
            0.0,
            2.0,
            0.5
        ),
        6.0
    );

    EXPECT_DOUBLE_EQ(
        first_ghost_value(
            boundaries,
            BoundarySide1D::right,
            1.0,
            2.0,
            0.5
        ),
        2.5
    );
}

TEST(GhostBoundary1D, RejectsInvalidDistance) {
    try {
        static_cast<void>(
            first_ghost_value(
                dirichlet_1d(1.0),
                BoundarySide1D::left,
                0.0,
                2.0,
                0.0
            )
        );
    } catch (const FVMException& exception) {
        EXPECT_EQ(
            exception.record().code,
            error_catalog::kInvalidBoundaryCondition.code
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Boundary"});
        return;
    }

    FAIL() << "first_ghost_value did not reject an invalid distance.";
}

}  // namespace fvm

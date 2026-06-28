// ============================================================================
// Arquivo: tst_Terms1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Terms 1 D no contexto de OneDimensional / Term.
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

#include <array>
#include <cmath>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>
#include <FVMaker/OneDimensional/Term/Terms1D.h>

#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] Real source(Real x) {
    return x * x;
}

}  // namespace

TEST(EquationContribution1D, AddsContributionsCoefficientWise) {
    EquationContribution1D left{2};
    EquationContribution1D right{2};

    left.aw()[0] = 1.0;
    left.ap()[0] = 2.0;
    left.ae()[0] = 3.0;
    left.bp()[0] = 4.0;

    right.aw()[0] = 10.0;
    right.ap()[0] = 20.0;
    right.ae()[0] = 30.0;
    right.bp()[0] = 40.0;

    const EquationContribution1D sum = left + right;

    EXPECT_DOUBLE_EQ(sum.aw()[0], 11.0);
    EXPECT_DOUBLE_EQ(sum.ap()[0], 22.0);
    EXPECT_DOUBLE_EQ(sum.ae()[0], 33.0);
    EXPECT_DOUBLE_EQ(sum.bp()[0], 44.0);
}

TEST(EquationContribution1D, ConvertsBookConventionToTridiagonalSystem) {
    EquationContribution1D contribution{3};
    contribution.aw()[0] = 0.0;
    contribution.aw()[1] = 1.0;
    contribution.aw()[2] = 2.0;
    contribution.ap()[0] = 3.0;
    contribution.ap()[1] = 4.0;
    contribution.ap()[2] = 5.0;
    contribution.ae()[0] = 6.0;
    contribution.ae()[1] = 7.0;
    contribution.ae()[2] = 0.0;
    contribution.bp()[0] = 8.0;
    contribution.bp()[1] = 9.0;
    contribution.bp()[2] = 10.0;

    const TridiagonalSystem1D system = to_tridiagonal_system(contribution);

    EXPECT_DOUBLE_EQ(system.lower()[0], -1.0);
    EXPECT_DOUBLE_EQ(system.lower()[1], -2.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[1], 4.0);
    EXPECT_DOUBLE_EQ(system.upper()[0], -6.0);
    EXPECT_DOUBLE_EQ(system.upper()[1], -7.0);
    EXPECT_DOUBLE_EQ(system.rhs()[2], 10.0);
}

TEST(Terms1D, ReproducesChapter5Equation565ByAddingTerms) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{5},
        fvgrid::Length{5.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};

    const DiffusionTerm1D diffusion{1.0};
    const SourceTerm1D source_term{function_source_1d(grid, source)};
    const BoundaryTerm1D boundary{
        BoundarySet1D{
            BoundaryCondition1D::dirichlet(1.0),
            BoundaryCondition1D::neumann(0.0)
        },
        1.0
    };

    const EquationContribution1D contribution =
        assemble(diffusion, grid)
      + assemble(source_term, grid)
      + assemble(boundary, grid);

    constexpr std::array<Real, 5> aw_ref{0.0, 1.0, 1.0, 1.0, 1.0};
    constexpr std::array<Real, 5> ap_ref{3.0, 2.0, 2.0, 2.0, 1.0};
    constexpr std::array<Real, 5> ae_ref{1.0, 1.0, 1.0, 1.0, 0.0};
    constexpr std::array<Real, 5> bp_ref{2.25, 2.25, 6.25, 12.25, 20.25};

    for (Size i = 0; i < grid.num_volumes(); ++i) {
        EXPECT_DOUBLE_EQ(contribution.aw()[i], aw_ref[i]);
        EXPECT_DOUBLE_EQ(contribution.ap()[i], ap_ref[i]);
        EXPECT_DOUBLE_EQ(contribution.ae()[i], ae_ref[i]);
        EXPECT_DOUBLE_EQ(contribution.bp()[i], bp_ref[i]);
    }
}

TEST(Terms1D, AssemblesInternalUpwindAdvectionFaces) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{3},
        fvgrid::Length{3.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};

    const AdvectionTerm1D advection{
        vector_face_flux_1d(
            grid,
            DenseVector{std::vector<Real>{0.0, 2.0, -3.0, 0.0}}
        )
    };

    const EquationContribution1D contribution = assemble(advection, grid);

    EXPECT_DOUBLE_EQ(contribution.ap()[0], 2.0);
    EXPECT_DOUBLE_EQ(contribution.ae()[0], 0.0);

    EXPECT_DOUBLE_EQ(contribution.aw()[1], 2.0);
    EXPECT_DOUBLE_EQ(contribution.ae()[1], 3.0);

    EXPECT_DOUBLE_EQ(contribution.ap()[2], 3.0);
    EXPECT_DOUBLE_EQ(contribution.aw()[2], 0.0);
}

}  // namespace fvm

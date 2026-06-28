// ============================================================================
// Arquivo: tst_Laplacian1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Laplacian 1 D no contexto de OneDimensional / Operator.
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

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/OneDimensional/Assembly/Assembler1D.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

#include <gtest/gtest.h>

namespace fvm {

TEST(Laplacian1D, StoresClassIdentity) {
    EXPECT_EQ(Laplacian1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(Laplacian1D::id().class_name(), std::string_view{"Laplacian1D"});
    EXPECT_EQ(
        Laplacian1D::id().class_id(),
        std::string_view{"fvm.1d.operator.Laplacian1D"}
    );
}

TEST(Laplacian1D, StoresConstantCoefficient) {
    const Laplacian1D laplacian{2.5};

    EXPECT_DOUBLE_EQ(laplacian.coefficient(), 2.5);
    EXPECT_FALSE(laplacian.has_variable_coefficient());
}

TEST(Laplacian1D, StoresVariableCoefficientOnFaces) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{3},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};
    const Laplacian1D laplacian{
        function_coefficient_1d(grid, [](Real x) { return Real{1.0} + x; })
    };

    EXPECT_TRUE(laplacian.has_variable_coefficient());
    EXPECT_DOUBLE_EQ(laplacian.face_coefficient(grid, 0), 1.0);
    EXPECT_DOUBLE_EQ(laplacian.face_coefficient(grid, 1), 4.0 / 3.0);
    EXPECT_DOUBLE_EQ(laplacian.face_coefficient(grid, 2), 5.0 / 3.0);
    EXPECT_DOUBLE_EQ(laplacian.face_coefficient(grid, 3), 2.0);
}

TEST(Laplacian1D, AssemblesUniformDirichletCoefficients) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{5},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};
    const BoundarySet1D boundaries{dirichlet_1d(0.0), dirichlet_1d(0.0)};

    const TridiagonalSystem1D system = assemble_laplacian_1d(
        grid,
        Laplacian1D{},
        boundaries
    );

    ASSERT_EQ(system.size(), static_cast<Size>(5));
    EXPECT_DOUBLE_EQ(system.diagonal()[0], -75.0);
    EXPECT_DOUBLE_EQ(system.upper()[0], 25.0);

    EXPECT_DOUBLE_EQ(system.lower()[0], 25.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[1], -50.0);
    EXPECT_DOUBLE_EQ(system.upper()[1], 25.0);

    EXPECT_DOUBLE_EQ(system.lower()[3], 25.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[4], -75.0);
}

TEST(Laplacian1D, DirichletBoundaryValuesModifyRightHandSide) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{2},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};
    const BoundarySet1D boundaries{dirichlet_1d(1.0), dirichlet_1d(2.0)};

    const TridiagonalSystem1D system = assemble_laplacian_1d(
        grid,
        Laplacian1D{},
        boundaries
    );

    EXPECT_DOUBLE_EQ(system.diagonal()[0], -12.0);
    EXPECT_DOUBLE_EQ(system.rhs()[0], -8.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[1], -12.0);
    EXPECT_DOUBLE_EQ(system.rhs()[1], -16.0);
}

TEST(Laplacian1D, AssemblesVariableCoefficientOnFaces) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{3},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const GridView1D grid{axis};
    const BoundarySet1D boundaries{dirichlet_1d(0.0), dirichlet_1d(0.0)};
    const Laplacian1D laplacian{
        function_coefficient_1d(grid, [](Real x) { return Real{1.0} + x; })
    };

    const TridiagonalSystem1D system = assemble_laplacian_1d(
        grid,
        laplacian,
        boundaries
    );

    EXPECT_DOUBLE_EQ(system.diagonal()[0], -30.0);
    EXPECT_DOUBLE_EQ(system.upper()[0], 12.0);

    EXPECT_DOUBLE_EQ(system.lower()[0], 12.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[1], -27.0);
    EXPECT_DOUBLE_EQ(system.upper()[1], 15.0);

    EXPECT_DOUBLE_EQ(system.lower()[1], 15.0);
    EXPECT_DOUBLE_EQ(system.diagonal()[2], -51.0);
}

}  // namespace fvm

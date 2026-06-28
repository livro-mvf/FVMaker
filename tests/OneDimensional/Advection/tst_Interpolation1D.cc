// ============================================================================
// Arquivo: tst_Interpolation1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Interpolation 1 D no contexto de OneDimensional / Advection.
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

#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Advection/Interpolation1D.h>

#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] GridView1D make_grid() {
    static const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{3},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    return GridView1D{axis};
}

}  // namespace

TEST(Interpolation1D, ComputesCentralAndUpwindInternalFaceValues) {
    EXPECT_DOUBLE_EQ(
        interpolate_internal_face_value_1d(CentralDifference1D{}, 1.0, 3.0, 2.0),
        2.0
    );
    EXPECT_DOUBLE_EQ(
        interpolate_internal_face_value_1d(Upwind1D{}, 1.0, 3.0, 2.0),
        1.0
    );
    EXPECT_DOUBLE_EQ(
        interpolate_internal_face_value_1d(Upwind1D{}, 1.0, 3.0, -2.0),
        3.0
    );
}

TEST(Interpolation1D, ProvidesFakePowerLawExponentialAndQuickValues) {
    EXPECT_DOUBLE_EQ(
        interpolate_internal_face_value_1d(PowerLaw1D{}, 1.0, 3.0, 0.5),
        2.0
    );
    EXPECT_DOUBLE_EQ(
        interpolate_internal_face_value_1d(Exponential1D{}, 1.0, 3.0, 2.0),
        1.0
    );
    EXPECT_DOUBLE_EQ(
        interpolate_internal_face_value_1d(QUICK1D{}, 1.0, 3.0, -2.0),
        3.0
    );
}

TEST(Interpolation1D, InterpolatesAllFaces) {
    const GridView1D grid = make_grid();
    const DenseVector phi{std::vector<Real>{1.0, 3.0, 5.0}};
    const FaceFlux1D flux = uniform_face_flux_1d(grid, 1.0);

    const DenseVector faces = interpolate_faces_1d(
        grid,
        phi,
        flux,
        CentralDifference1D{}
    );

    ASSERT_EQ(faces.size(), grid.num_faces());
    EXPECT_DOUBLE_EQ(faces[0], 1.0);
    EXPECT_DOUBLE_EQ(faces[1], 2.0);
    EXPECT_DOUBLE_EQ(faces[2], 4.0);
    EXPECT_DOUBLE_EQ(faces[3], 5.0);
}

TEST(Interpolation1D, RejectsInvalidCellValueSize) {
    const GridView1D grid = make_grid();
    const FaceFlux1D flux = uniform_face_flux_1d(grid, 1.0);

    EXPECT_THROW(
        static_cast<void>(interpolate_faces_1d(
            grid,
            DenseVector{std::vector<Real>{1.0, 2.0}},
            flux,
            Upwind1D{}
        )),
        FVMException
    );
}

}  // namespace fvm

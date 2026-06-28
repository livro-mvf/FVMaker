// ============================================================================
// Arquivo: tst_FaceFlux1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Face Flux 1 D no contexto de OneDimensional / Advection.
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
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Advection/FaceFlux1D.h>

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

TEST(FaceFlux1D, StoresClassIdentity) {
    EXPECT_EQ(FaceFlux1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(FaceFlux1D::id().class_name(), std::string_view{"FaceFlux1D"});
    EXPECT_EQ(
        FaceFlux1D::id().class_id(),
        std::string_view{"fvm.1d.advection.FaceFlux1D"}
    );
}

TEST(FaceFlux1D, BuildsUniformFluxOnFaces) {
    const GridView1D grid = make_grid();
    const FaceFlux1D flux = uniform_face_flux_1d(grid, 2.0);

    ASSERT_EQ(flux.size(), grid.num_faces());
    EXPECT_DOUBLE_EQ(flux.value(0), 2.0);
    EXPECT_DOUBLE_EQ(flux.value(3), 2.0);
}

TEST(FaceFlux1D, RejectsWrongVectorSize) {
    const GridView1D grid = make_grid();

    EXPECT_THROW(
        static_cast<void>(vector_face_flux_1d(
            grid,
            DenseVector{std::vector<Real>{1.0, 2.0}}
        )),
        FVMException
    );
}

TEST(FaceFlux1D, ComputesFacePecletNumber) {
    EXPECT_DOUBLE_EQ(face_peclet_1d(4.0, 2.0), 2.0);
    EXPECT_THROW(static_cast<void>(face_peclet_1d(4.0, 0.0)), FVMException);
}

}  // namespace fvm

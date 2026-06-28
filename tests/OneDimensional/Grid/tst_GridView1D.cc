// ============================================================================
// Arquivo: tst_GridView1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Grid View 1 D no contexto de OneDimensional / Grid.
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
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Axis1D/Axis1D.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(GridView1D, StoresClassIdentity) {
    EXPECT_EQ(GridView1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(GridView1D::id().class_name(), std::string_view{"GridView1D"});
    EXPECT_EQ(GridView1D::id().class_id(), std::string_view{"fvm.1d.grid.GridView1D"});

    EXPECT_EQ(GridView1D::class_name(), std::string_view{"GridView1D"});
    EXPECT_EQ(GridView1D::class_id(), std::string_view{"fvm.1d.grid.GridView1D"});
}

TEST(GridView1D, ReadsUniformAxisGeneratedByFVGridMaker) {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{4},
        fvgrid::Length{2.0},
        fvgrid::XInit{-1.0}
    );

    const GridView1D grid{axis};

    EXPECT_EQ(grid.num_volumes(), static_cast<Size>(4));
    EXPECT_EQ(grid.num_cells(), static_cast<Size>(4));
    EXPECT_EQ(grid.num_faces(), static_cast<Size>(5));
    EXPECT_DOUBLE_EQ(grid.xmin(), -1.0);
    EXPECT_DOUBLE_EQ(grid.xmax(), 1.0);
    EXPECT_DOUBLE_EQ(grid.length(), 2.0);

    ASSERT_EQ(grid.faces().size(), static_cast<Size>(5));
    EXPECT_DOUBLE_EQ(grid.faces()[0], -1.0);
    EXPECT_DOUBLE_EQ(grid.faces()[1], -0.5);
    EXPECT_DOUBLE_EQ(grid.faces()[2], 0.0);
    EXPECT_DOUBLE_EQ(grid.faces()[3], 0.5);
    EXPECT_DOUBLE_EQ(grid.faces()[4], 1.0);

    ASSERT_EQ(grid.centers().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(grid.centers()[0], -0.75);
    EXPECT_DOUBLE_EQ(grid.centers()[1], -0.25);
    EXPECT_DOUBLE_EQ(grid.centers()[2], 0.25);
    EXPECT_DOUBLE_EQ(grid.centers()[3], 0.75);
}

TEST(GridView1D, ReadsNonUniformAxisGeneratedByFVGridMaker) {
    const fvgrid::Axis1D axis{{0.0, 0.2, 0.7, 1.0}};
    const GridView1D grid{axis};

    EXPECT_EQ(grid.num_volumes(), static_cast<Size>(3));
    EXPECT_EQ(grid.num_faces(), static_cast<Size>(4));

    ASSERT_EQ(grid.cell_lengths().size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(grid.cell_lengths()[0], 0.2);
    EXPECT_DOUBLE_EQ(grid.cell_lengths()[1], 0.5);
    EXPECT_DOUBLE_EQ(grid.cell_lengths()[2], 0.3);

    ASSERT_EQ(grid.dx_centers().size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(grid.dx_centers()[0], 0.1);
    EXPECT_DOUBLE_EQ(grid.dx_centers()[1], 0.35);
    EXPECT_DOUBLE_EQ(grid.dx_centers()[2], 0.4);
    EXPECT_DOUBLE_EQ(grid.dx_centers()[3], 0.15);
}

TEST(GridView1D, MetricsBundleKeepsReadOnlyGridDataTogether) {
    const fvgrid::Axis1D axis{{0.0, 0.25, 1.0}};
    const GridView1D grid{axis};

    const GridMetrics1D metrics = grid.metrics();

    EXPECT_EQ(metrics.num_volumes, grid.num_volumes());
    EXPECT_EQ(metrics.num_faces, grid.num_faces());
    EXPECT_EQ(metrics.faces.data(), grid.faces().data());
    EXPECT_EQ(metrics.centers.data(), grid.centers().data());
    EXPECT_EQ(metrics.dx_faces.data(), grid.dx_faces().data());
    EXPECT_EQ(metrics.dx_centers.data(), grid.dx_centers().data());
    EXPECT_EQ(metrics.cell_lengths.data(), grid.cell_lengths().data());
    EXPECT_DOUBLE_EQ(metrics.xmin, grid.xmin());
    EXPECT_DOUBLE_EQ(metrics.xmax, grid.xmax());
    EXPECT_DOUBLE_EQ(metrics.length, grid.length());
    EXPECT_EQ(metrics.pattern_name, grid.pattern_name());
}

TEST(GridView1D, RejectsMissingMeshPointer) {
    try {
        const GridView1D grid{static_cast<const fvgrid::Axis1D*>(nullptr)};
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kMissingMesh.code);
        EXPECT_EQ(exception.record().category, std::string_view{"Mesh"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"GridView1D"});
        EXPECT_EQ(
            exception.record().source.class_id(),
            std::string_view{"fvm.1d.grid.GridView1D"}
        );
        return;
    }

    FAIL() << "GridView1D did not reject a missing FVGridMaker axis.";
}

}  // namespace fvm

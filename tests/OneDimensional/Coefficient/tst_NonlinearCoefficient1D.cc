// ============================================================================
// Arquivo: tst_NonlinearCoefficient1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Nonlinear Coefficient 1 D no contexto de OneDimensional / Coefficient.
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

#include <limits>
#include <string_view>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Coefficient/NonlinearCoefficient1D.h>

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

TEST(NonlinearCoefficient1D, StoresClassIdentity) {
    EXPECT_EQ(
        NonlinearCoefficient1D::id().module(),
        std::string_view{"OneDimensional"}
    );
    EXPECT_EQ(
        NonlinearCoefficient1D::id().class_name(),
        std::string_view{"NonlinearCoefficient1D"}
    );
    EXPECT_EQ(
        NonlinearCoefficient1D::id().class_id(),
        std::string_view{"fvm.1d.coefficient.NonlinearCoefficient1D"}
    );
}

TEST(NonlinearCoefficient1D, EvaluatesCellValuesFromPhiAndCenters) {
    const GridView1D grid = make_grid();
    const DenseVector phi{std::vector<Real>{1.0, 2.0, 3.0}};
    const NonlinearCoefficient1D coefficient = nonlinear_coefficient_1d(
        [](Real phi_value, Real x) {
            return Real{1.0} + phi_value * phi_value + x;
        }
    );

    const DenseVector values = coefficient.cell_values(grid, phi);

    ASSERT_EQ(values.size(), grid.num_volumes());
    EXPECT_DOUBLE_EQ(values[0], 1.0 + 1.0 + 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(values[1], 1.0 + 4.0 + 3.0 / 6.0);
    EXPECT_DOUBLE_EQ(values[2], 1.0 + 9.0 + 5.0 / 6.0);
}

TEST(NonlinearCoefficient1D, BuildsArithmeticFaceValues) {
    const GridView1D grid = make_grid();
    const DenseVector phi{std::vector<Real>{1.0, 3.0, 5.0}};
    const NonlinearCoefficient1D coefficient = nonlinear_coefficient_1d(
        [](Real phi_value, Real) {
            return phi_value;
        }
    );

    const DiffusionCoefficient1D face_values =
        coefficient.arithmetic_face_values(grid, phi);

    EXPECT_DOUBLE_EQ(face_values.face_value(0), 1.0);
    EXPECT_DOUBLE_EQ(face_values.face_value(1), 2.0);
    EXPECT_DOUBLE_EQ(face_values.face_value(2), 4.0);
    EXPECT_DOUBLE_EQ(face_values.face_value(3), 5.0);
}

TEST(NonlinearCoefficient1D, BuildsHarmonicFaceValues) {
    const GridView1D grid = make_grid();
    const DenseVector phi{std::vector<Real>{1.0, 3.0, 6.0}};
    const NonlinearCoefficient1D coefficient = nonlinear_coefficient_1d(
        [](Real phi_value, Real) {
            return phi_value;
        }
    );

    const DiffusionCoefficient1D face_values =
        coefficient.harmonic_face_values(grid, phi);

    EXPECT_DOUBLE_EQ(face_values.face_value(0), 1.0);
    EXPECT_DOUBLE_EQ(face_values.face_value(1), 1.5);
    EXPECT_DOUBLE_EQ(face_values.face_value(2), 4.0);
    EXPECT_DOUBLE_EQ(face_values.face_value(3), 6.0);
}

TEST(NonlinearCoefficient1D, RejectsInvalidPhiSize) {
    const GridView1D grid = make_grid();
    const DenseVector phi{std::vector<Real>{1.0, 2.0}};
    const NonlinearCoefficient1D coefficient = nonlinear_coefficient_1d(
        [](Real phi_value, Real) {
            return phi_value;
        }
    );

    EXPECT_THROW(static_cast<void>(coefficient.cell_values(grid, phi)), FVMException);
}

TEST(NonlinearCoefficient1D, RejectsNonFiniteCoefficientValue) {
    const GridView1D grid = make_grid();
    const DenseVector phi{std::vector<Real>{1.0, 2.0, 3.0}};
    const NonlinearCoefficient1D coefficient = nonlinear_coefficient_1d(
        [](Real, Real) {
            return std::numeric_limits<Real>::quiet_NaN();
        }
    );

    try {
        static_cast<void>(coefficient.cell_values(grid, phi));
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidCoefficient.code);
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"NonlinearCoefficient1D"}
        );
        return;
    }

    FAIL() << "NonlinearCoefficient1D did not reject a non-finite value.";
}

}  // namespace fvm

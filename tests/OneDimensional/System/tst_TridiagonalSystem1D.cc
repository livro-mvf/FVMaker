// ============================================================================
// Arquivo: tst_TridiagonalSystem1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Tridiagonal System 1 D no contexto de OneDimensional / System.
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

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/System/TridiagonalSystem1D.h>

#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] TridiagonalSystem1D make_system() {
    return TridiagonalSystem1D{
        std::vector<Real>{4.0, 5.0},
        std::vector<Real>{1.0, 2.0, 3.0},
        std::vector<Real>{6.0, 7.0},
        DenseVector{std::vector<Real>{0.0, 0.0, 0.0}}
    };
}

}  // namespace

TEST(TridiagonalSystem1D, MultipliesMatrixByVectorWithOperator) {
    const TridiagonalSystem1D system = make_system();
    const DenseVector x{std::vector<Real>{10.0, 20.0, 30.0}};

    const DenseVector y = system * x;

    ASSERT_EQ(y.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(y[0], 130.0);
    EXPECT_DOUBLE_EQ(y[1], 290.0);
    EXPECT_DOUBLE_EQ(y[2], 190.0);
}

TEST(TridiagonalSystem1D, MultipliesMatrixByVectorIntoOutput) {
    const TridiagonalSystem1D system = make_system();
    const DenseVector x{std::vector<Real>{10.0, 20.0, 30.0}};
    DenseVector y{3};

    multiply(system, x, y);

    EXPECT_DOUBLE_EQ(y[0], 130.0);
    EXPECT_DOUBLE_EQ(y[1], 290.0);
    EXPECT_DOUBLE_EQ(y[2], 190.0);
}

TEST(TridiagonalSystem1D, MultipliesTransposeByVector) {
    const TridiagonalSystem1D system = make_system();
    const DenseVector x{std::vector<Real>{10.0, 20.0, 30.0}};

    const DenseVector y = multiply_transpose(system, x);

    ASSERT_EQ(y.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(y[0], 90.0);
    EXPECT_DOUBLE_EQ(y[1], 250.0);
    EXPECT_DOUBLE_EQ(y[2], 230.0);
}

TEST(TridiagonalSystem1D, MultipliesTransposeByVectorIntoOutput) {
    const TridiagonalSystem1D system = make_system();
    const DenseVector x{std::vector<Real>{10.0, 20.0, 30.0}};
    DenseVector y{3};

    multiply_transpose(system, x, y);

    EXPECT_DOUBLE_EQ(y[0], 90.0);
    EXPECT_DOUBLE_EQ(y[1], 250.0);
    EXPECT_DOUBLE_EQ(y[2], 230.0);
}

TEST(TridiagonalSystem1D, RejectsVectorWithWrongSize) {
    const TridiagonalSystem1D system = make_system();
    const DenseVector x{std::vector<Real>{1.0, 2.0}};

    try {
        static_cast<void>(system * x);
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidSystemSize.code);
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"TridiagonalSystem1D"}
        );
        return;
    }

    FAIL() << "Tridiagonal product did not reject incompatible vector size.";
}

TEST(TridiagonalSystem1D, RejectsInPlaceMatrixVectorProduct) {
    const TridiagonalSystem1D system = make_system();
    DenseVector x{std::vector<Real>{10.0, 20.0, 30.0}};

    try {
        multiply(system, x, x);
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidArgument.code);
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"TridiagonalSystem1D"}
        );
        return;
    }

    FAIL() << "Tridiagonal product allowed the same input and output vector.";
}

}  // namespace fvm

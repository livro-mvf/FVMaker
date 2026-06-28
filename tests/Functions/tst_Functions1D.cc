// ============================================================================
// Arquivo: tst_Functions1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Functions 1 D no contexto de Functions.
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

#include <functional>

// ----------------------------------------------------------------------------
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Functions/FunctionEvaluation1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] Real free_scalar_1d(Real x) {
    return x + 1.0;
}

[[nodiscard]] VectorValue1D free_vector_1d(Real x) {
    return VectorValue1D{x};
}

struct ScalarFunctor1D final {
    [[nodiscard]] Real operator()(Real x) const {
        return x * x;
    }
};

struct VectorFunctor1D final {
    [[nodiscard]] VectorValue1D operator()(Real x) const {
        return VectorValue1D{2.0 * x};
    }
};

[[nodiscard]] GridView1D make_grid() {
    static const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{4},
        fvgrid::Length{2.0},
        fvgrid::XInit{0.0}
    );

    return GridView1D{axis};
}

}  // namespace

TEST(Functions1D, AcceptsFreeFunctionLambdaFunctorAndStdFunction) {
    static_assert(ScalarFunction1D<decltype(free_scalar_1d)>);
    static_assert(ScalarFunction1D<ScalarFunctor1D>);
    static_assert(ScalarFunction1D<std::function<Real(Real)>>);

    const auto lambda = [](Real x) {
        return x - 1.0;
    };

    static_assert(ScalarFunction1D<decltype(lambda)>);

    EXPECT_DOUBLE_EQ(free_scalar_1d(2.0), 3.0);
    EXPECT_DOUBLE_EQ(lambda(2.0), 1.0);
    EXPECT_DOUBLE_EQ(ScalarFunctor1D{}(2.0), 4.0);
}

TEST(Functions1D, AcceptsTimeDependentScalarFunctions) {
    const auto function = [](Real x, Real time) {
        return x + time;
    };

    static_assert(ScalarFunction1DTime<decltype(function)>);
    EXPECT_DOUBLE_EQ(function(2.0, 3.0), 5.0);

    const StoredScalarFunction1DTime stored = function;
    EXPECT_DOUBLE_EQ(stored(4.0, 5.0), 9.0);
}

TEST(Functions1D, AcceptsVectorFunctions) {
    static_assert(VectorFunction1D<decltype(free_vector_1d)>);
    static_assert(VectorFunction1D<VectorFunctor1D>);
    static_assert(VectorFunction1D<std::function<VectorValue1D(Real)>>);

    const auto lambda = [](Real x) {
        return VectorValue1D{x + 1.0};
    };

    static_assert(VectorFunction1D<decltype(lambda)>);

    EXPECT_DOUBLE_EQ(free_vector_1d(2.0)[0], 2.0);
    EXPECT_DOUBLE_EQ(lambda(2.0)[0], 3.0);
    EXPECT_DOUBLE_EQ(VectorFunctor1D{}(2.0)[0], 4.0);
}

TEST(Functions1D, EvaluatesScalarFunctionAtGridCenters) {
    const DenseVector values = evaluate_at_centers(
        make_grid(),
        [](Real x) {
            return x * x;
        }
    );

    ASSERT_EQ(values.size(), static_cast<Size>(4));
    EXPECT_DOUBLE_EQ(values[0], 0.25 * 0.25);
    EXPECT_DOUBLE_EQ(values[1], 0.75 * 0.75);
    EXPECT_DOUBLE_EQ(values[2], 1.25 * 1.25);
    EXPECT_DOUBLE_EQ(values[3], 1.75 * 1.75);
}

TEST(Functions1D, EvaluatesScalarFunctionAtGridFaces) {
    const DenseVector values = evaluate_at_faces(
        make_grid(),
        [](Real x) {
            return x + 10.0;
        }
    );

    ASSERT_EQ(values.size(), static_cast<Size>(5));
    EXPECT_DOUBLE_EQ(values[0], 10.0);
    EXPECT_DOUBLE_EQ(values[1], 10.5);
    EXPECT_DOUBLE_EQ(values[2], 11.0);
    EXPECT_DOUBLE_EQ(values[3], 11.5);
    EXPECT_DOUBLE_EQ(values[4], 12.0);
}

}  // namespace fvm

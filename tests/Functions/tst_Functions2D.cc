// ============================================================================
// Arquivo: tst_Functions2D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Functions 2 D no contexto de Functions.
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
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Functions/FunctionTypes.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] Real free_scalar_2d(Real x, Real y) {
    return x + y;
}

[[nodiscard]] VectorValue2D free_vector_2d(Real x, Real y) {
    return VectorValue2D{x, y};
}

struct ScalarFunctor2D final {
    [[nodiscard]] Real operator()(Real x, Real y) const {
        return x * y;
    }
};

struct VectorFunctor2D final {
    [[nodiscard]] VectorValue2D operator()(Real x, Real y) const {
        return VectorValue2D{x + 1.0, y + 2.0};
    }
};

}  // namespace

TEST(Functions2D, AcceptsScalarFunctions) {
    static_assert(ScalarFunction2D<decltype(free_scalar_2d)>);
    static_assert(ScalarFunction2D<ScalarFunctor2D>);
    static_assert(ScalarFunction2D<std::function<Real(Real, Real)>>);

    const auto lambda = [](Real x, Real y) {
        return x - y;
    };

    static_assert(ScalarFunction2D<decltype(lambda)>);

    EXPECT_DOUBLE_EQ(free_scalar_2d(2.0, 3.0), 5.0);
    EXPECT_DOUBLE_EQ(lambda(2.0, 3.0), -1.0);
    EXPECT_DOUBLE_EQ(ScalarFunctor2D{}(2.0, 3.0), 6.0);
}

TEST(Functions2D, AcceptsTimeDependentScalarFunctions) {
    const auto function = [](Real x, Real y, Real time) {
        return x + y + time;
    };

    static_assert(ScalarFunction2DTime<decltype(function)>);
    EXPECT_DOUBLE_EQ(function(1.0, 2.0, 3.0), 6.0);

    const StoredScalarFunction2DTime stored = function;
    EXPECT_DOUBLE_EQ(stored(4.0, 5.0, 6.0), 15.0);
}

TEST(Functions2D, AcceptsVectorFunctions) {
    static_assert(VectorFunction2D<decltype(free_vector_2d)>);
    static_assert(VectorFunction2D<VectorFunctor2D>);
    static_assert(VectorFunction2D<std::function<VectorValue2D(Real, Real)>>);

    const auto lambda = [](Real x, Real y) {
        return VectorValue2D{x - y, x + y};
    };

    static_assert(VectorFunction2D<decltype(lambda)>);

    EXPECT_DOUBLE_EQ(free_vector_2d(2.0, 3.0)[0], 2.0);
    EXPECT_DOUBLE_EQ(free_vector_2d(2.0, 3.0)[1], 3.0);
    EXPECT_DOUBLE_EQ(lambda(2.0, 3.0)[0], -1.0);
    EXPECT_DOUBLE_EQ(lambda(2.0, 3.0)[1], 5.0);
    EXPECT_DOUBLE_EQ(VectorFunctor2D{}(2.0, 3.0)[0], 3.0);
    EXPECT_DOUBLE_EQ(VectorFunctor2D{}(2.0, 3.0)[1], 5.0);
}

}  // namespace fvm

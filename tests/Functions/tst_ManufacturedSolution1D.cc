// ============================================================================
// Arquivo: tst_ManufacturedSolution1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Manufactured Solution 1 D no contexto de Functions.
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

#include <FVMaker/Functions/ManufacturedSolution1D.h>
#include <FVMaker/Functions/ManufacturedProblem1D.h>
#include <FVMaker/Functions/VariableCoefficient1D.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(ManufacturedSolution1D, StoresSolutionAndDerivatives) {
    const ManufacturedSolution1D solution{
        .phi =
            [](Real x) {
                return x * x;
            },
        .dphi_dx =
            [](Real x) {
                return 2.0 * x;
            },
        .d2phi_dx2 =
            [](Real) {
                return 2.0;
            },
    };

    EXPECT_DOUBLE_EQ(solution.value(3.0), 9.0);
    EXPECT_DOUBLE_EQ(solution.first_derivative(3.0), 6.0);
    EXPECT_DOUBLE_EQ(solution.second_derivative(3.0), 2.0);
}

TEST(VariableCoefficient1D, StoresCoefficientAndDerivative) {
    const VariableCoefficient1D coefficient{
        .gamma =
            [](Real x) {
                return 1.0 + x;
            },
        .dgamma_dx =
            [](Real) {
                return 1.0;
            },
    };

    EXPECT_DOUBLE_EQ(coefficient.value(2.0), 3.0);
    EXPECT_DOUBLE_EQ(coefficient.derivative(2.0), 1.0);
}

TEST(ManufacturedSolution1D, BuildsVariableCoefficientDiffusionSource) {
    const ManufacturedSolution1D solution{
        .phi =
            [](Real x) {
                return x * x;
            },
        .dphi_dx =
            [](Real x) {
                return 2.0 * x;
            },
        .d2phi_dx2 =
            [](Real) {
                return 2.0;
            },
    };

    const VariableCoefficient1D coefficient{
        .gamma =
            [](Real x) {
                return 1.0 + x;
            },
        .dgamma_dx =
            [](Real) {
                return 1.0;
            },
    };

    const StoredScalarFunction1D source = [solution, coefficient](Real x) {
        return coefficient.value(x) * solution.second_derivative(x) +
               coefficient.derivative(x) * solution.first_derivative(x);
    };

    EXPECT_DOUBLE_EQ(source(2.0), 10.0);
}

TEST(ManufacturedProblem1D, StoresMultipleNamedCoefficients) {
    ManufacturedProblem1D problem{
        ManufacturedSolution1D{
            .phi =
                [](Real x) {
                    return x * x;
                },
            .dphi_dx =
                [](Real x) {
                    return 2.0 * x;
                },
            .d2phi_dx2 =
                [](Real) {
                    return 2.0;
                },
        }
    };

    problem.add_coefficient(
        "gamma",
        VariableCoefficient1D{
            .gamma =
                [](Real x) {
                    return 1.0 + x;
                },
            .dgamma_dx =
                [](Real) {
                    return 1.0;
                },
        }
    );

    problem.add_coefficient(
        "sigma",
        VariableCoefficient1D{
            .gamma =
                [](Real x) {
                    return 2.0 + x * x;
                },
            .dgamma_dx =
                [](Real x) {
                    return 2.0 * x;
                },
        }
    );

    EXPECT_EQ(problem.coefficient_count(), static_cast<Size>(2));
    EXPECT_DOUBLE_EQ(problem.coefficient("gamma").value(2.0), 3.0);
    EXPECT_DOUBLE_EQ(problem.coefficient("sigma").value(2.0), 6.0);
    EXPECT_DOUBLE_EQ(problem.coefficient("sigma").derivative(2.0), 4.0);
}

}  // namespace fvm

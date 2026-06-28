// ============================================================================
// Arquivo: ex_ManufacturedFunctions1D.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa ex Manufactured Functions 1 D no contexto de projeto.
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

#include <iostream>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Functions/ManufacturedProblem1D.h>
#include <FVMaker/Functions/ManufacturedSolution1D.h>
#include <FVMaker/Functions/VariableCoefficient1D.h>

int main() {
    const fvm::ManufacturedSolution1D solution{
        .phi =
            [](fvm::Real x) {
                return x * x;
            },
        .dphi_dx =
            [](fvm::Real x) {
                return 2.0 * x;
            },
        .d2phi_dx2 =
            [](fvm::Real) {
                return 2.0;
            },
    };

    const fvm::VariableCoefficient1D coefficient{
        .gamma =
            [](fvm::Real x) {
                return 1.0 + x;
            },
        .dgamma_dx =
            [](fvm::Real) {
                return 1.0;
            },
    };

    fvm::ManufacturedProblem1D problem{solution};
    problem.add_coefficient("gamma", coefficient);
    problem.add_coefficient(
        "sigma",
        fvm::VariableCoefficient1D{
            .gamma =
                [](fvm::Real x) {
                    return 2.0 + x * x;
                },
            .dgamma_dx =
                [](fvm::Real x) {
                    return 2.0 * x;
                },
        }
    );

    const fvm::StoredScalarFunction1D source = [solution, coefficient](fvm::Real x) {
        return coefficient.value(x) * solution.second_derivative(x) +
               coefficient.derivative(x) * solution.first_derivative(x);
    };

    const fvm::Real x = 0.5;

    std::cout << "Manufactured functions 1D\n";
    std::cout << "x          : " << x << '\n';
    std::cout << "phi        : " << solution.value(x) << '\n';
    std::cout << "dphi_dx    : " << solution.first_derivative(x) << '\n';
    std::cout << "d2phi_dx2  : " << solution.second_derivative(x) << '\n';
    std::cout << "gamma      : " << coefficient.value(x) << '\n';
    std::cout << "dgamma_dx  : " << coefficient.derivative(x) << '\n';
    std::cout << "sigma      : " << problem.coefficient("sigma").value(x) << '\n';
    std::cout << "source     : " << source(x) << '\n';

    return 0;
}

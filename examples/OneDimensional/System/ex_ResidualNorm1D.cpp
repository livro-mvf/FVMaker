// ============================================================================
// Arquivo: ex_ResidualNorm1D.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa ex Residual Norm 1 D no contexto de projeto.
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
#include <vector>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/System/AlgebraicResidual1D.h>

int main() {
    const fvm::TridiagonalSystem1D system{
        std::vector<fvm::Real>{-1.0, -1.0},
        std::vector<fvm::Real>{2.0, 2.0, 2.0},
        std::vector<fvm::Real>{-1.0, -1.0},
        fvm::DenseVector{std::vector<fvm::Real>{1.0, 0.0, 1.0}}
    };

    const fvm::DenseVector solution{
        std::vector<fvm::Real>{1.0, 0.5, 1.0}
    };

    const fvm::DenseVector residual =
        fvm::algebraic_residual(system, solution);

    std::cout << "Residual r = A X - B\n";
    std::cout << "||r||inf = " << fvm::norm_infinity(residual) << '\n';
    std::cout << "||r||1   = " << fvm::norm_l1(residual) << '\n';
    std::cout << "||r||2   = " << fvm::norm_l2(residual) << '\n';

    return 0;
}

// ============================================================================
// Arquivo: ex_Source1D.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa ex Source 1 D no contexto de projeto.
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

#include <iomanip>
#include <iostream>
#include <vector>

#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>

int main() {
    using fvm::DenseVector;
    using fvm::LinearizedSource1D;
    using fvm::Real;

    const LinearizedSource1D source = fvm::vector_source_1d(
        DenseVector{std::vector<Real>{1.0, 2.0, 3.0}},
        DenseVector{std::vector<Real>{0.5, -1.0, 2.0}}
    );
    const DenseVector phi{std::vector<Real>{2.0, 3.0, 4.0}};
    const DenseVector b = source.evaluate(phi);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Linearized source 1D\n";
    std::cout << "====================\n";
    std::cout << "B_P = S_C + S_P phi_P\n\n";

    for (fvm::Size i = 0; i < source.size(); ++i) {
        std::cout << "i = " << i
                  << ", S_C = " << source.source_constant()[i]
                  << ", S_P = " << source.source_linear()[i]
                  << ", phi_P = " << phi[i]
                  << ", B_P = " << b[i]
                  << '\n';
    }

    return b[0] == Real{2.0} && b[1] == Real{-1.0} && b[2] == Real{11.0}
        ? 0
        : 1;
}

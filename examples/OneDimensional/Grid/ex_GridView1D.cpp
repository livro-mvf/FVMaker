// ============================================================================
// Arquivo: ex_GridView1D.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa ex Grid View 1 D no contexto de projeto.
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
// FVGridMaker includes
// ----------------------------------------------------------------------------
#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

int main() {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{5},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );

    const fvm::GridView1D grid{axis};
    const fvm::GridMetrics1D metrics = grid.metrics();

    std::cout << "FVM GridView1D over FVGridMaker Axis1D\n";
    std::cout << "volumes : " << metrics.num_volumes << '\n';
    std::cout << "faces   : " << metrics.num_faces << '\n';
    std::cout << "xmin    : " << metrics.xmin << '\n';
    std::cout << "xmax    : " << metrics.xmax << '\n';
    std::cout << "length  : " << metrics.length << '\n';
    std::cout << "dx[0]   : " << metrics.cell_lengths.front() << '\n';

    return 0;
}

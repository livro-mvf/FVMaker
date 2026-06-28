// ============================================================================
// Arquivo: ex_Advection1D.cpp
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa ex Advection 1 D no contexto de projeto.
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

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/OneDimensional/Advection/Advection1D.h>

namespace {

void print_faces(
    const char* name,
    const fvm::GridView1D& grid,
    const fvm::DenseVector& values
) {
    std::cout << name << '\n';

    for (fvm::Size face = 0; face < grid.num_faces(); ++face) {
        std::cout << "  face " << std::setw(2) << face
                  << " x = " << std::setw(12) << grid.faces()[face]
                  << " phi_f = " << std::setw(12) << values[face] << '\n';
    }
}

}  // namespace

int main() {
    const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{4},
        fvgrid::Length{1.0},
        fvgrid::XInit{0.0}
    );
    const fvm::GridView1D grid{axis};

    const fvm::DenseVector phi{std::vector<fvm::Real>{1.0, 2.0, 4.0, 8.0}};
    const fvm::Advection1D advection{fvm::uniform_face_flux_1d(grid, 1.0)};

    const fvm::DenseVector central = fvm::advective_face_values_1d(
        grid,
        phi,
        advection,
        fvm::CentralDifference1D{}
    );
    const fvm::DenseVector upwind = fvm::advective_face_values_1d(
        grid,
        phi,
        advection,
        fvm::Upwind1D{}
    );
    const fvm::DenseVector power_law = fvm::advective_face_values_1d(
        grid,
        phi,
        advection,
        fvm::PowerLaw1D{}
    );
    const fvm::DenseVector exponential = fvm::advective_face_values_1d(
        grid,
        phi,
        advection,
        fvm::Exponential1D{}
    );
    const fvm::DenseVector quick = fvm::advective_face_values_1d(
        grid,
        phi,
        advection,
        fvm::QUICK1D{}
    );

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Preliminary 1D advection interpolation example\n";
    std::cout << "==============================================\n";
    std::cout << "Status: provisional fake example for face-value plumbing only\n\n";

    print_faces("CentralDifference1D", grid, central);
    print_faces("Upwind1D", grid, upwind);
    print_faces("PowerLaw1D fake", grid, power_law);
    print_faces("Exponential1D fake", grid, exponential);
    print_faces("QUICK1D fake", grid, quick);

    return 0;
}

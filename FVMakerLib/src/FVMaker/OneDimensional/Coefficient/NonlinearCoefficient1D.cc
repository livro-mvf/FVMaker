// ============================================================================
// Arquivo: NonlinearCoefficient1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Nonlinear Coefficient 1 D no contexto de OneDimensional / Coefficient.
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

#include <cmath>
#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Coefficient/NonlinearCoefficient1D.h>

namespace fvm {

NonlinearCoefficient1D::NonlinearCoefficient1D(Function function)
    : function_(std::move(function)) {
    require(
        static_cast<bool>(function_),
        error_catalog::kInvalidCoefficient,
        NonlinearCoefficient1D::id()
    );
}

DenseVector NonlinearCoefficient1D::cell_values(
    const GridView1D& grid,
    const DenseVector& phi
) const {
    require(
        phi.size() == grid.num_volumes(),
        error_catalog::kInvalidCoefficient,
        NonlinearCoefficient1D::id()
    );

    DenseVector values{grid.num_volumes()};

    for (Size cell = 0; cell < grid.num_volumes(); ++cell) {
        values[cell] = function_(phi[cell], grid.centers()[cell]);

        require(
            std::isfinite(values[cell]),
            error_catalog::kInvalidCoefficient,
            NonlinearCoefficient1D::id()
        );
    }

    return values;
}

DiffusionCoefficient1D NonlinearCoefficient1D::arithmetic_face_values(
    const GridView1D& grid,
    const DenseVector& phi
) const {
    return arithmetic_field_coefficient_1d(grid, cell_values(grid, phi));
}

DiffusionCoefficient1D NonlinearCoefficient1D::harmonic_face_values(
    const GridView1D& grid,
    const DenseVector& phi
) const {
    return harmonic_field_coefficient_1d(grid, cell_values(grid, phi));
}

}  // namespace fvm

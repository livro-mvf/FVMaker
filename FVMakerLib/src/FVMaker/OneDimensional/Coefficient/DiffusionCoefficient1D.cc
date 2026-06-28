// ============================================================================
// Arquivo: DiffusionCoefficient1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Diffusion Coefficient 1 D no contexto de OneDimensional / Coefficient.
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
#include <FVMaker/OneDimensional/Coefficient/DiffusionCoefficient1D.h>

namespace fvm {

DiffusionCoefficient1D::DiffusionCoefficient1D(DenseVector face_values)
    : face_values_(std::move(face_values)) {
    validate();
}

Size DiffusionCoefficient1D::num_faces() const noexcept {
    return face_values_.size();
}

std::span<const Real> DiffusionCoefficient1D::face_values() const noexcept {
    return face_values_.values();
}

Real DiffusionCoefficient1D::face_value(Size face) const noexcept {
    return face_values_[face];
}

void DiffusionCoefficient1D::validate() const {
    require(
        !face_values_.empty(),
        error_catalog::kInvalidCoefficient,
        DiffusionCoefficient1D::id()
    );

    for (const Real value : face_values_.values()) {
        require(
            std::isfinite(value),
            error_catalog::kInvalidCoefficient,
            DiffusionCoefficient1D::id()
        );
    }
}

DiffusionCoefficient1D uniform_coefficient_1d(
    const GridView1D& grid,
    Real coefficient
) {
    return DiffusionCoefficient1D{DenseVector{grid.num_faces(), coefficient}};
}

DiffusionCoefficient1D face_coefficient_1d(
    const GridView1D& grid,
    DenseVector face_values
) {
    require(
        face_values.size() == grid.num_faces(),
        error_catalog::kInvalidCoefficient,
        DiffusionCoefficient1D::id()
    );

    return DiffusionCoefficient1D{std::move(face_values)};
}

DiffusionCoefficient1D arithmetic_field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
) {
    return interpolated_field_coefficient_1d(
        grid, cell_values, ArithmeticFaceInterpolation1D{}
    );
}

DiffusionCoefficient1D harmonic_field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
) {
    for (const Real value : cell_values.values()) {
        require(
            value > Real{},
            error_catalog::kInvalidCoefficient,
            DiffusionCoefficient1D::id()
        );
    }
    return interpolated_field_coefficient_1d(
        grid, cell_values, HarmonicFaceInterpolation1D{}
    );
}

DiffusionCoefficient1D field_coefficient_1d(
    const GridView1D& grid,
    const DenseVector& cell_values
) {
    return arithmetic_field_coefficient_1d(grid, cell_values);
}

}  // namespace fvm

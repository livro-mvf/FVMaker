// ============================================================================
// Arquivo: Laplacian1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Laplacian 1 D no contexto de OneDimensional / Operator.
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
#include <vector>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Operator/Laplacian1D.h>

namespace fvm {

Laplacian1D::Laplacian1D(Real coefficient)
    : coefficient_(coefficient) {
    validate();
}

Laplacian1D::Laplacian1D(DiffusionCoefficient1D coefficient)
    : coefficient_(Real{1.0}),
      face_coefficients_(std::vector<Real>{
          coefficient.face_values().begin(),
          coefficient.face_values().end()
      }) {
    validate();
}

Real Laplacian1D::coefficient() const noexcept {
    return coefficient_;
}

bool Laplacian1D::has_variable_coefficient() const noexcept {
    return !face_coefficients_.empty();
}

Real Laplacian1D::face_coefficient(const GridView1D& grid, Size face) const {
    if (!has_variable_coefficient()) {
        return coefficient_;
    }

    require(
        face_coefficients_.size() == grid.num_faces(),
        error_catalog::kInvalidCoefficient,
        Laplacian1D::id()
    );

    require(
        face < face_coefficients_.size(),
        error_catalog::kOutOfRange,
        Laplacian1D::id()
    );

    return face_coefficients_[face];
}

void Laplacian1D::validate() const {
    require(
        std::isfinite(coefficient_),
        error_catalog::kInvalidCoefficient,
        Laplacian1D::id()
    );

    for (const Real value : face_coefficients_.values()) {
        require(
            std::isfinite(value),
            error_catalog::kInvalidCoefficient,
            Laplacian1D::id()
        );
    }
}

}  // namespace fvm

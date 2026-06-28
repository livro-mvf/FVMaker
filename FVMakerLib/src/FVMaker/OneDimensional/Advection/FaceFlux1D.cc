// ============================================================================
// Arquivo: FaceFlux1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Face Flux 1 D no contexto de OneDimensional / Advection.
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
#include <FVMaker/OneDimensional/Advection/FaceFlux1D.h>

namespace fvm {

FaceFlux1D::FaceFlux1D(DenseVector values) : values_(std::move(values)) {
    validate();
}

Size FaceFlux1D::size() const noexcept {
    return values_.size();
}

std::span<const Real> FaceFlux1D::values() const noexcept {
    return values_.values();
}

Real FaceFlux1D::value(Size face) const noexcept {
    return values_[face];
}

void FaceFlux1D::validate() const {
    require(!values_.empty(), error_catalog::kInvalidArgument, FaceFlux1D::id());

    for (const Real value : values_.values()) {
        require(
            std::isfinite(value),
            error_catalog::kInvalidArgument,
            FaceFlux1D::id()
        );
    }
}

FaceFlux1D uniform_face_flux_1d(const GridView1D& grid, Real value) {
    return FaceFlux1D{DenseVector{grid.num_faces(), value}};
}

FaceFlux1D vector_face_flux_1d(
    const GridView1D& grid,
    DenseVector values
) {
    require(
        values.size() == grid.num_faces(),
        error_catalog::kInvalidArgument,
        FaceFlux1D::id()
    );

    return FaceFlux1D{std::move(values)};
}

Real face_peclet_1d(Real mass_flux, Real diffusion) {
    require(
        std::isfinite(mass_flux) && std::isfinite(diffusion)
            && std::abs(diffusion) > Real{},
        error_catalog::kInvalidArgument,
        FaceFlux1D::id()
    );

    return mass_flux / diffusion;
}

}  // namespace fvm

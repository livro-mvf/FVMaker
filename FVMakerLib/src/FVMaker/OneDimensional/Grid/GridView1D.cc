// ============================================================================
// Arquivo: GridView1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Grid View 1 D no contexto de OneDimensional / Grid.
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

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>

namespace fvm {

GridView1D::GridView1D(const fvgrid::Axis1D& axis) : axis_(&axis) {
    validate();
}

GridView1D::GridView1D(const fvgrid::Axis1D* axis) : axis_(axis) {
    validate();
}

Size GridView1D::num_volumes() const noexcept {
    return static_cast<Size>(axis_->num_cells());
}

Size GridView1D::num_cells() const noexcept {
    return num_volumes();
}

Size GridView1D::num_faces() const noexcept {
    return static_cast<Size>(axis_->num_faces());
}

std::span<const Real> GridView1D::faces() const noexcept {
    return axis_->faces();
}

std::span<const Real> GridView1D::centers() const noexcept {
    return axis_->centers();
}

std::span<const Real> GridView1D::dx_faces() const noexcept {
    return axis_->dx_faces();
}

std::span<const Real> GridView1D::dx_centers() const noexcept {
    return axis_->dx_centers();
}

std::span<const Real> GridView1D::cell_lengths() const noexcept {
    return axis_->cell_lengths();
}

Real GridView1D::xmin() const noexcept {
    return axis_->xmin();
}

Real GridView1D::xmax() const noexcept {
    return axis_->xmax();
}

Real GridView1D::length() const noexcept {
    return axis_->length();
}

std::string_view GridView1D::pattern_name() const noexcept {
    return axis_->pattern_name();
}

GridMetrics1D GridView1D::metrics() const noexcept {
    return GridMetrics1D{
        .num_volumes = num_volumes(),
        .num_faces = num_faces(),
        .faces = faces(),
        .centers = centers(),
        .dx_faces = dx_faces(),
        .dx_centers = dx_centers(),
        .cell_lengths = cell_lengths(),
        .xmin = xmin(),
        .xmax = xmax(),
        .length = length(),
        .pattern_name = pattern_name(),
    };
}

void GridView1D::validate() const {
    require(axis_ != nullptr, error_catalog::kMissingMesh, GridView1D::id());

    require(
        axis_->num_cells() > 0,
        error_catalog::kInvalidMeshDimension,
        GridView1D::id()
    );

    require(
        axis_->num_faces() == axis_->num_cells() + 1,
        error_catalog::kIncompatibleMesh,
        GridView1D::id()
    );

    require(
        axis_->faces().size() == axis_->num_faces(),
        error_catalog::kIncompatibleMesh,
        GridView1D::id()
    );

    require(
        axis_->centers().size() == axis_->num_cells(),
        error_catalog::kIncompatibleMesh,
        GridView1D::id()
    );

    require(
        axis_->dx_faces().size() == axis_->num_cells(),
        error_catalog::kIncompatibleMesh,
        GridView1D::id()
    );

    require(
        axis_->dx_centers().size() == axis_->num_faces(),
        error_catalog::kIncompatibleMesh,
        GridView1D::id()
    );
}

}  // namespace fvm

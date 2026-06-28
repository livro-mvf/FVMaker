// ============================================================================
// Arquivo: GhostCells1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Ghost Cells 1 D no contexto de OneDimensional / Boundary.
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
#include <FVMaker/OneDimensional/Boundary/GhostCells1D.h>

namespace fvm {

GhostCells1D::GhostCells1D(Size left, Size right)
    : left_(left), right_(right) {
    validate();
}

Size GhostCells1D::left() const noexcept {
    return left_;
}

Size GhostCells1D::right() const noexcept {
    return right_;
}

Size GhostCells1D::total() const noexcept {
    return left_ + right_;
}

bool GhostCells1D::empty() const noexcept {
    return total() == 0;
}

Size GhostCells1D::first_physical_index() const noexcept {
    return left_;
}

Size GhostCells1D::storage_size(Size physical_size) const noexcept {
    return physical_size + total();
}

Size GhostCells1D::storage_index(Size physical_index) const noexcept {
    return left_ + physical_index;
}

void GhostCells1D::validate() const {
    require(
        left_ <= max_per_side(),
        error_catalog::kInvalidBoundaryCondition,
        GhostCells1D::id()
    );

    require(
        right_ <= max_per_side(),
        error_catalog::kInvalidBoundaryCondition,
        GhostCells1D::id()
    );
}

}  // namespace fvm

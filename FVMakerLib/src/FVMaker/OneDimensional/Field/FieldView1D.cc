// ============================================================================
// Arquivo: FieldView1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Field View 1 D no contexto de OneDimensional / Field.
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
#include <FVMaker/OneDimensional/Field/FieldView1D.h>

namespace fvm {

FieldView1D::FieldView1D(
    GridView1D grid,
    std::string_view name,
    std::span<Real> values
)
    : grid_(grid),
      name_(name),
      values_(values) {
    validate();
}

const GridView1D& FieldView1D::grid() const noexcept {
    return grid_;
}

std::string_view FieldView1D::name() const noexcept {
    return name_;
}

Size FieldView1D::size() const noexcept {
    return values_.size();
}

bool FieldView1D::empty() const noexcept {
    return values_.empty();
}

std::span<Real> FieldView1D::values() noexcept {
    return values_;
}

std::span<const Real> FieldView1D::values() const noexcept {
    return values_;
}

Real& FieldView1D::operator[](Size index) noexcept {
    return values_[index];
}

const Real& FieldView1D::operator[](Size index) const noexcept {
    return values_[index];
}

void FieldView1D::validate() const {
    require(
        values_.size() == grid_.num_volumes(),
        error_catalog::kInvalidFieldSize,
        FieldView1D::id()
    );
}

}  // namespace fvm

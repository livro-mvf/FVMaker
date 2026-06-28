// ============================================================================
// Arquivo: Field1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Field 1 D no contexto de OneDimensional / Field.
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

#include <algorithm>
#include <utility>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Field/Field1D.h>

namespace fvm {

Field1D::Field1D(GridView1D grid, std::string name)
    : Field1D(grid, std::move(name), Real{}, false) {}

Field1D::Field1D(
    GridView1D grid,
    std::string name,
    Real uniform_value,
    bool keep_history
)
    : grid_(grid),
      name_(std::move(name)),
      values_(grid_.num_volumes(), uniform_value),
      keep_history_(keep_history) {
    validate_size();
    record_initial_state();
}

Field1D::Field1D(
    GridView1D grid,
    std::string name,
    std::vector<Real> values,
    bool keep_history
)
    : grid_(grid),
      name_(std::move(name)),
      values_(std::move(values)),
      keep_history_(keep_history) {
    validate_size();
    record_initial_state();
}

const GridView1D& Field1D::grid() const noexcept {
    return grid_;
}

std::string_view Field1D::name() const noexcept {
    return name_;
}

void Field1D::rename(std::string name) {
    name_ = std::move(name);
}

Size Field1D::size() const noexcept {
    return values_.size();
}

bool Field1D::empty() const noexcept {
    return values_.empty();
}

std::span<Real> Field1D::values() noexcept {
    return values_;
}

std::span<const Real> Field1D::values() const noexcept {
    return values_;
}

Real& Field1D::operator[](Size index) noexcept {
    return values_[index];
}

const Real& Field1D::operator[](Size index) const noexcept {
    return values_[index];
}

void Field1D::fill(Real value) {
    std::ranges::fill(values_, value);
}

void Field1D::assign_values(std::vector<Real> values) {
    values_ = std::move(values);
    validate_size();
}

FieldView1D Field1D::view() noexcept {
    return FieldView1D{grid_, name_, values_};
}

bool Field1D::history_enabled() const noexcept {
    return keep_history_;
}

Size Field1D::history_size() const noexcept {
    return values_.empty() ? Size{} : history_.size() / values_.size();
}

std::span<const Real> Field1D::history_step(Size index) const {
    require(
        index < history_size(),
        error_catalog::kOutOfRange,
        Field1D::id()
    );

    const Size offset = index * values_.size();
    return std::span<const Real>{history_.data() + offset, values_.size()};
}

void Field1D::save_state() {
    if (keep_history_) {
        history_.insert(history_.end(), values_.begin(), values_.end());
    }
}

void Field1D::clear_history() {
    history_.clear();
}

void Field1D::validate_size() const {
    require(
        values_.size() == grid_.num_volumes(),
        error_catalog::kInvalidFieldSize,
        Field1D::id()
    );
}

void Field1D::record_initial_state() {
    if (keep_history_) {
        history_.insert(history_.end(), values_.begin(), values_.end());
    }
}

}  // namespace fvm

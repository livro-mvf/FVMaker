// ============================================================================
// Arquivo: Variable1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Variable 1 D no contexto de OneDimensional / Variable.
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
#include <iomanip>
#include <ostream>
#include <string>
#include <utility>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/OneDimensional/Variable/Variable1D.h>

namespace fvm {

Variable1D::Variable1D(GridView1D grid, std::string name)
    : Variable1D(std::move(grid), std::move(name), Real{}) {}

Variable1D::Variable1D(
    GridView1D grid,
    std::string name,
    Real initial_value
)
    : grid_(std::move(grid)),
      name_(std::move(name)),
      phi_(grid_.num_volumes(), initial_value),
      phi_ant_(grid_.num_volumes(), initial_value),
      phi_iter_(grid_.num_volumes(), initial_value),
      phi_analit_(grid_.num_volumes(), Real{}) {}

Variable1D::Variable1D(
    GridView1D grid,
    std::string name,
    std::vector<Real> phi
)
    : grid_(std::move(grid)),
      name_(std::move(name)),
      phi_(std::move(phi)),
      phi_ant_(grid_.num_volumes()),
      phi_iter_(grid_.num_volumes()),
      phi_analit_(grid_.num_volumes()) {
    validate_size(phi_);
    phi_ant_ = phi_;
    phi_iter_ = phi_;
}

const GridView1D& Variable1D::grid() const noexcept {
    return grid_;
}

std::string_view Variable1D::name() const noexcept {
    return name_;
}

void Variable1D::rename(std::string name) {
    name_ = std::move(name);
}

Size Variable1D::size() const noexcept {
    return phi_.size();
}

bool Variable1D::empty() const noexcept {
    return phi_.empty();
}

DenseVector& Variable1D::phi() noexcept {
    return phi_;
}

const DenseVector& Variable1D::phi() const noexcept {
    return phi_;
}

DenseVector& Variable1D::phi_ant() noexcept {
    return phi_ant_;
}

const DenseVector& Variable1D::phi_ant() const noexcept {
    return phi_ant_;
}

DenseVector& Variable1D::phi_iter() noexcept {
    return phi_iter_;
}

const DenseVector& Variable1D::phi_iter() const noexcept {
    return phi_iter_;
}

DenseVector& Variable1D::phi_analit() noexcept {
    return phi_analit_;
}

const DenseVector& Variable1D::phi_analit() const noexcept {
    return phi_analit_;
}

void Variable1D::fill(Real value) {
    std::ranges::fill(phi_.values(), value);
    std::ranges::fill(phi_ant_.values(), value);
    std::ranges::fill(phi_iter_.values(), value);
}

void Variable1D::assign_phi(DenseVector values) {
    validate_size(values);
    phi_ = std::move(values);
}

void Variable1D::assign_phi(std::vector<Real> values) {
    assign_phi(DenseVector{std::move(values)});
}

void Variable1D::update_phi(DenseVector values) {
    validate_size(values);
    copy_phi_to_previous();
    phi_ = std::move(values);
}

void Variable1D::update_phi(const SolveResult& result) {
    update_phi(result.solution);
}

void Variable1D::save_iteration(DenseVector values) {
    validate_size(values);
    phi_iter_ = std::move(values);
}

void Variable1D::advance_iteration_to_phi() {
    copy_phi_to_previous();
    phi_ = phi_iter_;
}

DenseVector Variable1D::analytic_error() const {
    return difference_to_analytic();
}

DenseVector Variable1D::difference_to_previous() const {
    return difference_to(phi_ant_);
}

DenseVector Variable1D::difference_to_iteration() const {
    return difference_to(phi_iter_);
}

DenseVector Variable1D::difference_to_analytic() const {
    return difference_to(phi_analit_);
}

VariableDifferenceNorms1D Variable1D::norms_to_previous() const {
    return norms_of(difference_to_previous());
}

VariableDifferenceNorms1D Variable1D::norms_to_iteration() const {
    return norms_of(difference_to_iteration());
}

VariableDifferenceNorms1D Variable1D::norms_to_analytic() const {
    return norms_of(difference_to_analytic());
}

void Variable1D::validate_size(const DenseVector& values) const {
    require(
        values.size() == grid_.num_volumes(),
        error_catalog::kInvalidFieldSize,
        id()
    );
}

void Variable1D::copy_phi_to_previous() {
    phi_ant_ = phi_;
}

DenseVector Variable1D::difference_to(const DenseVector& reference) const {
    validate_size(reference);

    DenseVector difference{size()};

    for (Size i = 0; i < size(); ++i) {
        difference[i] = phi_[i] - reference[i];
    }

    return difference;
}

VariableDifferenceNorms1D Variable1D::norms_of(
    const DenseVector& difference
) {
    return VariableDifferenceNorms1D{
        .infinity = VectorNorms::evaluate(difference, NormType::infinity),
        .l1 = VectorNorms::evaluate(difference, NormType::l1),
        .l2 = VectorNorms::evaluate(difference, NormType::l2),
    };
}

std::ostream& operator<<(std::ostream& os, const Variable1D& variable) {
    os << "indice"
       << std::setw(16) << "x_C"
       << std::setw(16) << "phi"
       << std::setw(16) << "phi_ant"
       << std::setw(16) << "phi_iter"
       << std::setw(16) << "phi_analit"
       << '\n';

    os << std::string(86, '-') << '\n';

    const auto centers = variable.grid().centers();

    for (Size i = 0; i < variable.size(); ++i) {
        os << std::setw(6) << i
           << std::setw(16) << centers[i]
           << std::setw(16) << variable.phi()[i]
           << std::setw(16) << variable.phi_ant()[i]
           << std::setw(16) << variable.phi_iter()[i]
           << std::setw(16) << variable.phi_analit()[i];

        if (i + 1 < variable.size()) {
            os << '\n';
        }
    }

    return os;
}

}  // namespace fvm

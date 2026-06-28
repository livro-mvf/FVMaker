// ============================================================================
// Arquivo: DenseVector.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Dense Vector no contexto de Algebra.
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

#include <utility>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>

namespace fvm {

namespace {

void require_same_size(const DenseVector& left, const DenseVector& right) {
    require(
        left.size() == right.size(),
        error_catalog::kInvalidSystemSize,
        DenseVector::id()
    );
}

}  // namespace

DenseVector::DenseVector(Size size) : values_(size) {}

DenseVector::DenseVector(Size size, Real value) : values_(size, value) {}

DenseVector::DenseVector(std::vector<Real> values) : values_(std::move(values)) {}

Size DenseVector::size() const noexcept {
    return values_.size();
}

bool DenseVector::empty() const noexcept {
    return values_.empty();
}

std::span<Real> DenseVector::values() noexcept {
    return values_;
}

std::span<const Real> DenseVector::values() const noexcept {
    return values_;
}

Real& DenseVector::operator[](Size index) noexcept {
    return values_[index];
}

const Real& DenseVector::operator[](Size index) const noexcept {
    return values_[index];
}

DenseVector& operator+=(DenseVector& left, const DenseVector& right) {
    require_same_size(left, right);

    for (Size i = 0; i < left.size(); ++i) {
        left[i] += right[i];
    }

    return left;
}

DenseVector& operator-=(DenseVector& left, const DenseVector& right) {
    require_same_size(left, right);

    for (Size i = 0; i < left.size(); ++i) {
        left[i] -= right[i];
    }

    return left;
}

DenseVector& operator*=(DenseVector& vector, Real scalar) {
    for (Real& value : vector.values()) {
        value *= scalar;
    }

    return vector;
}

DenseVector operator+(const DenseVector& left, const DenseVector& right) {
    DenseVector result = left;
    result += right;
    return result;
}

DenseVector operator-(const DenseVector& left, const DenseVector& right) {
    DenseVector result = left;
    result -= right;
    return result;
}

DenseVector operator-(const DenseVector& vector) {
    DenseVector result = vector;
    result *= Real{-1};
    return result;
}

DenseVector operator*(Real scalar, const DenseVector& vector) {
    DenseVector result = vector;
    result *= scalar;
    return result;
}

DenseVector operator*(const DenseVector& vector, Real scalar) {
    return scalar * vector;
}

}  // namespace fvm

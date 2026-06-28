// ============================================================================
// Arquivo: LinearizedSource1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Linearized Source 1 D no contexto de OneDimensional / Source.
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
#include <vector>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/ThrowError.h>
#include <FVMaker/OneDimensional/Source/LinearizedSource1D.h>

namespace fvm {

LinearizedSource1D::LinearizedSource1D(Size size)
    : source_constant_(size), source_linear_(size) {
    validate();
}

LinearizedSource1D::LinearizedSource1D(
    DenseVector source_constant,
    DenseVector source_linear
)
    : source_constant_(std::move(source_constant)),
      source_linear_(std::move(source_linear)) {
    validate();
}

Size LinearizedSource1D::size() const noexcept {
    return source_constant_.size();
}

DenseVector& LinearizedSource1D::source_constant() noexcept {
    return source_constant_;
}

const DenseVector& LinearizedSource1D::source_constant() const noexcept {
    return source_constant_;
}

DenseVector& LinearizedSource1D::source_linear() noexcept {
    return source_linear_;
}

const DenseVector& LinearizedSource1D::source_linear() const noexcept {
    return source_linear_;
}

DenseVector LinearizedSource1D::evaluate(const DenseVector& phi) const {
    require(
        phi.size() == size(),
        error_catalog::kInvalidFieldSize,
        LinearizedSource1D::id()
    );

    DenseVector result{size()};

    for (Size i = 0; i < size(); ++i) {
        result[i] = source_constant_[i] + source_linear_[i] * phi[i];
    }

    return result;
}

void LinearizedSource1D::validate() const {
    require(
        !source_constant_.empty(),
        error_catalog::kInvalidSource,
        LinearizedSource1D::id()
    );

    require(
        source_linear_.size() == source_constant_.size(),
        error_catalog::kInvalidSource,
        LinearizedSource1D::id()
    );
}

LinearizedSource1D zero_source_1d(Size size) {
    return LinearizedSource1D{size};
}

LinearizedSource1D uniform_source_1d(
    Size size,
    Real source_constant,
    Real source_linear
) {
    return LinearizedSource1D{
        DenseVector{size, source_constant},
        DenseVector{size, source_linear}
    };
}

LinearizedSource1D vector_source_1d(
    DenseVector source_constant,
    DenseVector source_linear
) {
    return LinearizedSource1D{
        std::move(source_constant),
        std::move(source_linear)
    };
}

LinearizedSource1D explicit_vector_source_1d(DenseVector source_constant) {
    const Size size = source_constant.size();

    return LinearizedSource1D{
        std::move(source_constant),
        DenseVector{size}
    };
}

void apply_source_to_system(
    TridiagonalSystem1D& system,
    const LinearizedSource1D& source
) {
    require(
        system.size() == source.size(),
        error_catalog::kInvalidSource,
        LinearizedSource1D::id()
    );

    for (Size i = 0; i < system.size(); ++i) {
        system.rhs()[i] += source.source_constant()[i];
        system.diagonal()[i] -= source.source_linear()[i];
    }
}

}  // namespace fvm

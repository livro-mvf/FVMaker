// ============================================================================
// Arquivo: ErrorNorms.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Implementa Error Norms no contexto de Algebra.
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
#include <cmath>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/ErrorNorms.h>

namespace fvm {

Real VectorNorms::evaluate(const DenseVector& vector, NormType type) noexcept {
    switch (type) {
        case NormType::infinity:
            return infinity(vector);
        case NormType::l1:
            return l1(vector);
        case NormType::l2:
            return l2(vector);
        case NormType::rms:
            return rms(vector);
    }

    return infinity(vector);
}

Real VectorNorms::infinity(const DenseVector& vector) noexcept {
    Real norm{};

    for (Real value : vector.values()) {
        norm = std::max(norm, std::abs(value));
    }

    return norm;
}

Real VectorNorms::l1(const DenseVector& vector) noexcept {
    Real norm{};

    for (Real value : vector.values()) {
        norm += std::abs(value);
    }

    return norm;
}

Real VectorNorms::l2(const DenseVector& vector) noexcept {
    Real sum{};

    for (Real value : vector.values()) {
        sum += value * value;
    }

    return std::sqrt(sum);
}

Real VectorNorms::rms(const DenseVector& vector) noexcept {
    if (vector.size() == 0) {
        return Real{};
    }
    return l2(vector) / std::sqrt(static_cast<Real>(vector.size()));
}

Real norm_infinity(const DenseVector& vector) noexcept {
    return VectorNorms::infinity(vector);
}

Real norm_l1(const DenseVector& vector) noexcept {
    return VectorNorms::l1(vector);
}

Real norm_l2(const DenseVector& vector) noexcept {
    return VectorNorms::l2(vector);
}

Real norm_rms(const DenseVector& vector) noexcept {
    return VectorNorms::rms(vector);
}

}  // namespace fvm

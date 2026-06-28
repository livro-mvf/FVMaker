// ============================================================================
// Arquivo: DenseVector.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Dense Vector no contexto de Algebra.
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

#pragma once

// ----------------------------------------------------------------------------
// C++ standard library includes
// ----------------------------------------------------------------------------
#include <span>
#include <string_view>
#include <vector>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

class DenseVector final {
public:
    DenseVector() = default;
    explicit DenseVector(Size size);
    DenseVector(Size size, Real value);
    explicit DenseVector(std::vector<Real> values);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"Algebra", "DenseVector", "fvm.algebra.DenseVector"};
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] Size size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    [[nodiscard]] std::span<Real> values() noexcept;
    [[nodiscard]] std::span<const Real> values() const noexcept;

    [[nodiscard]] Real& operator[](Size index) noexcept;
    [[nodiscard]] const Real& operator[](Size index) const noexcept;

private:
    std::vector<Real> values_;
};

DenseVector& operator+=(DenseVector& left, const DenseVector& right);
DenseVector& operator-=(DenseVector& left, const DenseVector& right);
DenseVector& operator*=(DenseVector& vector, Real scalar);

[[nodiscard]] DenseVector operator+(const DenseVector& left, const DenseVector& right);
[[nodiscard]] DenseVector operator-(const DenseVector& left, const DenseVector& right);
[[nodiscard]] DenseVector operator-(const DenseVector& vector);
[[nodiscard]] DenseVector operator*(Real scalar, const DenseVector& vector);
[[nodiscard]] DenseVector operator*(const DenseVector& vector, Real scalar);

}  // namespace fvm

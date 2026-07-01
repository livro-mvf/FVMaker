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

// Representa um vetor denso de valores reais usado nos sistemas discretos.
class DenseVector final {
public:
    // Cria um objeto DenseVector com os dados fornecidos.
    DenseVector() = default;
    // Cria um objeto DenseVector com os dados fornecidos.
    explicit DenseVector(Size size);
    // Cria um objeto DenseVector com os dados fornecidos.
    DenseVector(Size size, Real value);
    // Cria um objeto DenseVector com os dados fornecidos.
    explicit DenseVector(std::vector<Real> values);

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"Algebra", "DenseVector", "fvm.algebra.DenseVector"};
    }

    // Retorna o nome curto da classe para diagnostico e documentacao.
    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    // Retorna o identificador completo da classe na hierarquia da biblioteca.
    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    // Retorna a informacao size associada ao objeto.
    [[nodiscard]] Size size() const noexcept;
    // Retorna a informacao empty associada ao objeto.
    [[nodiscard]] bool empty() const noexcept;

    // Retorna a informacao values armazenada no objeto.
    [[nodiscard]] std::span<Real> values() noexcept;
    // Retorna a informacao values armazenada no objeto.
    [[nodiscard]] std::span<const Real> values() const noexcept;

    // Define o comportamento do operador usado por esta abstracao.
    [[nodiscard]] Real& operator[](Size index) noexcept;
    // Define o comportamento do operador usado por esta abstracao.
    [[nodiscard]] const Real& operator[](Size index) const noexcept;

private:
    std::vector<Real> values_;
};

DenseVector& operator+=(DenseVector& left, const DenseVector& right);
DenseVector& operator-=(DenseVector& left, const DenseVector& right);
DenseVector& operator*=(DenseVector& vector, Real scalar);

// Define o comportamento do operador usado por esta abstracao.
[[nodiscard]] DenseVector operator+(const DenseVector& left, const DenseVector& right);
// Define o comportamento do operador usado por esta abstracao.
[[nodiscard]] DenseVector operator-(const DenseVector& left, const DenseVector& right);
// Define o comportamento do operador usado por esta abstracao.
[[nodiscard]] DenseVector operator-(const DenseVector& vector);
// Define o comportamento do operador usado por esta abstracao.
[[nodiscard]] DenseVector operator*(Real scalar, const DenseVector& vector);
// Define o comportamento do operador usado por esta abstracao.
[[nodiscard]] DenseVector operator*(const DenseVector& vector, Real scalar);

}  // namespace fvm

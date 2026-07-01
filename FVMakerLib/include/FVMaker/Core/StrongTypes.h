// ============================================================================
// Arquivo: StrongTypes.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Strong Types no contexto de Core.
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
#include <concepts>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/Types.h>

namespace fvm {

// Define um real fortemente tipado para evitar mistura de grandezas.
template <std::floating_point T, class Tag>
class StrongReal final {
public:
    using value_type = T;
    using tag_type = Tag;

    // Cria um objeto StrongReal com os dados fornecidos.
    constexpr explicit StrongReal(T value) noexcept : value_(value) {}

    // Retorna a informacao value armazenada no objeto.
    [[nodiscard]] constexpr T value() const noexcept {
        return value_;
    }

private:
    T value_;
};

// Define um inteiro fortemente tipado para evitar mistura de indices.
template <std::integral T, class Tag>
class StrongInteger final {
public:
    using value_type = T;
    using tag_type = Tag;

    // Cria um objeto StrongInteger com os dados fornecidos.
    constexpr explicit StrongInteger(T value) noexcept : value_(value) {}

    // Retorna a informacao value armazenada no objeto.
    [[nodiscard]] constexpr T value() const noexcept {
        return value_;
    }

private:
    T value_;
};

// Representa o conceito de tolerance tag dentro da biblioteca FVMaker.
struct ToleranceTag final {};
// Representa o conceito de max iterations tag dentro da biblioteca FVMaker.
struct MaxIterationsTag final {};
// Representa o conceito de time tag dentro da biblioteca FVMaker.
struct TimeTag final {};
// Representa o conceito de time step tag dentro da biblioteca FVMaker.
struct TimeStepTag final {};
// Representa o conceito de initial time tag dentro da biblioteca FVMaker.
struct InitialTimeTag final {};
// Representa o conceito de final time tag dentro da biblioteca FVMaker.
struct FinalTimeTag final {};
// Representa o conceito de relaxation factor tag dentro da biblioteca FVMaker.
struct RelaxationFactorTag final {};

using Tolerance = StrongReal<Real, ToleranceTag>;
using MaxIterations = StrongInteger<Size, MaxIterationsTag>;
using Time = StrongReal<Real, TimeTag>;
using TimeStep = StrongReal<Real, TimeStepTag>;
using InitialTime = StrongReal<Real, InitialTimeTag>;
using FinalTime = StrongReal<Real, FinalTimeTag>;
using RelaxationFactor = StrongReal<Real, RelaxationFactorTag>;

}  // namespace fvm

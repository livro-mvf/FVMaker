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

template <std::floating_point T, class Tag>
class StrongReal final {
public:
    using value_type = T;
    using tag_type = Tag;

    constexpr explicit StrongReal(T value) noexcept : value_(value) {}

    [[nodiscard]] constexpr T value() const noexcept {
        return value_;
    }

private:
    T value_;
};

template <std::integral T, class Tag>
class StrongInteger final {
public:
    using value_type = T;
    using tag_type = Tag;

    constexpr explicit StrongInteger(T value) noexcept : value_(value) {}

    [[nodiscard]] constexpr T value() const noexcept {
        return value_;
    }

private:
    T value_;
};

struct ToleranceTag final {};
struct MaxIterationsTag final {};
struct TimeTag final {};
struct TimeStepTag final {};
struct InitialTimeTag final {};
struct FinalTimeTag final {};
struct RelaxationFactorTag final {};

using Tolerance = StrongReal<Real, ToleranceTag>;
using MaxIterations = StrongInteger<Size, MaxIterationsTag>;
using Time = StrongReal<Real, TimeTag>;
using TimeStep = StrongReal<Real, TimeStepTag>;
using InitialTime = StrongReal<Real, InitialTimeTag>;
using FinalTime = StrongReal<Real, FinalTimeTag>;
using RelaxationFactor = StrongReal<Real, RelaxationFactorTag>;

}  // namespace fvm

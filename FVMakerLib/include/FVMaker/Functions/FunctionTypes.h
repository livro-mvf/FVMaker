// ============================================================================
// Arquivo: FunctionTypes.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Function Types no contexto de Functions.
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
#include <array>
#include <concepts>
#include <functional>

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Core/Types.h>

namespace fvm {

using VectorValue1D = std::array<Real, 1>;
using VectorValue2D = std::array<Real, 2>;

template <class Function>
concept ScalarFunction1D =
    std::invocable<Function, Real> &&
    std::convertible_to<std::invoke_result_t<Function, Real>, Real>;

template <class Function>
concept ScalarFunction1DTime =
    std::invocable<Function, Real, Real> &&
    std::convertible_to<std::invoke_result_t<Function, Real, Real>, Real>;

template <class Function>
concept ScalarFunction2D =
    std::invocable<Function, Real, Real> &&
    std::convertible_to<std::invoke_result_t<Function, Real, Real>, Real>;

template <class Function>
concept ScalarFunction2DTime =
    std::invocable<Function, Real, Real, Real> &&
    std::convertible_to<
        std::invoke_result_t<Function, Real, Real, Real>,
        Real
    >;

template <class Function>
concept VectorFunction1D =
    std::invocable<Function, Real> &&
    std::convertible_to<std::invoke_result_t<Function, Real>, VectorValue1D>;

template <class Function>
concept VectorFunction2D =
    std::invocable<Function, Real, Real> &&
    std::convertible_to<std::invoke_result_t<Function, Real, Real>, VectorValue2D>;

using StoredScalarFunction1D = std::function<Real(Real)>;
using StoredScalarFunction1DTime = std::function<Real(Real, Real)>;
using StoredScalarFunction2D = std::function<Real(Real, Real)>;
using StoredScalarFunction2DTime = std::function<Real(Real, Real, Real)>;
using StoredVectorFunction1D = std::function<VectorValue1D(Real)>;
using StoredVectorFunction2D = std::function<VectorValue2D(Real, Real)>;

}  // namespace fvm

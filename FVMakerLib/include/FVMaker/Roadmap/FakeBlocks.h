// ============================================================================
// Arquivo: FakeBlocks.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Fake Blocks no contexto de Roadmap.
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

#include <array>
#include <string_view>

#include <FVMaker/Core/Types.h>

namespace fvm::roadmap {

struct FakeBlockResult final {
    int block{};
    std::string_view name{};
    bool callable{true};
    bool provisional{true};
};

[[nodiscard]] constexpr FakeBlockResult transient_1d() noexcept {
    return {15, "Transient1D/Ddt1D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult time_schemes_1d() noexcept {
    return {16, "TimeSchemes1D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult wave_1d() noexcept {
    return {17, "Wave1D/D2dt2_1D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult grid_view_2d() noexcept {
    return {18, "GridView2D/GridMetrics2D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult field_boundary_2d() noexcept {
    return {19, "Field2D/Boundary2D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult linear_system_2d() noexcept {
    return {20, "SparseStructuredMatrix2D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult laplacian_2d() noexcept {
    return {21, "Laplacian2D/Poisson2D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult linear_solvers_2d() noexcept {
    return {22, "Jacobi2D/GaussSeidel2D/CG2D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult nonlinear_2d() noexcept {
    return {23, "NonlinearSolve2D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult transient_2d() noexcept {
    return {24, "Transient2D/Ddt2D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult wave_2d() noexcept {
    return {25, "Wave2D/D2dt2_2D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult advection_2d() noexcept {
    return {26, "Advection2D/Interpolation2D", true, true};
}

[[nodiscard]] constexpr FakeBlockResult output_fields() noexcept {
    return {27, "OutputFields", true, true};
}

[[nodiscard]] constexpr FakeBlockResult yaml_examples() noexcept {
    return {28, "ExternalYamlExamples", true, true};
}

[[nodiscard]] constexpr FakeBlockResult performance_profiling() noexcept {
    return {29, "PerformanceProfiling", true, true};
}

[[nodiscard]] constexpr std::array<FakeBlockResult, 15> remaining_blocks()
    noexcept {
    return {
        transient_1d(),
        time_schemes_1d(),
        wave_1d(),
        grid_view_2d(),
        field_boundary_2d(),
        linear_system_2d(),
        laplacian_2d(),
        linear_solvers_2d(),
        nonlinear_2d(),
        transient_2d(),
        wave_2d(),
        advection_2d(),
        output_fields(),
        yaml_examples(),
        performance_profiling()
    };
}

[[nodiscard]] constexpr bool all_remaining_blocks_callable() noexcept {
    for (const FakeBlockResult block : remaining_blocks()) {
        if (!block.callable || !block.provisional) {
            return false;
        }
    }

    return true;
}

}  // namespace fvm::roadmap

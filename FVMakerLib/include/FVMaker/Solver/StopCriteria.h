// ============================================================================
// Arquivo: StopCriteria.h
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Declara Stop Criteria no contexto de Solver.
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

#include <span>
#include <string_view>
#include <vector>

#include <FVMaker/Algebra/DenseVector.h>
#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/Core/ID.h>
#include <FVMaker/Core/Types.h>

namespace fvm {

enum class StopCriterionKind {
    correction_absolute,
    correction_relative_solution,
    correction_relative_range,
    correction_rms,
    correction_weighted_rms,
    residual_absolute,
    residual_relative_initial,
    max_iterations,
};

[[nodiscard]] std::string_view name(StopCriterionKind kind) noexcept;

struct StopCriterion final {
    StopCriterionKind kind{StopCriterionKind::residual_absolute};
    Real tolerance{1.0e-10};
    NormType norm{NormType::infinity};
};

struct StopCriteriaState final {
    Size iteration{};
    Size max_iterations{};
    const DenseVector* solution{};
    const DenseVector* correction{};
    const DenseVector* residual{};
    const DenseVector* initial_residual{};
    Real initial_residual_norm{};
    std::span<const Real> weights{};
};

struct StopCriteriaEvaluation final {
    bool converged{false};
    bool reached_iteration_limit{false};
    StopCriterionKind criterion{StopCriterionKind::max_iterations};
    Real value{};
    Real tolerance{};
};

class StopCriteria final {
public:
    StopCriteria() = default;
    explicit StopCriteria(std::vector<StopCriterion> criteria);

    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"Solver", "StopCriteria", "fvm.solver.StopCriteria"};
    }

    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    [[nodiscard]] static StopCriteria residual_absolute(Real tolerance, NormType norm = NormType::infinity);
    [[nodiscard]] static StopCriteria residual_relative_initial(Real tolerance, NormType norm = NormType::infinity);
    [[nodiscard]] static StopCriteria correction_absolute(Real tolerance);
    [[nodiscard]] static StopCriteria chapter_defaults(Real tolerance);

    void add(StopCriterion criterion);
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] std::span<const StopCriterion> criteria() const noexcept;

    [[nodiscard]] StopCriteriaEvaluation evaluate(
        const StopCriteriaState& state
    ) const;

    void validate() const;

private:
    std::vector<StopCriterion> criteria_;
};

}  // namespace fvm

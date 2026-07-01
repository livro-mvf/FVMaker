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

// Classifica o criterio usado para parar uma iteracao.
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

// Realiza a operacao name definida por esta interface.
[[nodiscard]] std::string_view name(StopCriterionKind kind) noexcept;

// Representa o conceito de stop criterion dentro da biblioteca FVMaker.
struct StopCriterion final {
    StopCriterionKind kind{StopCriterionKind::residual_absolute};
    Real tolerance{1.0e-10};
    NormType norm{NormType::infinity};
};

// Representa o conceito de stop criteria state dentro da biblioteca FVMaker.
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

// Representa o conceito de stop criteria evaluation dentro da biblioteca FVMaker.
struct StopCriteriaEvaluation final {
    bool converged{false};
    bool reached_iteration_limit{false};
    StopCriterionKind criterion{StopCriterionKind::max_iterations};
    Real value{};
    Real tolerance{};
};

// Agrupa criterios de parada usados por algoritmos iterativos.
class StopCriteria final {
public:
    // Cria um objeto StopCriteria com os dados fornecidos.
    StopCriteria() = default;
    // Cria um objeto StopCriteria com os dados fornecidos.
    explicit StopCriteria(std::vector<StopCriterion> criteria);

    // Retorna o identificador estavel desta classe na biblioteca.
    [[nodiscard]] static constexpr ID id() noexcept {
        return ID{"Solver", "StopCriteria", "fvm.solver.StopCriteria"};
    }

    // Retorna o nome curto da classe para diagnostico e documentacao.
    [[nodiscard]] static constexpr std::string_view class_name() noexcept {
        return id().class_name();
    }

    // Retorna o identificador completo da classe na hierarquia da biblioteca.
    [[nodiscard]] static constexpr std::string_view class_id() noexcept {
        return id().class_id();
    }

    // Calcula a grandeza residual absolute definida por esta interface.
    [[nodiscard]] static StopCriteria residual_absolute(Real tolerance, NormType norm = NormType::infinity);
    // Calcula a grandeza residual relative initial definida por esta interface.
    [[nodiscard]] static StopCriteria residual_relative_initial(Real tolerance, NormType norm = NormType::infinity);
    // Realiza a operacao correction absolute definida por esta interface.
    [[nodiscard]] static StopCriteria correction_absolute(Real tolerance);
    // Realiza a operacao chapter defaults definida por esta interface.
    [[nodiscard]] static StopCriteria chapter_defaults(Real tolerance);

    // Acrescenta a informacao add ao objeto.
    void add(StopCriterion criterion);
    // Retorna a informacao empty associada ao objeto.
    [[nodiscard]] bool empty() const noexcept;
    // Retorna a informacao criteria associada ao objeto.
    [[nodiscard]] std::span<const StopCriterion> criteria() const noexcept;

    // Calcula a grandeza evaluate definida por esta interface.
    [[nodiscard]] StopCriteriaEvaluation evaluate(
        const StopCriteriaState& state
    ) const;

    // Verifica se as hipoteses numericas e estruturais foram atendidas.
    void validate() const;

private:
    std::vector<StopCriterion> criteria_;
};

}  // namespace fvm

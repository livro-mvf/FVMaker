// ============================================================================
// Arquivo: tst_StopCriteria.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Stop Criteria no contexto de Solver.
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

#include <string_view>
#include <vector>

#include <FVMaker/Solver/StopCriteria.h>

#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] StopCriteriaState make_state() {
    static const DenseVector solution{std::vector<Real>{2.0, 4.0, 6.0}};
    static const DenseVector correction{std::vector<Real>{0.1, -0.2, 0.3}};
    static const DenseVector residual{std::vector<Real>{0.01, -0.02, 0.03}};
    static const std::vector<Real> weights{1.0, 2.0, 3.0};

    return StopCriteriaState{
        .iteration = 4,
        .max_iterations = 10,
        .solution = &solution,
        .correction = &correction,
        .residual = &residual,
        .initial_residual_norm = 3.0,
        .weights = weights
    };
}

}  // namespace

TEST(StopCriteria, StoresCriterionNames) {
    EXPECT_EQ(
        name(StopCriterionKind::correction_absolute),
        std::string_view{"correction_absolute"}
    );
    EXPECT_EQ(
        name(StopCriterionKind::residual_relative_initial),
        std::string_view{"residual_relative_initial"}
    );
}

TEST(StopCriteria, AcceptsAbsoluteCorrectionCriterion) {
    const StopCriteria criteria{
        {StopCriterion{StopCriterionKind::correction_absolute, 0.31}}
    };

    const StopCriteriaEvaluation result = criteria.evaluate(make_state());

    EXPECT_TRUE(result.converged);
    EXPECT_EQ(result.criterion, StopCriterionKind::correction_absolute);
    EXPECT_DOUBLE_EQ(result.value, 0.3);
}

TEST(StopCriteria, AcceptsRelativeCorrectionBySolutionNorm) {
    const StopCriteria criteria{
        {StopCriterion{StopCriterionKind::correction_relative_solution, 0.051}}
    };

    const StopCriteriaEvaluation result = criteria.evaluate(make_state());

    EXPECT_TRUE(result.converged);
    EXPECT_EQ(result.criterion, StopCriterionKind::correction_relative_solution);
    EXPECT_DOUBLE_EQ(result.value, 0.3 / 6.0);
}

TEST(StopCriteria, AcceptsRelativeCorrectionBySolutionRange) {
    const StopCriteria criteria{
        {StopCriterion{StopCriterionKind::correction_relative_range, 0.076}}
    };

    const StopCriteriaEvaluation result = criteria.evaluate(make_state());

    EXPECT_TRUE(result.converged);
    EXPECT_EQ(result.criterion, StopCriterionKind::correction_relative_range);
    EXPECT_DOUBLE_EQ(result.value, 0.3 / 4.0);
}

TEST(StopCriteria, AcceptsRmsAndWeightedRmsCorrectionCriteria) {
    const StopCriteria rms{
        {StopCriterion{StopCriterionKind::correction_rms, 0.217}}
    };
    const StopCriteria weighted{
        {StopCriterion{StopCriterionKind::correction_weighted_rms, 0.25}}
    };

    EXPECT_TRUE(rms.evaluate(make_state()).converged);
    EXPECT_TRUE(weighted.evaluate(make_state()).converged);
}

TEST(StopCriteria, AcceptsResidualCriteria) {
    const StopCriteria absolute{
        {StopCriterion{StopCriterionKind::residual_absolute, 0.031}}
    };
    const StopCriteria relative{
        {StopCriterion{StopCriterionKind::residual_relative_initial, 0.011}}
    };

    EXPECT_TRUE(absolute.evaluate(make_state()).converged);
    EXPECT_TRUE(relative.evaluate(make_state()).converged);
}

TEST(StopCriteria, ReportsIterationLimitAfterCriteriaFail) {
    const StopCriteria criteria{
        {StopCriterion{StopCriterionKind::residual_absolute, 1.0e-30}}
    };
    StopCriteriaState state = make_state();
    state.iteration = 10;

    const StopCriteriaEvaluation result = criteria.evaluate(state);

    EXPECT_FALSE(result.converged);
    EXPECT_TRUE(result.reached_iteration_limit);
    EXPECT_EQ(result.criterion, StopCriterionKind::max_iterations);
}


TEST(StopCriteria, NormalizesByActualSmallInitialResidual) {
    const DenseVector initial{std::vector<Real>{0.1}};
    const DenseVector residual{std::vector<Real>{0.01}};
    StopCriteriaState state = make_state();
    state.residual = &residual;
    state.initial_residual = &initial;

    const StopCriteria criteria{{StopCriterion{
        StopCriterionKind::residual_relative_initial,
        0.05,
        NormType::l2
    }}};
    const StopCriteriaEvaluation result = criteria.evaluate(state);

    EXPECT_FALSE(result.converged);
}

}  // namespace fvm

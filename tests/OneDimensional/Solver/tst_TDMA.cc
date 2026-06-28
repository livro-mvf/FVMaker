// ============================================================================
// Arquivo: tst_TDMA.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst TDMA no contexto de OneDimensional / Solver.
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

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Solver/TDMA.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(TDMA, StoresClassIdentity) {
    EXPECT_EQ(TDMA::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(TDMA::id().class_name(), std::string_view{"TDMA"});
    EXPECT_EQ(TDMA::id().class_id(), std::string_view{"fvm.1d.solver.TDMA"});
}

TEST(TDMA, SolvesSingleEquationSystem) {
    const TridiagonalSystem1D system{
        std::vector<Real>{},
        std::vector<Real>{2.0},
        std::vector<Real>{},
        DenseVector{std::vector<Real>{8.0}}
    };

    const SolveResult result = TDMA::solve(system);

    ASSERT_EQ(result.solution.size(), static_cast<Size>(1));
    EXPECT_TRUE(result.converged);
    EXPECT_EQ(result.iterations, static_cast<Size>(1));
    EXPECT_DOUBLE_EQ(result.solution[0], 4.0);
}

TEST(TDMA, SolvesKnownThreeByThreeSystem) {
    const TridiagonalSystem1D system{
        std::vector<Real>{-1.0, -1.0},
        std::vector<Real>{2.0, 2.0, 2.0},
        std::vector<Real>{-1.0, -1.0},
        DenseVector{std::vector<Real>{1.0, 0.0, 1.0}}
    };

    const SolveResult result = TDMA::solve(system);

    ASSERT_EQ(result.solution.size(), static_cast<Size>(3));
    EXPECT_TRUE(result.converged);
    EXPECT_EQ(result.iterations, static_cast<Size>(1));
    EXPECT_DOUBLE_EQ(result.solution[0], 1.0);
    EXPECT_DOUBLE_EQ(result.solution[1], 1.0);
    EXPECT_DOUBLE_EQ(result.solution[2], 1.0);
}

TEST(TDMA, SolvesNonSymmetricTridiagonalSystem) {
    const TridiagonalSystem1D system{
        std::vector<Real>{1.0, 1.0},
        std::vector<Real>{2.0, 2.0, 2.0},
        std::vector<Real>{1.0, 1.0},
        DenseVector{std::vector<Real>{4.0, 8.0, 8.0}}
    };

    const SolveResult result = TDMA::solve(system);

    ASSERT_EQ(result.solution.size(), static_cast<Size>(3));
    EXPECT_DOUBLE_EQ(result.solution[0], 1.0);
    EXPECT_DOUBLE_EQ(result.solution[1], 2.0);
    EXPECT_DOUBLE_EQ(result.solution[2], 3.0);
}

TEST(TDMA, RejectsZeroFirstPivot) {
    const TridiagonalSystem1D system{
        std::vector<Real>{1.0},
        std::vector<Real>{0.0, 2.0},
        std::vector<Real>{1.0},
        DenseVector{std::vector<Real>{1.0, 1.0}}
    };

    EXPECT_THROW(static_cast<void>(TDMA::solve(system)), FVMException);
}

TEST(TDMA, RejectsZeroPivotDuringElimination) {
    const TridiagonalSystem1D system{
        std::vector<Real>{1.0},
        std::vector<Real>{1.0, 1.0},
        std::vector<Real>{1.0},
        DenseVector{std::vector<Real>{1.0, 1.0}}
    };

    try {
        static_cast<void>(TDMA::solve(system));
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kSingularSystem.code);
        EXPECT_EQ(exception.record().category, std::string_view{"System"});
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"TDMA"});
        return;
    }

    FAIL() << "TDMA did not reject a zero pivot.";
}


TEST(TDMA, ExposesForwardSweepCoefficients) {
    const TridiagonalSystem1D system{
        std::vector<Real>{-1.0, -1.0},
        std::vector<Real>{2.0, 2.0, 2.0},
        std::vector<Real>{-1.0, -1.0},
        DenseVector{std::vector<Real>{1.0, 0.0, 1.0}}
    };

    const TDMAForwardSweep sweep = TDMA::forward_sweep(system);

    ASSERT_EQ(sweep.t.size(), static_cast<Size>(3));
    ASSERT_EQ(sweep.q.size(), static_cast<Size>(3));

    EXPECT_DOUBLE_EQ(sweep.t[0], 0.5);
    EXPECT_DOUBLE_EQ(sweep.t[1], 2.0 / 3.0);
    EXPECT_DOUBLE_EQ(sweep.t[2], 0.0);

    EXPECT_DOUBLE_EQ(sweep.q[0], 0.5);
    EXPECT_DOUBLE_EQ(sweep.q[1], 1.0 / 3.0);
    EXPECT_DOUBLE_EQ(sweep.q[2], 1.0);
}

}  // namespace fvm

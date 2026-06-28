// ============================================================================
// Arquivo: tst_Variable1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Variable 1 D no contexto de OneDimensional / Variable.
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

#include <cmath>
#include <sstream>
#include <string_view>
#include <vector>

#include <FVGridMaker/Core/StrongTypes.h>
#include <FVGridMaker/OneDimensional/Distribution1D/Uniform1D.h>

#include <FVMaker/Algebra/ErrorNorms.h>
#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Grid/GridView1D.h>
#include <FVMaker/OneDimensional/Variable/Variable1D.h>
#include <FVMaker/Solver/SolveResult.h>

#include <gtest/gtest.h>

namespace fvm {

namespace {

[[nodiscard]] GridView1D make_grid() {
    static const fvgrid::Axis1D axis = fvgrid::uniform_axis_1d(
        fvgrid::NVol{4},
        fvgrid::Length{2.0},
        fvgrid::XInit{0.0}
    );

    return GridView1D{axis};
}

}  // namespace

TEST(Variable1D, StoresClassIdentity) {
    EXPECT_EQ(Variable1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(Variable1D::id().class_name(), std::string_view{"Variable1D"});
    EXPECT_EQ(
        Variable1D::id().class_id(),
        std::string_view{"fvm.1d.variable.Variable1D"}
    );
}

TEST(Variable1D, InitializesAllNumericalStates) {
    const Variable1D variable{make_grid(), "phi", 2.0};

    EXPECT_EQ(variable.name(), std::string_view{"phi"});
    EXPECT_EQ(variable.size(), static_cast<Size>(4));

    for (Size i = 0; i < variable.size(); ++i) {
        EXPECT_DOUBLE_EQ(variable.phi()[i], 2.0);
        EXPECT_DOUBLE_EQ(variable.phi_ant()[i], 2.0);
        EXPECT_DOUBLE_EQ(variable.phi_iter()[i], 2.0);
        EXPECT_DOUBLE_EQ(variable.phi_analit()[i], 0.0);
    }
}

TEST(Variable1D, UpdatesPhiWhileKeepingPreviousPhi) {
    Variable1D variable{make_grid(), "phi", 1.0};

    variable.update_phi(DenseVector{std::vector<Real>{4.0, 3.0, 2.0, 1.0}});

    EXPECT_DOUBLE_EQ(variable.phi_ant()[0], 1.0);
    EXPECT_DOUBLE_EQ(variable.phi_ant()[3], 1.0);
    EXPECT_DOUBLE_EQ(variable.phi()[0], 4.0);
    EXPECT_DOUBLE_EQ(variable.phi()[3], 1.0);
}

TEST(Variable1D, UpdatesPhiFromSolveResult) {
    Variable1D variable{make_grid(), "phi"};
    SolveResult result;
    result.solution = DenseVector{std::vector<Real>{1.0, 2.0, 3.0, 4.0}};

    variable.update_phi(result);

    EXPECT_DOUBLE_EQ(variable.phi()[0], 1.0);
    EXPECT_DOUBLE_EQ(variable.phi()[3], 4.0);
}

TEST(Variable1D, StoresIterationAndAdvancesItToCurrentPhi) {
    Variable1D variable{make_grid(), "phi", 1.0};

    variable.save_iteration(DenseVector{std::vector<Real>{0.0, 1.0, 2.0, 3.0}});
    variable.advance_iteration_to_phi();

    EXPECT_DOUBLE_EQ(variable.phi_ant()[0], 1.0);
    EXPECT_DOUBLE_EQ(variable.phi()[0], 0.0);
    EXPECT_DOUBLE_EQ(variable.phi()[3], 3.0);
}

TEST(Variable1D, AssignsAnalyticSolutionAndComputesError) {
    Variable1D variable{
        make_grid(),
        "phi",
        std::vector<Real>{0.25, 0.75, 1.25, 1.75}
    };

    variable.assign_analytic([](Real x) {
        return x;
    });

    const DenseVector error = variable.analytic_error();

    EXPECT_LE(norm_infinity(error), 1.0e-15);
}

TEST(Variable1D, ComputesDifferencesAndNormsBetweenStoredStates) {
    Variable1D variable{make_grid(), "phi", 1.0};

    variable.save_iteration(DenseVector{std::vector<Real>{0.0, 1.0, 2.0, 3.0}});
    variable.update_phi(DenseVector{std::vector<Real>{2.0, 4.0, 6.0, 8.0}});
    variable.assign_analytic([](Real) {
        return 3.0;
    });

    const DenseVector to_previous = variable.difference_to_previous();
    const DenseVector to_iteration = variable.difference_to_iteration();
    const DenseVector to_analytic = variable.difference_to_analytic();

    EXPECT_DOUBLE_EQ(to_previous[0], 1.0);
    EXPECT_DOUBLE_EQ(to_previous[3], 7.0);
    EXPECT_DOUBLE_EQ(to_iteration[0], 2.0);
    EXPECT_DOUBLE_EQ(to_iteration[3], 5.0);
    EXPECT_DOUBLE_EQ(to_analytic[0], -1.0);
    EXPECT_DOUBLE_EQ(to_analytic[3], 5.0);

    const VariableDifferenceNorms1D previous_norms =
        variable.norms_to_previous();
    const VariableDifferenceNorms1D iteration_norms =
        variable.norms_to_iteration();
    const VariableDifferenceNorms1D analytic_norms =
        variable.norms_to_analytic();

    EXPECT_DOUBLE_EQ(previous_norms.infinity, 7.0);
    EXPECT_DOUBLE_EQ(previous_norms.l1, 16.0);
    EXPECT_NEAR(previous_norms.l2, std::sqrt(1.0 + 9.0 + 25.0 + 49.0), 1.0e-15);

    EXPECT_DOUBLE_EQ(iteration_norms.infinity, 5.0);
    EXPECT_DOUBLE_EQ(iteration_norms.l1, 14.0);

    EXPECT_DOUBLE_EQ(analytic_norms.infinity, 5.0);
    EXPECT_DOUBLE_EQ(analytic_norms.l1, 10.0);
}

TEST(Variable1D, PrintsAllStoredStatesInColumns) {
    Variable1D variable{make_grid(), "phi", std::vector<Real>{1.0, 2.0, 3.0, 4.0}};
    variable.assign_analytic([](Real x) {
        return x;
    });

    std::ostringstream output;
    output << variable;

    const std::string text = output.str();
    EXPECT_NE(text.find("indice"), std::string::npos);
    EXPECT_NE(text.find("phi_ant"), std::string::npos);
    EXPECT_NE(text.find("phi_iter"), std::string::npos);
    EXPECT_NE(text.find("phi_analit"), std::string::npos);
}

TEST(Variable1D, RejectsWrongSizeVectors) {
    try {
        const Variable1D variable{
            make_grid(),
            "phi",
            std::vector<Real>{1.0, 2.0}
        };
    } catch (const FVMException& exception) {
        EXPECT_EQ(exception.record().code, error_catalog::kInvalidFieldSize.code);
        EXPECT_EQ(exception.record().source.class_name(), std::string_view{"Variable1D"});
        return;
    }

    FAIL() << "Variable1D accepted a vector with the wrong size.";
}

}  // namespace fvm

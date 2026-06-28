// ============================================================================
// Arquivo: tst_Boundary1D.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Boundary 1 D no contexto de OneDimensional / Boundary.
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
#include <type_traits>

#include <FVMaker/ErrorHandling/ErrorCatalog.h>
#include <FVMaker/ErrorHandling/FVMException.h>
#include <FVMaker/OneDimensional/Boundary/BoundaryCondition1D.h>

#include <gtest/gtest.h>

namespace fvm {

static_assert(std::is_final_v<BoundaryCondition1D>);

TEST(Boundary1D, BoundaryConditionStoresClassIdentity) {
    EXPECT_EQ(
        BoundaryCondition1D::id().module(),
        std::string_view{"OneDimensional"}
    );
    EXPECT_EQ(
        BoundaryCondition1D::id().class_name(),
        std::string_view{"BoundaryCondition1D"}
    );
    EXPECT_EQ(
        BoundaryCondition1D::id().class_id(),
        std::string_view{"fvm.1d.boundary.BoundaryCondition1D"}
    );
}

TEST(Boundary1D, BoundarySetStoresClassIdentity) {
    EXPECT_EQ(BoundarySet1D::id().module(), std::string_view{"OneDimensional"});
    EXPECT_EQ(BoundarySet1D::id().class_name(), std::string_view{"BoundarySet1D"});
    EXPECT_EQ(
        BoundarySet1D::id().class_id(),
        std::string_view{"fvm.1d.boundary.BoundarySet1D"}
    );
}

TEST(Boundary1D, StoresGeneralLinearBoundaryCondition) {
    const BoundaryCondition1D condition = linear_boundary_1d(2.0, 3.0, 4.0);

    EXPECT_DOUBLE_EQ(condition.alpha(0.0), 2.0);
    EXPECT_DOUBLE_EQ(condition.beta(0.0), 3.0);
    EXPECT_DOUBLE_EQ(condition.gamma(0.0), 4.0);
}

TEST(Boundary1D, StoresFunctionalLinearBoundaryCondition) {
    const BoundaryCondition1D condition = linear_boundary_1d(
        StoredBoundaryFunction1D{[](Real x, Real) {
            return Real{1.0} + x;
        }},
        StoredBoundaryFunction1D{[](Real, Real time) {
            return Real{2.0} + time;
        }},
        StoredBoundaryFunction1D{[](Real x, Real time) {
            return x + time;
        }}
    );

    EXPECT_DOUBLE_EQ(condition.alpha(3.0, 4.0), 4.0);
    EXPECT_DOUBLE_EQ(condition.beta(3.0, 4.0), 6.0);
    EXPECT_DOUBLE_EQ(condition.gamma(3.0, 4.0), 7.0);
}

TEST(Boundary1D, DirichletIsLinearBoundaryShortcut) {
    const BoundaryCondition1D condition = dirichlet_1d(2.5);

    EXPECT_EQ(condition.kind(), BoundaryConditionKind1D::dirichlet);
    EXPECT_EQ(condition.kind_name(), std::string_view{"Dirichlet"});
    EXPECT_TRUE(condition.is_dirichlet());
    EXPECT_FALSE(condition.is_neumann());
    EXPECT_FALSE(condition.is_robin());
    EXPECT_DOUBLE_EQ(condition.alpha(0.0), 1.0);
    EXPECT_DOUBLE_EQ(condition.beta(0.0), 0.0);
    EXPECT_DOUBLE_EQ(condition.gamma(0.0), 2.5);
}

TEST(Boundary1D, NeumannIsLinearBoundaryShortcut) {
    const BoundaryCondition1D condition = neumann_1d(-1.25);

    EXPECT_EQ(condition.kind(), BoundaryConditionKind1D::neumann);
    EXPECT_EQ(condition.kind_name(), std::string_view{"Neumann"});
    EXPECT_FALSE(condition.is_dirichlet());
    EXPECT_TRUE(condition.is_neumann());
    EXPECT_FALSE(condition.is_robin());
    EXPECT_DOUBLE_EQ(condition.alpha(0.0), 0.0);
    EXPECT_DOUBLE_EQ(condition.beta(0.0), 1.0);
    EXPECT_DOUBLE_EQ(condition.gamma(0.0), -1.25);
}

TEST(Boundary1D, RobinIsLinearBoundaryShortcut) {
    const BoundaryCondition1D condition = robin_1d(2.0, 3.0, 4.0);

    EXPECT_EQ(condition.kind(), BoundaryConditionKind1D::robin);
    EXPECT_EQ(condition.kind_name(), std::string_view{"Robin"});
    EXPECT_FALSE(condition.is_dirichlet());
    EXPECT_FALSE(condition.is_neumann());
    EXPECT_TRUE(condition.is_robin());
    EXPECT_DOUBLE_EQ(condition.alpha(0.0), 2.0);
    EXPECT_DOUBLE_EQ(condition.beta(0.0), 3.0);
    EXPECT_DOUBLE_EQ(condition.gamma(0.0), 4.0);
}

TEST(Boundary1D, ClassFactoriesDefineBoundaryConditionsWithoutInheritance) {
    const BoundaryCondition1D left = BoundaryCondition1D::dirichlet(1.0);
    const BoundaryCondition1D right = BoundaryCondition1D::neumann(0.0);
    const BoundaryCondition1D mixed = BoundaryCondition1D::robin(2.0, 3.0, 4.0);

    EXPECT_TRUE(left.is_dirichlet());
    EXPECT_DOUBLE_EQ(left.alpha(0.0), 1.0);
    EXPECT_DOUBLE_EQ(left.beta(0.0), 0.0);
    EXPECT_DOUBLE_EQ(left.gamma(0.0), 1.0);

    EXPECT_TRUE(right.is_neumann());
    EXPECT_DOUBLE_EQ(right.alpha(0.0), 0.0);
    EXPECT_DOUBLE_EQ(right.beta(0.0), 1.0);
    EXPECT_DOUBLE_EQ(right.gamma(0.0), 0.0);

    EXPECT_TRUE(mixed.is_robin());
    EXPECT_DOUBLE_EQ(mixed.alpha(0.0), 2.0);
    EXPECT_DOUBLE_EQ(mixed.beta(0.0), 3.0);
    EXPECT_DOUBLE_EQ(mixed.gamma(0.0), 4.0);
}

TEST(Boundary1D, BoundarySetStoresLeftAndRightConditions) {
    const BoundarySet1D boundaries{dirichlet_1d(1.0), neumann_1d(0.0)};

    EXPECT_DOUBLE_EQ(boundaries.left().alpha(0.0), 1.0);
    EXPECT_DOUBLE_EQ(boundaries.left().gamma(0.0), 1.0);
    EXPECT_DOUBLE_EQ(boundaries.right().beta(1.0), 1.0);
    EXPECT_DOUBLE_EQ(boundaries.right().gamma(1.0), 0.0);

    EXPECT_DOUBLE_EQ(boundaries.at(BoundarySide1D::left).gamma(0.0), 1.0);
    EXPECT_DOUBLE_EQ(boundaries.at(BoundarySide1D::right).gamma(1.0), 0.0);
}

TEST(Boundary1D, RejectsInvalidLinearBoundaryCoefficients) {
    try {
        const BoundaryCondition1D condition = linear_boundary_1d(0.0, 0.0, 1.0);
        (void)condition;
    } catch (const FVMException& exception) {
        EXPECT_EQ(
            exception.record().code,
            error_catalog::kInvalidBoundaryCondition.code
        );
        EXPECT_EQ(exception.record().category, std::string_view{"Boundary"});
        EXPECT_EQ(
            exception.record().source.class_name(),
            std::string_view{"BoundaryCondition1D"}
        );
        return;
    }

    FAIL() << "BoundaryCondition1D did not reject alpha=0 and beta=0.";
}

}  // namespace fvm

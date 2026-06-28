// ============================================================================
// Arquivo: tst_ErrorNorms.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Error Norms no contexto de Algebra.
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

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/Algebra/ErrorNorms.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(ErrorNorms, EmptyVectorHasZeroNorms) {
    const DenseVector vector;

    EXPECT_DOUBLE_EQ(norm_infinity(vector), 0.0);
    EXPECT_DOUBLE_EQ(norm_l1(vector), 0.0);
    EXPECT_DOUBLE_EQ(norm_l2(vector), 0.0);
}

TEST(ErrorNorms, ComputesInfinityNorm) {
    const DenseVector vector{std::vector<Real>{-2.0, 3.5, -1.0}};

    EXPECT_DOUBLE_EQ(norm_infinity(vector), 3.5);
    EXPECT_DOUBLE_EQ(VectorNorms::infinity(vector), 3.5);
    EXPECT_DOUBLE_EQ(VectorNorms::evaluate(vector, NormType::infinity), 3.5);
}

TEST(ErrorNorms, ComputesL1Norm) {
    const DenseVector vector{std::vector<Real>{-2.0, 3.5, -1.0}};

    EXPECT_DOUBLE_EQ(norm_l1(vector), 6.5);
    EXPECT_DOUBLE_EQ(VectorNorms::l1(vector), 6.5);
    EXPECT_DOUBLE_EQ(VectorNorms::evaluate(vector, NormType::l1), 6.5);
}

TEST(ErrorNorms, ComputesL2Norm) {
    const DenseVector vector{std::vector<Real>{3.0, 4.0}};

    EXPECT_DOUBLE_EQ(norm_l2(vector), 5.0);
    EXPECT_DOUBLE_EQ(VectorNorms::l2(vector), 5.0);
    EXPECT_DOUBLE_EQ(VectorNorms::evaluate(vector, NormType::l2), 5.0);
}

TEST(ErrorNorms, ComputesRmsNorm) {
    const DenseVector vector{std::vector<Real>{3.0, 4.0}};
    EXPECT_DOUBLE_EQ(norm_rms(vector), std::sqrt(12.5));
    EXPECT_DOUBLE_EQ(VectorNorms::evaluate(vector, NormType::rms), std::sqrt(12.5));
}

}  // namespace fvm

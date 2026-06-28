// ============================================================================
// Arquivo: tst_ErrorCodes.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Error Codes no contexto de ErrorHandling.
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

// ----------------------------------------------------------------------------
// FVMaker includes
// ----------------------------------------------------------------------------
#include <FVMaker/ErrorHandling/ErrorCodes.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(ErrorCodes, CoreCodesAreStable) {
    EXPECT_EQ(
        error_code::kInvalidArgument,
        std::string_view{"FVM.CORE.INVALID_ARGUMENT"}
    );
    EXPECT_EQ(error_code::kOutOfRange, std::string_view{"FVM.CORE.OUT_OF_RANGE"});
    EXPECT_EQ(
        error_code::kNotImplemented,
        std::string_view{"FVM.CORE.NOT_IMPLEMENTED"}
    );
    EXPECT_EQ(
        error_code::kInternalError,
        std::string_view{"FVM.CORE.INTERNAL_ERROR"}
    );
    EXPECT_EQ(
        error_code::kInvalidTolerance,
        std::string_view{"FVM.CORE.INVALID_TOLERANCE"}
    );
}

TEST(ErrorCodes, MeshCodesAreStable) {
    EXPECT_EQ(error_code::kMissingMesh, std::string_view{"FVM.MESH.MISSING_MESH"});
    EXPECT_EQ(
        error_code::kInvalidMeshDimension,
        std::string_view{"FVM.MESH.INVALID_DIMENSION"}
    );
    EXPECT_EQ(
        error_code::kIncompatibleMesh,
        std::string_view{"FVM.MESH.INCOMPATIBLE_MESH"}
    );
}

TEST(ErrorCodes, FieldCoefficientSourceAndBoundaryCodesAreStable) {
    EXPECT_EQ(
        error_code::kInvalidFieldSize,
        std::string_view{"FVM.FIELD.INVALID_SIZE"}
    );
    EXPECT_EQ(error_code::kMissingField, std::string_view{"FVM.FIELD.MISSING_FIELD"});
    EXPECT_EQ(
        error_code::kInvalidCoefficient,
        std::string_view{"FVM.COEFFICIENT.INVALID_COEFFICIENT"}
    );
    EXPECT_EQ(
        error_code::kInvalidSource,
        std::string_view{"FVM.SOURCE.INVALID_SOURCE"}
    );
    EXPECT_EQ(
        error_code::kInvalidBoundaryCondition,
        std::string_view{"FVM.BOUNDARY.INVALID_CONDITION"}
    );
    EXPECT_EQ(
        error_code::kMissingBoundaryCondition,
        std::string_view{"FVM.BOUNDARY.MISSING_CONDITION"}
    );
}

TEST(ErrorCodes, EquationAssemblySystemSolverAndTimeCodesAreStable) {
    EXPECT_EQ(error_code::kInvalidTerm, std::string_view{"FVM.TERM.INVALID_TERM"});
    EXPECT_EQ(
        error_code::kIncompatibleTerms,
        std::string_view{"FVM.TERM.INCOMPATIBLE_TERMS"}
    );
    EXPECT_EQ(error_code::kAssemblyFailed, std::string_view{"FVM.ASSEMBLY.FAILED"});
    EXPECT_EQ(
        error_code::kSingularSystem,
        std::string_view{"FVM.SYSTEM.SINGULAR_SYSTEM"}
    );
    EXPECT_EQ(
        error_code::kInvalidSystemSize,
        std::string_view{"FVM.SYSTEM.INVALID_SIZE"}
    );
    EXPECT_EQ(
        error_code::kSolverDidNotConverge,
        std::string_view{"FVM.SOLVER.DID_NOT_CONVERGE"}
    );
    EXPECT_EQ(
        error_code::kInvalidTimeStep,
        std::string_view{"FVM.TIME.INVALID_TIME_STEP"}
    );
    EXPECT_EQ(
        error_code::kInvalidTimeInterval,
        std::string_view{"FVM.TIME.INVALID_INTERVAL"}
    );
}

TEST(ErrorCodes, OutputCodesAreStable) {
    EXPECT_EQ(
        error_code::kOutputFileOpenFailed,
        std::string_view{"FVM.OUTPUT.FILE_OPEN_FAILED"}
    );
    EXPECT_EQ(
        error_code::kOutputFileWriteFailed,
        std::string_view{"FVM.OUTPUT.FILE_WRITE_FAILED"}
    );
}

}  // namespace fvm

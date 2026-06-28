// ============================================================================
// Arquivo: tst_ErrorCatalog.cc
// Projeto: FVMaker
// Versão: consulte <FVMaker/Core/Version.h>
// Descrição: Testa tst Error Catalog no contexto de ErrorHandling.
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
#include <FVMaker/ErrorHandling/ErrorCatalog.h>

// ----------------------------------------------------------------------------
// External library includes
// ----------------------------------------------------------------------------
#include <gtest/gtest.h>

namespace fvm {

TEST(ErrorCatalog, CoreDescriptorsAreStable) {
    EXPECT_EQ(error_catalog::kInvalidArgument.code, error_code::kInvalidArgument);
    EXPECT_EQ(error_catalog::kInvalidArgument.message, std::string_view{"Invalid argument."});
    EXPECT_EQ(error_catalog::kInvalidArgument.category, std::string_view{"Core"});

    EXPECT_EQ(error_catalog::kOutOfRange.code, error_code::kOutOfRange);
    EXPECT_EQ(error_catalog::kOutOfRange.message, std::string_view{"Value is out of range."});
    EXPECT_EQ(error_catalog::kOutOfRange.category, std::string_view{"Core"});

    EXPECT_EQ(error_catalog::kNotImplemented.code, error_code::kNotImplemented);
    EXPECT_EQ(
        error_catalog::kNotImplemented.message,
        std::string_view{"Requested functionality is not implemented."}
    );
    EXPECT_EQ(error_catalog::kNotImplemented.category, std::string_view{"Core"});

    EXPECT_EQ(error_catalog::kInternalError.code, error_code::kInternalError);
    EXPECT_EQ(error_catalog::kInternalError.message, std::string_view{"Internal FVMaker error."});
    EXPECT_EQ(error_catalog::kInternalError.category, std::string_view{"Core"});

    EXPECT_EQ(error_catalog::kInvalidTolerance.code, error_code::kInvalidTolerance);
    EXPECT_EQ(
        error_catalog::kInvalidTolerance.message,
        std::string_view{"The tolerance value is invalid."}
    );
    EXPECT_EQ(error_catalog::kInvalidTolerance.category, std::string_view{"Core"});
}

TEST(ErrorCatalog, MeshDescriptorsAreStable) {
    EXPECT_EQ(error_catalog::kMissingMesh.code, error_code::kMissingMesh);
    EXPECT_EQ(
        error_catalog::kMissingMesh.message,
        std::string_view{"A mesh supplied by FVGridMaker is required."}
    );
    EXPECT_EQ(error_catalog::kMissingMesh.category, std::string_view{"Mesh"});

    EXPECT_EQ(error_catalog::kInvalidMeshDimension.code, error_code::kInvalidMeshDimension);
    EXPECT_EQ(
        error_catalog::kInvalidMeshDimension.message,
        std::string_view{"The mesh dimension is invalid for this operation."}
    );
    EXPECT_EQ(error_catalog::kInvalidMeshDimension.category, std::string_view{"Mesh"});

    EXPECT_EQ(error_catalog::kIncompatibleMesh.code, error_code::kIncompatibleMesh);
    EXPECT_EQ(
        error_catalog::kIncompatibleMesh.message,
        std::string_view{"The mesh is incompatible with the requested operation."}
    );
    EXPECT_EQ(error_catalog::kIncompatibleMesh.category, std::string_view{"Mesh"});
}

TEST(ErrorCatalog, NumericalObjectDescriptorsAreStable) {
    EXPECT_EQ(error_catalog::kInvalidFieldSize.code, error_code::kInvalidFieldSize);
    EXPECT_EQ(
        error_catalog::kInvalidFieldSize.message,
        std::string_view{"The field size is incompatible with the mesh."}
    );
    EXPECT_EQ(error_catalog::kInvalidFieldSize.category, std::string_view{"Field"});

    EXPECT_EQ(error_catalog::kMissingField.code, error_code::kMissingField);
    EXPECT_EQ(error_catalog::kMissingField.message, std::string_view{"A required field is missing."});
    EXPECT_EQ(error_catalog::kMissingField.category, std::string_view{"Field"});

    EXPECT_EQ(error_catalog::kInvalidCoefficient.code, error_code::kInvalidCoefficient);
    EXPECT_EQ(
        error_catalog::kInvalidCoefficient.message,
        std::string_view{"The coefficient value or layout is invalid."}
    );
    EXPECT_EQ(error_catalog::kInvalidCoefficient.category, std::string_view{"Coefficient"});

    EXPECT_EQ(error_catalog::kInvalidSource.code, error_code::kInvalidSource);
    EXPECT_EQ(error_catalog::kInvalidSource.message, std::string_view{"The source term is invalid."});
    EXPECT_EQ(error_catalog::kInvalidSource.category, std::string_view{"Source"});
}

TEST(ErrorCatalog, BoundaryAndTermDescriptorsAreStable) {
    EXPECT_EQ(error_catalog::kInvalidBoundaryCondition.code, error_code::kInvalidBoundaryCondition);
    EXPECT_EQ(
        error_catalog::kInvalidBoundaryCondition.message,
        std::string_view{"The boundary condition is invalid."}
    );
    EXPECT_EQ(error_catalog::kInvalidBoundaryCondition.category, std::string_view{"Boundary"});

    EXPECT_EQ(error_catalog::kMissingBoundaryCondition.code, error_code::kMissingBoundaryCondition);
    EXPECT_EQ(
        error_catalog::kMissingBoundaryCondition.message,
        std::string_view{"A required boundary condition is missing."}
    );
    EXPECT_EQ(error_catalog::kMissingBoundaryCondition.category, std::string_view{"Boundary"});

    EXPECT_EQ(error_catalog::kInvalidTerm.code, error_code::kInvalidTerm);
    EXPECT_EQ(error_catalog::kInvalidTerm.message, std::string_view{"The equation term is invalid."});
    EXPECT_EQ(error_catalog::kInvalidTerm.category, std::string_view{"Term"});

    EXPECT_EQ(error_catalog::kIncompatibleTerms.code, error_code::kIncompatibleTerms);
    EXPECT_EQ(
        error_catalog::kIncompatibleTerms.message,
        std::string_view{"Equation terms are incompatible."}
    );
    EXPECT_EQ(error_catalog::kIncompatibleTerms.category, std::string_view{"Term"});
}

TEST(ErrorCatalog, AssemblySystemSolverTimeAndOutputDescriptorsAreStable) {
    EXPECT_EQ(error_catalog::kAssemblyFailed.code, error_code::kAssemblyFailed);
    EXPECT_EQ(
        error_catalog::kAssemblyFailed.message,
        std::string_view{"The finite-volume system assembly failed."}
    );
    EXPECT_EQ(error_catalog::kAssemblyFailed.category, std::string_view{"Assembly"});

    EXPECT_EQ(error_catalog::kSingularSystem.code, error_code::kSingularSystem);
    EXPECT_EQ(error_catalog::kSingularSystem.message, std::string_view{"The linear system is singular."});
    EXPECT_EQ(error_catalog::kSingularSystem.category, std::string_view{"System"});

    EXPECT_EQ(error_catalog::kInvalidSystemSize.code, error_code::kInvalidSystemSize);
    EXPECT_EQ(
        error_catalog::kInvalidSystemSize.message,
        std::string_view{"The linear system size is invalid."}
    );
    EXPECT_EQ(error_catalog::kInvalidSystemSize.category, std::string_view{"System"});

    EXPECT_EQ(error_catalog::kSolverDidNotConverge.code, error_code::kSolverDidNotConverge);
    EXPECT_EQ(
        error_catalog::kSolverDidNotConverge.message,
        std::string_view{"The solver did not converge."}
    );
    EXPECT_EQ(error_catalog::kSolverDidNotConverge.category, std::string_view{"Solver"});

    EXPECT_EQ(error_catalog::kInvalidTimeStep.code, error_code::kInvalidTimeStep);
    EXPECT_EQ(error_catalog::kInvalidTimeStep.message, std::string_view{"The time step must be positive."});
    EXPECT_EQ(error_catalog::kInvalidTimeStep.category, std::string_view{"Time"});

    EXPECT_EQ(error_catalog::kInvalidTimeInterval.code, error_code::kInvalidTimeInterval);
    EXPECT_EQ(error_catalog::kInvalidTimeInterval.message, std::string_view{"The time interval is invalid."});
    EXPECT_EQ(error_catalog::kInvalidTimeInterval.category, std::string_view{"Time"});

    EXPECT_EQ(error_catalog::kOutputFileOpenFailed.code, error_code::kOutputFileOpenFailed);
    EXPECT_EQ(
        error_catalog::kOutputFileOpenFailed.message,
        std::string_view{"Output file could not be opened."}
    );
    EXPECT_EQ(error_catalog::kOutputFileOpenFailed.category, std::string_view{"Output"});

    EXPECT_EQ(error_catalog::kOutputFileWriteFailed.code, error_code::kOutputFileWriteFailed);
    EXPECT_EQ(
        error_catalog::kOutputFileWriteFailed.message,
        std::string_view{"Output file could not be written."}
    );
    EXPECT_EQ(error_catalog::kOutputFileWriteFailed.category, std::string_view{"Output"});
}

}  // namespace fvm
